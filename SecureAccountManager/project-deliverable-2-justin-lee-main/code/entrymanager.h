#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H

#include "serviceentry.h"
#include <QObject>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QString>

class EntryManager : public QObject {
    Q_OBJECT

public:
    static EntryManager *getInstance();

    void addEntry(ServiceEntry *entry, int userId);
    void editEntry(int id, const QString &serviceName, const QString &username, const QString &password);
    void deleteEntry(int id);
    QList<ServiceEntry *> loadEntries(int userId);

    QString encrypt(const QString &data);
    QString decrypt(const QString &data);

signals:
    void entryAdded(ServiceEntry *entry);
    void entryUpdated(int index, ServiceEntry *entry);
    void entryDeleted(int index);

private:
    static EntryManager *instance;

    EntryManager();
};

#endif
