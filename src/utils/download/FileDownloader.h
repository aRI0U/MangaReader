#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = nullptr);

    void download(QUrl url, QFile &target);

signals:

private slots:
    void writeDownloadedData(QNetworkReply *reply);

private:
    QHash<QNetworkAccessManager*, QString> targetFiles;
};

#endif // FILEDOWNLOADER_H
