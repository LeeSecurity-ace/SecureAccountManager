#include "entrymanager.h"
#include "aes.h"
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QRandomGenerator>

#define AES_KEY reinterpret_cast<const uint8_t *>("myencryptionkey12")

EntryManager *EntryManager::instance = nullptr;

EntryManager::EntryManager() {}

EntryManager *EntryManager::getInstance() {
    if (!instance) {
        instance = new EntryManager();
    }
    return instance;
}

void EntryManager::addEntry(ServiceEntry *entry, int userId) {
    QSqlQuery query;
    query.prepare("INSERT INTO entries (user_id, service_name, username, password, strength, last_changed) "
                  "VALUES (:user_id, :service_name, :username, :password, :strength, :last_changed)");
    query.bindValue(":user_id", userId);
    query.bindValue(":service_name", entry->getServiceName());
    query.bindValue(":username", entry->getUsername());

    // Debug: Show original password
    QString originalPassword = entry->getPassword();
   // qDebug() << "Original Password:" << originalPassword;

    QString encryptedPassword = encrypt(originalPassword);

    // Debug: Show encrypted password
    //qDebug() << "Encrypted Password (Base64):" << encryptedPassword;

    query.bindValue(":password", encryptedPassword);
    query.bindValue(":strength", entry->getPasswordStrength());
    query.bindValue(":last_changed", entry->getLastChanged());

    if (!query.exec()) {
        qCritical() << "Failed to add entry:" << query.lastError().text();
    } else {
        emit entryAdded(entry);
    }
}

void EntryManager::editEntry(int id, const QString &serviceName, const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("UPDATE entries SET service_name = :service_name, username = :username, password = :password, "
                  "last_changed = datetime('now') WHERE id = :id");
    query.bindValue(":service_name", serviceName);
    query.bindValue(":username", username);

    // Debug: Show original password during edit
    //qDebug() << "Original Password (Edit):" << password;

    QString encryptedPassword = encrypt(password);

    // Debug: Show encrypted password during edit
    //qDebug() << "Encrypted Password (Base64, Edit):" << encryptedPassword;

    query.bindValue(":password", encryptedPassword);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qCritical() << "Failed to edit entry:" << query.lastError().text();
    }
}

void EntryManager::deleteEntry(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM entries WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qCritical() << "Failed to delete entry:" << query.lastError().text();
    }
}

QList<ServiceEntry *> EntryManager::loadEntries(int userId) {
    QList<ServiceEntry *> entries;

    QSqlQuery query;
    query.prepare("SELECT id, service_name, username, password, strength, last_changed "
                  "FROM entries WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (query.exec()) {
        while (query.next()) {
            QString encryptedPassword = query.value("password").toString();

            // Debug: Show encrypted password being loaded
            //qDebug() << "Encrypted Password (Base64, Load):" << encryptedPassword;

            QString decryptedPassword;

            try {
                decryptedPassword = decrypt(encryptedPassword);

                // Debug: Show decrypted password after loading
                //qDebug() << "Decrypted Password (Load):" << decryptedPassword;
            } catch (...) {
                decryptedPassword = encryptedPassword; // Assume plaintext
                QString reEncryptedPassword = encrypt(decryptedPassword);
                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE entries SET password = :password WHERE id = :id");
                updateQuery.bindValue(":password", reEncryptedPassword);
                updateQuery.bindValue(":id", query.value("id").toInt());
                updateQuery.exec();
            }

            ServiceEntry *entry = new ServiceEntry(
                query.value("service_name").toString(),
                query.value("username").toString(),
                decryptedPassword
                );
            entry->setId(query.value("id").toInt());
            entry->setPasswordStrength(query.value("strength").toString());
            entry->setLastChanged(query.value("last_changed").toString());

            entries.append(entry);
        }
    } else {
        qCritical() << "Failed to load entries:" << query.lastError().text();
    }

    return entries;
}

QString EntryManager::encrypt(const QString &data) {
    QByteArray input = data.toUtf8();
    int paddedLength = (input.size() + AES_BLOCKLEN - 1) / AES_BLOCKLEN * AES_BLOCKLEN;
    QByteArray paddedInput = input.leftJustified(paddedLength, '\0');

    QByteArray iv(AES_BLOCKLEN, '\0');
    for (int i = 0; i < AES_BLOCKLEN; ++i) {
        iv[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, AES_KEY, reinterpret_cast<const uint8_t *>(iv.data()));
    AES_CBC_encrypt_buffer(&ctx, reinterpret_cast<uint8_t *>(paddedInput.data()), paddedLength);

    QByteArray combined = iv + paddedInput;
    return combined.toBase64();
}

QString EntryManager::decrypt(const QString &data) {
    QByteArray combined = QByteArray::fromBase64(data.toUtf8());
    if (combined.size() < AES_BLOCKLEN) {
        qCritical() << "Decryption error: invalid data length.";
        return QString();
    }

    QByteArray iv = combined.left(AES_BLOCKLEN);
    QByteArray encryptedData = combined.mid(AES_BLOCKLEN);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, AES_KEY, reinterpret_cast<const uint8_t *>(iv.data()));
    AES_CBC_decrypt_buffer(&ctx, reinterpret_cast<uint8_t *>(encryptedData.data()), encryptedData.size());

    // PKCS#7 padding removal
    if (!encryptedData.isEmpty()) {
        int lastByte = static_cast<uint8_t>(encryptedData[encryptedData.size() - 1]);
        if (lastByte > 0 && lastByte <= AES_BLOCKLEN) {
            encryptedData.chop(lastByte);
        }
    }

    // Convert to QString and remove null characters
    QString decryptedPassword = QString::fromUtf8(encryptedData);

    // Explicitly remove '\u0000' if present
    decryptedPassword.replace(QChar('\u0000'), "");

    // Debug: Show decrypted password after trimming null characters
    //qDebug() << "Decrypted Password (Trimmed):" << decryptedPassword;

    return decryptedPassword;
}

