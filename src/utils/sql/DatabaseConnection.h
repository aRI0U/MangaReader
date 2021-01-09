#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QDebug>
#include <QSettings>
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
    ~DatabaseConnection();

    QString lastError() const;

    bool addWebsiteToDatabase(const int id,
                              const QString &name,
                              const QString &baseUrl,
                              const QString &allMangasUrl);

    QSqlQuery *followedMangas(const uint website, const uint delay = 0) const;
    QSqlQuery *chaptersToDownload() const;
    bool restoreChaptersList();

    bool insertManga(const int website, const QString &url, const QString &name, const QString &author = "", const QString &synopsis = "");
    bool addChapterToDatabase(const uint manga, const uint number, const QString &name, const QUrl &url);
    bool addExistingChapter(const uint mangaId, const uint number, const QString &title, const uint volume);
    bool chapterAlreadyRegistered(const uint manga, const uint number);

    uint getMangaId(const QString &mangaName) const;
    QString getMangaName(const uint &mangaId) const;
    uint getAuthorId(const QString &author);
    bool isComplete(const uint chapterId) const;
    bool markAsComplete(const uint chapterId);
    bool updateLastDownloadDatetime(const uint mangaId);


signals:

private:
    bool createDatabase();

    bool exec(QString query, QHash<QString, QVariant> metadata);

    QSqlDatabase db;
    QString m_lastError;
};

#endif // DATABASECONNECTION_H
