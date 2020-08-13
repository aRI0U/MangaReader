#ifndef SCANTRADDOWNLOADER_H
#define SCANTRADDOWNLOADER_H

#include <QDir>
#include <QObject>
#include <QStandardPaths>
#include <QUrl>
#include <QWebEnginePage>

#include "constants.h"
#include "FileDownloader.h"

class ScantradDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ScantradDownloader(QObject *parent = nullptr);

    void downloadLastChapters(const QString mangaName);

signals:

private:
    QDir htmlDir;

    QString baseURL;

    FileDownloader* downloader;
};

#endif // SCANTRADDOWNLOADER_H
