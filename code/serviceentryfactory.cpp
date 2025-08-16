#include "serviceentryfactory.h"
#include <QSqlError>

ServiceEntry *ServiceEntryFactory::createEntry(const QString &serviceName, const QString &username, const QString &password) {
    return new ServiceEntry(serviceName, username, password);
}
