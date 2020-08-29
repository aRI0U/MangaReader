#ifndef SCANTRADDOWNLOADER_H
#define SCANTRADDOWNLOADER_H

#include <QDir>
#include <QObject>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QUrl>

#include "constants.h"
#include "utils/html/QSgml.h"
#include "FileDownloader.h"


struct Chapter {
    unsigned int number;
    QString name;
    QUrl url;
};

class ScantradDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ScantradDownloader(QObject *parent = nullptr);

    void downloadLastChapters(const QString &mangaName);

signals:

private slots:
    void downloadFinished(QUrl url, QFile &file);
    void extractChaptersFromHtml(const QUrl &mangaUrl, QFile &htmlFile);
    void downloadChapter(const QDir &dir, const Chapter &chapter);

private:
    QDir htmlDir;

    QUrl baseURL;

    FileDownloader* downloader;
};

#endif // SCANTRADDOWNLOADER_H
