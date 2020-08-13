#include "ScantradDownloader.h"

ScantradDownloader::ScantradDownloader(QObject *parent) : QObject(parent)
{
    downloader = new FileDownloader(this);

    baseURL = constants::scantradBaseUrl;

    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dirName = constants::scantradHTMLDir;

    if (!localDataLocation.mkpath(dirName))
        qDebug() << "Failed to create path" << dirName;

    htmlDir = QDir(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, dirName, QStandardPaths::LocateDirectory));
}

void ScantradDownloader::downloadLastChapters(const QString mangaName) {
    QWebEnginePage* webPage = new QWebEnginePage(this);
    QString htmlLocation = htmlDir.absoluteFilePath(mangaName + ".html");

    // eventually download html
    if (!QFile::exists(htmlLocation))
        downloader->download(baseURL + "/" + mangaName, htmlLocation);

    // read html
    webPage->load("file://" + htmlLocation);
    qDebug() << htmlLocation;
}

