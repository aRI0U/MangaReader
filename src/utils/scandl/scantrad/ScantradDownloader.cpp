#include "ScantradDownloader.h"

ScantradDownloader::ScantradDownloader(QObject *parent)
    : AbstractScansDownloader(parent)
{
    m_id = Downloader::Scantrad;

    addWebsiteToDatabase();  // TODO put this in base class

    m_baseUrl = QUrl(constants::scantradBaseUrl);
    m_listUrl = QUrl(constants::scantradAllMangasUrl);

    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dirName = "html_" + constants::scantradName;

    if (!localDataLocation.mkpath(dirName))
        qDebug() << "Failed to create path" << dirName;

    m_htmlDir = QPath(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, dirName, QStandardPaths::LocateDirectory));

    connect(m_downloader, &QDownloader::downloadTerminated,
            this, &ScantradDownloader::downloadFinished);

    downloadMangaList();
}

void ScantradDownloader::downloadFinished(QDownload *download) {
    if (!download->success()) {
        QMessageBox::critical(nullptr,
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

void ScantradDownloader::extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) {
    QFile f(htmlFile);
    QSgml *html = new QSgml(f);

    QList<QSgmlTag*> elements;
    QRegularExpression numberRegex("\\d+");
    QRegularExpressionMatch reMatch;

    html->getElementsByAttribute("class", "chapitre", &elements);

    for (QSgmlTag *elem : elements) {
        Chapter chapter;
        chapter.manga = m_database->getMangaName(mangaId);

        QSgmlTag *numberElement = elem->find("span", "class", "chl-num");
        if (numberElement == nullptr)
            qDebug() << "This chapter does not have a number element:" << elem->toString();
        else {
            reMatch = numberRegex.match(numberElement->getText());
            if (reMatch.hasMatch())
                chapter.number = reMatch.captured(0).toInt();
            else
                qDebug() << "Could not extract chapter number from this string:" << numberElement->getText();
        }

        QSgmlTag* nameElement = elem->find("span", "class", "chl-titre");
        if (nameElement == nullptr)
            qDebug() << "This chapter does not have a name element";
        else
            chapter.name = nameElement->getText();

        QSgmlTag* urlElement = elem->find("a", "class", "chr-button");
        if (urlElement == nullptr
                || urlElement->getText() != "Lire"
                || !urlElement->hasAttribute("href"))
            qDebug() << "This chapter does not have a URL element:" << elem->toString();

        else {
            chapter.url = mangaUrl.resolved(QUrl(urlElement->getArgValue("href")));
            int chapterId = addChapterToDatabase(mangaUrl, chapter);
            if (m_database->isComplete(chapterId))
                continue;

            QSettings settings;
            if (settings.value("Download/autoDownload", false).toBool()) {
                m_chaptersList.insert(chapterId, chapter);
                downloadChapter(mangaId, chapterId, chapter);
            }
        }
    }
}

void ScantradDownloader::extractImagesFromChapter(QPath &chapterFile, uint chapterId) {
    Chapter chapter = m_chaptersList.value(chapterId);

    // TODO use QStringBuilder + format language
    QString chapterName = constants::chapterFolderTemplate.arg(chapter.number).arg(chapter.name);

    QSettings settings;
    QPath chapterDir = QPath(settings.value("Library/scansPath").toString()) / chapter.manga / chapterName;

    if (!chapterDir.mkdir())
        qDebug() << "Failed to create folder" << chapterDir;

    QFile f(chapterFile);
    QSgml html(f);

    QList<QSgmlTag*> elements;

    html.getElementsByAttribute("class", "sc-lel", &elements);
    QList<QUrl> imageUrlList;

    for (QSgmlTag *elem : elements) {
        QList<QSgmlTag*> images = elem->getElementsByName("img");

        for (QSgmlTag *image : images) {
            if (!image->hasAttribute("data-src"))
                continue;

            QString imageUrl(image->getArgValue("data-src"));
            if (imageUrl.startsWith("lel"))
                imageUrlList.append(QUrl(imageUrl));
        }
    }

    m_nbImagesToDownload.insert(chapterId, imageUrlList.size());

    for (int i = 0; i < imageUrlList.size(); ++i) {
        QFile imageFile(chapterDir / (QString::number(i+1).rightJustified(2, '0') + ".png"));

        m_downloader->download(m_baseUrl.resolved(imageUrlList.at(i)), imageFile, FileType::Image, {{"id", chapterId}});
    }
    emit chapterDownloaded(chapter);
}

void ScantradDownloader::downloadChapter(const uint mangaId, const uint chapterId, const Chapter &chapter) {
    QPath chapterHtml = m_htmlDir / QString::number(mangaId) / (QString::number(chapterId) + ".html");

    m_downloader->download(chapter.url, chapterHtml, FileType::ChapterHTML, {{"id", chapterId}});
}


bool ScantradDownloader::addWebsiteToDatabase() {
    return m_database->addWebsiteToDatabase(m_id,
                                            constants::scantradName,
                                            constants::scantradBaseUrl,
                                            constants::scantradAllMangasUrl);
}

void ScantradDownloader::generateMangaList(const QString &htmlFile) {
    QFile f(htmlFile);
    QSgml html(f);

    QStringList names;
    QStringList hrefs;

    QList<QSgmlTag *> elements;

    html.getElementsByAttribute("class", "home-manga", &elements);

    QSgmlTag *titleElement;
    for (QSgmlTag *elem : elements) {
        titleElement = elem->find("div", "class", "hmi-titre");
        if (titleElement == nullptr)
            continue;
        names.append(elem->find("div", "class", "hmi-titre")->getText());
        hrefs.append(elem->getArgValue("href"));
    }

    for (int i=0; i<names.count(); ++i) {
        QUrl url = m_baseUrl.resolved(hrefs.at(i));
        QString name = names.at(i);
        m_database->insertManga(url.url(), name, m_id);
    }
}
