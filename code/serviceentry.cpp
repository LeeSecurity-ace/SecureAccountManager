#include "serviceentry.h"

ServiceEntry::ServiceEntry(const QString &serviceName, const QString &username, const QString &password)
    : id(-1), serviceName(serviceName), username(username), password(password), lastChanged("") {}

QString ServiceEntry::getServiceName() const { return serviceName; }
QString ServiceEntry::getUsername() const { return username; }
QString ServiceEntry::getPassword() const { return password; }
QString ServiceEntry::getPasswordStrength() const { return passwordStrength; }
QString ServiceEntry::getLastChanged() const { return lastChanged; }
int ServiceEntry::getId() const { return id; }

void ServiceEntry::setServiceName(const QString &serviceName) { this->serviceName = serviceName; }
void ServiceEntry::setUsername(const QString &username) { this->username = username; }
void ServiceEntry::setPassword(const QString &password) { this->password = password; }
void ServiceEntry::setPasswordStrength(const QString &strength) { this->passwordStrength = strength; }
void ServiceEntry::setLastChanged(const QString &lastChanged) { this->lastChanged = lastChanged; }
void ServiceEntry::setId(int id) { this->id = id; }
