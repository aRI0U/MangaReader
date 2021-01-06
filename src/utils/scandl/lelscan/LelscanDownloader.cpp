#include "LelscanDownloader.h"

LelscanDownloader::LelscanDownloader(DatabaseConnection *database, QObject *parent)
    : AbstractScansDownloader(database, parent)
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
        case FileType::PageHTML:
            qDebug() << targetFile << download->error();
            downloadImage(targetFile, download->metadata("dir").toString(), id, download->metadata("page").toUInt());
            break;
        case FileType::Image:
            imageDownloaded(id);
            break;
        default:
            break;
    }
}


void LelscanDownloader::extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) {
    QFile f(htmlFile);
    QSgml html(f);

    QList<QSgmlTag*> *forms = new QList<QSgmlTag *>();
    html.getElementsByName("form", "action", "/lecture-en-ligne.php", forms);
    if (forms->size() != 1) {
        qDebug() << htmlFile << "doesn't have a lel form";
        return;
    }

    QSgmlTag *select = forms->at(0)->getElementsByName("select").at(0);
    QList<QSgmlTag *> elements = select->findAll("option");

    qDebug() << "tag" << elements.size();
    for (QSgmlTag *elem : elements) {
        Chapter chapter;

        chapter.manga = m_database->getMangaName(mangaId);

        chapter.number = elem->getText().toUInt();

        chapter.url = elem->getArgValue("value").split('"').at(0);

        uint chapterId = m_database->chapterAlreadyRegistered(mangaId, chapter.number)
                ? m_database->getChapterId(mangaId, chapter.number)
                : addChapterToDatabase(mangaUrl, chapter);

        if (m_database->isComplete(chapterId))
            continue;

        qDebug() << chapter.manga << chapter.number << chapter.url << chapterId;

        QSettings settings;
        if (settings.value("Download/autoDownload", false).toBool()) {
            m_chaptersList.insert(chapterId, chapter);
            downloadChapter(mangaId, chapterId, chapter);

        }
        break;
    }
}

void LelscanDownloader::extractImagesFromChapter(QPath &chapterFile, uint chapterId) {
    Chapter chapter = m_chaptersList.value(chapterId);

    // TODO use QStringBuilder + format language
    QString chapterName = constants::chapterFolderTemplate.arg(chapter.number).arg(chapter.name);

    QSettings settings;
    QPath chapterDir = (QPath(settings.value("Library/scansPath").toString()) / chapter.manga / chapterName).valid();

    if (!chapterDir.mkdir())
        qDebug() << "Failed to create folder" << chapterDir;

    QFile f(chapterFile);
    QSgml html(f);

    QList<QSgmlTag*> elements;

    html.getElementsByAttribute("id", "navigation", &elements);

    if (elements.size() != 1) {
        qDebug() << chapterFile << "contains" << elements.size() << "tags with id 'navigation'";
        return;
    }
    QList<QSgmlTag *> pagesTags = elements.at(0)->findAll("a");

    m_nbImagesToDownload.insert(chapterId, pagesTags.size()-1);

    for (QSgmlTag *page : pagesTags) {
        QString pageUrl = page->getArgValue("href");
        uint pageNumber = pageUrl.split('/').last().toInt();

        QPath path = QPath("/tmp") / (QString::number(chapterId) + '-' + QString::number(pageNumber));

        m_downloader->download(pageUrl, path, FileType::PageHTML, {{"dir", chapterDir}, {"id", chapterId},{"page", pageNumber}});
    }
}


void LelscanDownloader::downloadImage(const QString &pageHtml, const QString chapterDir, const uint chapterId, const uint page) {
    QFile f(pageHtml);
    QSgml html(f);

    QList<QSgmlTag *> elements;
    html.getElementsByName("div", "id", "image", &elements);

    if (elements.size() != 1) {
        qDebug() << pageHtml << "has" << elements.size() << "div tags with id 'image'";
        return;
    }

    QUrl srcUrl = m_baseUrl.resolved(elements.at(0)->find("img")->getArgValue("src"));

    QPath imageFile = QPath(chapterDir) / (QString::number(page).rightJustified(2, '0') + ".png");
    qDebug() << srcUrl << imageFile;
    m_downloader->download(srcUrl, imageFile, FileType::Image, {{"id", chapterId}});
}

void LelscanDownloader::generateMangaList(const QString &htmlFile) {
    QFile f(htmlFile);
    QSgml html(f);

    QStringList names;
    QStringList hrefs;

    QList<QSgmlTag *> elements;

    html.getElementsByAttribute("class", "hot_manga_img", &elements);

    for (QSgmlTag *elem : elements) {
        names.append(elem->getArgValue("title").chopped(5));
        hrefs.append(elem->getArgValue("href"));
    }

    for (int i=0; i<names.count(); ++i) {
        QString url = hrefs.at(i);
        QString name = names.at(i);
        m_database->insertManga(m_id, url, name);
    }
}
