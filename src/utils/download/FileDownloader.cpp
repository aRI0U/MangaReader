#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject *parent) : QObject(parent)
{
    webCtrl = new QNetworkAccessManager(this);
    connect(webCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(writeDownloadedData(QNetworkReply*)));
}

void FileDownloader::setSourceURL(QString url) {
    sourceURL = url;
}

void FileDownloader::setTargetFile(QString file) {
    target = file;
}

void FileDownloader::download() {
    QUrl url(sourceURL);
    QNetworkRequest request(url);
    webCtrl->get(request);
}

void FileDownloader::download(QString url, QString target) {
    setSourceURL(url);
    setTargetFile(target);
    download();
}

void FileDownloader::writeDownloadedData(QNetworkReply* reply) {
    QFile targetFile(target);
    if (!targetFile.open(QIODevice::WriteOnly))
        return;
    const QByteArray data = reply->readAll();
    targetFile.write(data);
    targetFile.close();
}
