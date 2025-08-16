/*
#include "ManualDecrypt.h"
#include "aes.h" // Include your AES library
#include <QByteArray>
#include <QDebug>

#define AES_KEY reinterpret_cast<const uint8_t *>("myencryptionkey12")

QString ManualDecrypt::decryptPassword(const QString &encryptedString) {
    QByteArray combined = QByteArray::fromBase64(encryptedString.toUtf8());
    if (combined.size() < AES_BLOCKLEN) {
        qCritical() << "Decryption error: invalid data length.";
        return QString();
    }

    QByteArray iv = combined.left(AES_BLOCKLEN); // Extract IV
    QByteArray encryptedData = combined.mid(AES_BLOCKLEN); // Extract Encrypted Data

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

    QString decryptedPassword = QString::fromUtf8(encryptedData).replace(QChar('\u0000'), "");
    return decryptedPassword;
}
*/
