#include "databasemanager.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Get the database manager instance
    DatabaseManager* dbManager = DatabaseManager::getInstance();

    // Initialize database through DatabaseManager
    if (!dbManager->initializeDatabase()) {
        qCritical() << "Failed to initialize the database.";
        return -1;  // Exit the app if database fails
    }

    MainWindow w;
    w.show();

    int result = app.exec();

    // Ensure the database connection is properly closed before exiting
    dbManager->closeDatabase();

    return result;
}
