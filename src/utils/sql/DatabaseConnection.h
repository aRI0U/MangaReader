#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include "constants.h"

class DatabaseConnection : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConnection(QObject *parent = nullptr);

    bool addWebsiteToDatabase(const int id,
                              const QString &name,
                              const QString &baseUrl,
                              const QString &allMangasUrl);

    QSqlQuery *followedMangas(const int website) const;
    bool insertManga(const QString &url, const QString &name, const int website);

signals:

private:
    bool createDatabase();

    QSqlDatabase db;
};

#endif // DATABASECONNECTION_H
