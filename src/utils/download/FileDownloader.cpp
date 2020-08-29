#include "FileDownloader.h"

FileDownloader::FileDownloader(QObject *parent) : QObject(parent)
{
}

void FileDownloader::download(QUrl url, QFile &target) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(writeDownloadedData(QNetworkReply*)));

    targetFiles[manager] = target.fileName();

    manager->get(QNetworkRequest(url));
}

void FileDownloader::writeDownloadedData(QNetworkReply *reply) {
    QNetworkAccessManager *parentManager = reply->manager();
    QFile targetFile(targetFiles.take(parentManager));
    if (!targetFile.open(QIODevice::WriteOnly))
        return;
    const QByteArray data = reply->readAll();
    targetFile.write(data);
    targetFile.close();

    reply->deleteLater();

    emit fileDownloaded(reply->url(), targetFile);
}
