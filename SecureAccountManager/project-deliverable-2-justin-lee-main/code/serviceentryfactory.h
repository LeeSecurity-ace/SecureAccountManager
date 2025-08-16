#ifndef SERVICEENTRYFACTORY_H
#define SERVICEENTRYFACTORY_H

#include "serviceentry.h"
#include <QString>
#include <QSqlError>

class ServiceEntryFactory {
public:
    static ServiceEntry *createEntry(const QString &serviceName, const QString &username, const QString &password);
};

#endif
