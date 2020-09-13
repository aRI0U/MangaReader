#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "constants.h"

class DatabaseConnection : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConnection(QObject *parent = nullptr);

signals:

private:
    bool createDatabase(QSqlDatabase &db);
};

#endif // DATABASECONNECTION_H
