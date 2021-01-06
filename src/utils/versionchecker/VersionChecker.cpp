#include "VersionChecker.h"

VersionChecker::VersionChecker(QObject *parent)
    : QObject(parent),
      m_downloader(new QDownloader(this))
{
    connect(m_downloader, &QDownloader::downloadTerminated,
            this, &VersionChecker::parseVersion);
}

void VersionChecker::checkVersion() {
    QUrl url("https://raw.githubusercontent.com/aRI0U/MangaReader/master/config/windows.xml");
    m_file.open();
    m_downloader->download(url, m_file);
}

void VersionChecker::parseVersion(QDownload *dl) {
    if (!dl->success()) {
        return;
    }
    QFile *file = new QFile(dl->targetFile());
    QDomDocument doc("my document");
    doc.setContent(file);
    QDomNodeList elements = doc.elementsByTagName("Version");
    if (elements.size() == 0)
        return;
    QString lastVersion = elements.at(0).firstChild().nodeValue();

    if (compareVersions(constants::currentVersion, lastVersion))
        emit existsNewVersion(lastVersion);
}

bool VersionChecker::compareVersions(QString v1, QString v2) {
    QStringList l1 = v1.split('.');
    QStringList l2 = v2.split('.');
    int size = l1.size() < l2.size() ? l1.size() : l2.size();
    for (int i = 0; i < size; ++i) {
        int e1 = l1.at(i).toInt();
        int e2 = l2.at(i).toInt();
        if (e1 < e2)
            return true;
        if (e2 < e1)
            return false;
    }
    return (l2.size() > size);
}
