#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager* getInstance(); // Singleton access method
    bool initializeDatabase();
    QSqlDatabase getDatabase();
    void closeDatabase();

private:
    DatabaseManager();  // Private constructor for Singleton
    ~DatabaseManager(); // Destructor for proper cleanup
    static DatabaseManager* instance; // Singleton instance

    QSqlDatabase db; // Database connection
};

#endif // DATABASEMANAGER_H
