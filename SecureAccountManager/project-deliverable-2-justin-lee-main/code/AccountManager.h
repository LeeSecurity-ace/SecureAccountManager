#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QString>
#include <QSqlDatabase>

class AccountManager {
public:
    static AccountManager *getInstance();

    bool validateLogin(const QString &username, const QString &password);
    bool registerUser(const QString &username, const QString &password);
    int getUserId(const QString &username);

private:
    AccountManager();
    static AccountManager *instance;

    QSqlDatabase db;
};

#endif // ACCOUNTMANAGER_H
