#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QDomDocument>
#include <QMessageBox>
#include <QObject>
#include <QTemporaryFile>

#include "constants.h"
#include "libs/QDownload/QDownloader.h"

class VersionChecker : public QObject
{
    Q_OBJECT
public:
    explicit VersionChecker(QObject *parent = nullptr);
    void checkVersion();

signals:
    void existsNewVersion(QString version);

private slots:
    void parseVersion(QDownload *dl);

private:
    QDownloader *m_downloader;
    QTemporaryFile m_file;

    bool compareVersions(QString v1, QString v2);
};

#endif // VERSIONCHECKER_H
