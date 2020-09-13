#ifndef ABSTRACTSCANSDOWNLOADER_H
#define ABSTRACTSCANSDOWNLOADER_H

#include <QDir>

#include "constants.h"
#include "utils/download/QDownloader.h"
#include "utils/sql/DatabaseConnection.h"

enum FileType {
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

    virtual void lookForNewChapters() {}
    virtual void downloadChapters(const QString &mangaName) {}

signals:

protected:
    QDir m_htmlDir;
    QUrl m_baseUrl;

    DatabaseConnection *m_database;

    QDownloader *m_downloader;
};

#endif // ABSTRACTSCANSDOWNLOADER_H
