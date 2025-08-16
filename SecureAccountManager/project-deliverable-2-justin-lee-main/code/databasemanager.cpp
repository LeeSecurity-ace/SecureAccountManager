#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

// Initialize the Singleton instance to nullptr
DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() : db(QSqlDatabase::addDatabase("QSQLITE")) {}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

DatabaseManager* DatabaseManager::getInstance() {
    if (!instance) {
        instance = new DatabaseManager();
    }
    return instance;
}

bool DatabaseManager::initializeDatabase() {
    if (db.isValid() && db.isOpen()) {
        qDebug() << "Database already initialized.";
        return true;
    }

    QString dbPath = QDir::currentPath() + "/accounts.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database initialized at:" << dbPath;

    QSqlQuery query;

    // Create users table
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT UNIQUE, "
                    "password TEXT)")) {
        qCritical() << "Failed to create users table:" << query.lastError().text();
        return false;
    }

    // Create entries table
    if (!query.exec("CREATE TABLE IF NOT EXISTS entries ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "service_name TEXT, "
                    "username TEXT, "
                    "password TEXT, "
                    "strength TEXT, "
                    "last_changed TEXT, "
                    "user_id INTEGER, "
                    "FOREIGN KEY(user_id) REFERENCES users(id))")) {
        qCritical() << "Failed to create entries table:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}

void DatabaseManager::closeDatabase() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed.";
    }
}
