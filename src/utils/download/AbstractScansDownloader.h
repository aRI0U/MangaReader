#ifndef ABSTRACTSCANSDOWNLOADER_H
#define ABSTRACTSCANSDOWNLOADER_H

#include <QDir>

#include "constants.h"

#include "FileDownloader.h"


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

//    virtual void downloadChapters(const QString &mangaName);

signals:

protected:
    QDir htmlDir;
    QUrl baseUrl;
    FileDownloader* downloader;
};

#endif // ABSTRACTSCANSDOWNLOADER_H
