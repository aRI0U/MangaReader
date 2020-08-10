#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = nullptr);

    void setSourceURL(QUrl url);
    void setSourceURL(QString url);

    void setTargetFile(QFile file);
    void setTargetFile(QString file);

    void download();
    void download(QString url, QString target);

signals:

private slots:
    void writeDownloadedData(QNetworkReply* reply);

private:
    QString sourceURL;
    QString target;

    QNetworkAccessManager* webCtrl;
};

#endif // FILEDOWNLOADER_H
