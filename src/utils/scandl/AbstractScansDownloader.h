#ifndef ABSTRACTSCANSDOWNLOADER_H
#define ABSTRACTSCANSDOWNLOADER_H

#include <QDir>

#include "constants.h"
#include "utils/download/QDownloader.h"
#include "utils/sql/DatabaseConnection.h"

enum Downloader {
    Scantrad
};

enum FileType {
    ListHTML,
    MangaHTML,
    ChapterHTML,
    Image
};

struct Chapter {
    unsigned int number;
    QString name;
    QUrl url;
};

QDebug operator<<(QDebug debug, const Chapter &chapter);

typedef QList<Chapter> QChapterList;


class AbstractScansDownloader : public QObject
{
    Q_OBJECT
public:
    explicit AbstractScansDownloader(QObject *parent = nullptr);

    void lookForNewChapters();

    virtual void downloadNewChapters() { return; }

signals:

protected slots:
    virtual void downloadFinished(QDownload *download) { return; }
    virtual void downloadChapter(const QDir &dir, const Chapter &chapter) { return; }
    virtual void extractChaptersFromHtml(const QUrl &mangaUrl, QFile &htmlFile) { return; }
    virtual void extractImagesFromChapter(QFile &chapterFile) { return; }

protected:
    void downloadMangaList();

    bool addChapterToDatabase(const QUrl &mangaUrl, const Chapter &chapter);

    virtual bool addWebsiteToDatabase() { return false; }

    virtual void generateMangaList(const QString &htmlFile) { return; }


    int m_id;

    QDir m_htmlDir;
    QUrl m_baseUrl;
    QUrl m_listUrl;

    DatabaseConnection *m_database;

    QDownloader *m_downloader;
};

#endif // ABSTRACTSCANSDOWNLOADER_H
