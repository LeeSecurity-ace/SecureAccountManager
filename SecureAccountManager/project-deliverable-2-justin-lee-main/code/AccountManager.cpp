#include "AccountManager.h"
#include "databasemanager.h"
#include "entrymanager.h" // For encryption and decryption
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AccountManager *AccountManager::instance = nullptr;

AccountManager::AccountManager() {
    db = DatabaseManager::getInstance()->getDatabase(); // Use shared DatabaseManager connection
}

AccountManager *AccountManager::getInstance() {
    if (!instance)
        instance = new AccountManager();
    return instance;
}

bool AccountManager::validateLogin(const QString &username, const QString &password) {
    QSqlQuery query(db); // Use the shared connection
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.next()) {
            QString encryptedPassword = query.value(0).toString(); // Get the stored encrypted password

            // Decrypt the stored password
            QString decryptedPassword = EntryManager::getInstance()->decrypt(encryptedPassword);

            // Compare the decrypted password with the entered password
            if (decryptedPassword == password) {
                qDebug() << "Login successful for username:" << username;
                return true;
            } else {
                qDebug() << "Login failed: Password mismatch.";
            }
        } else {
            qDebug() << "Login failed: No matching username found.";
        }
    } else {
        qDebug() << "Login failed: Query execution error:" << query.lastError().text();
    }
    return false;
}

int AccountManager::getUserId(const QString &username) {
    QSqlQuery query(db); // Use the shared connection
    query.prepare("SELECT id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1; // Return -1 if user ID is not found
}

bool AccountManager::registerUser(const QString &username, const QString &password) {
    // Encrypt the password before storing it
    QString encryptedPassword = EntryManager::getInstance()->encrypt(password);


    QSqlQuery query(db); // Use the shared connection
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", encryptedPassword); // Store encrypted password

    if (query.exec()) {
        qDebug() << "Registration successful for username:" << username;
        return true;
    } else {
        qDebug() << "Registration failed:" << query.lastError().text();
        return false;
    }
}
