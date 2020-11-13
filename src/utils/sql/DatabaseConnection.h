#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrl>

#include "constants.h"
#include "libs/QPath/QPath.h"

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
    int addChapterToDatabase(const uint manga, const uint number, const QString &name, const QUrl &url);

    uint getMangaId(const QUrl &mangaUrl) const;
    QString getMangaName(const uint &mangaId) const;
    bool isComplete(const uint chapterId) const;
    bool markAsComplete(const uint chapterId);
    bool updateLastDownloadDatetime(const uint mangaId);


signals:

private:
    bool createDatabase();

    QSqlDatabase db;
};

#endif // DATABASECONNECTION_H
