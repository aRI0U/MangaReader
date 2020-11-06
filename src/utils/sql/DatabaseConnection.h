#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrl>

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

    QSqlQuery *followedMangas(const uint website, const uint delay = 0) const;
    QSqlQuery *chaptersToDownload() const;

    bool insertManga(const QString &url, const QString &name, const int website);
    bool addChapterToDatabase(const uint manga, const uint number, const QString &name, const QUrl &url);

    int getMangaId(const QUrl &mangaUrl) const;
    bool markAsComplete(const uint chapterId);
    bool updateLastDownloadDatetime(const uint mangaId);


signals:

private:
    bool createDatabase();

    QSqlDatabase db;
};

#endif // DATABASECONNECTION_H
