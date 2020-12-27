#include "LelscanDownloader.h"

LelscanDownloader::LelscanDownloader(QObject *parent)
    : AbstractScansDownloader(parent)
{
    m_id = Downloader::Lelscan;

    addWebsiteToDatabase();

    m_baseUrl = QUrl(constants::lelscanBaseUrl);
    m_listUrl = QUrl(constants::lelscanBaseUrl);
//    m_imagesUrl = QUrl(constants::lelscanImagesUrl);

    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dirName = "html_" + constants::lelscanName;

    if (!localDataLocation.mkpath(dirName))
        qDebug() << "Failed to create path" << dirName;

    m_htmlDir = QPath(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, dirName, QStandardPaths::LocateDirectory));

    connect(m_downloader, &QDownloader::downloadTerminated,
            this, &LelscanDownloader::downloadFinished);

    downloadMangaList();
}

void LelscanDownloader::downloadFinished(QDownload *download) {
    if (!download->success()) {
        QMessageBox::warning(nullptr,
                             download->targetFile(),
                             download->targetUrl().url() + download->error());
        return;
    }

    QPath targetFile(download->targetFile());
    uint id = download->metadata("id", 0).toUInt();

    switch (download->kind()) {
        case FileType::ListHTML:
            generateMangaList(targetFile);
            break;
        case FileType::MangaHTML:
            m_database->updateLastDownloadDatetime(id);
            extractChaptersFromHtml(download->targetUrl(), targetFile, id);
            break;
        case FileType::ChapterHTML:
            extractImagesFromChapter(targetFile, id);
            break;
        case FileType::Image:
            imageDownloaded(id);
            break;
        default:
            break;
    }
}


void LelscanDownloader::generateMangaList(const QString &htmlFile) {
    QFile f(htmlFile);
    QSgml html(f);

    QStringList names;
    QStringList hrefs;

    QList<QSgmlTag *> elements;

    html.getElementsByAttribute("class", "hot_manga_img", &elements);

    qDebug() << "lelscan" << elements.size();

    for (int i=0; i<names.count(); ++i) {
        QUrl url = m_baseUrl.resolved(hrefs.at(i));
        QString name = names.at(i);
        m_database->insertManga(m_id, url.url(), name);
    }
}
