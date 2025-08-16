#ifndef SERVICEENTRY_H
#define SERVICEENTRY_H

#include <QString>

class ServiceEntry {
public:
    ServiceEntry(const QString &serviceName, const QString &username, const QString &password);

    QString getServiceName() const;
    QString getUsername() const;
    QString getPassword() const;
    QString getPasswordStrength() const;
    QString getLastChanged() const;
    int getId() const; // Getter for ID

    void setServiceName(const QString &serviceName);
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setPasswordStrength(const QString &strength);
    void setLastChanged(const QString &lastChanged);
    void setId(int id); // Setter for ID

private:
    int id; // Unique ID for the entry
    QString serviceName;
    QString username;
    QString password;
    QString passwordStrength;
    QString lastChanged;
};

#endif // SERVICEENTRY_H
