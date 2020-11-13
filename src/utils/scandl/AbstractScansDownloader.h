#ifndef ABSTRACTSCANSDOWNLOADER_H
#define ABSTRACTSCANSDOWNLOADER_H

#include "constants.h"
#include "libs/QDownload/QDownloader.h"
#include "libs/QPath/QPath.h"
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
    QString manga;
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

    void downloadNewChapters();

signals:
    void chapterDownloaded();

protected slots:
    virtual void downloadFinished(QDownload*) { }
    virtual void downloadChapter(const uint, const uint, const Chapter&) { }
    virtual void extractChaptersFromHtml(const QUrl&, QPath&) { }
    virtual void extractImagesFromChapter(QPath&) { }
    void imageDownloaded(uint chapterId);

protected:
    void downloadMangaList();

    int addChapterToDatabase(const QUrl &mangaUrl, const Chapter &chapter);

    virtual bool addWebsiteToDatabase() { return false; }

    virtual void generateMangaList(const QString&) {}


    int m_id;

    QPath m_htmlDir;
    QUrl m_baseUrl;
    QUrl m_listUrl;

    DatabaseConnection *m_database;

    QDownloader *m_downloader;

    // mappings
    QHash<QString, uint> m_dirnameToChapterId;
    QHash<QString, uint> m_htmlToChapterId;
    QHash<QString, uint> m_htmlToMangaId;

    QHash<uint, uint> m_nbImagesToDownload;
    QHash<uint, Chapter> m_chaptersList;
};

#endif // ABSTRACTSCANSDOWNLOADER_H
