#include "ScantradDownloader.h"

ScantradDownloader::ScantradDownloader(DatabaseConnection *database, QObject *parent)
    : AbstractScansDownloader(database, parent)
{
    m_id = Downloader::Scantrad;

    addWebsiteToDatabase();  // TODO put this in base class

    m_baseUrl = QUrl(constants::scantradBaseUrl);
    m_listUrl = QUrl(constants::scantradAllMangasUrl);
    m_imagesUrl = QUrl(constants::scantradImagesUrl);

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

void ScantradDownloader::extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) {
    QHtml html(htmlFile);
    html.parse();

    QRegularExpression numberRegex("\\d+");
    QRegularExpressionMatch reMatch;

    QList<QHtmlTag *> *elements = html.findAll(nullptr, "div", {{"class", "chapitre"}});

    for (QHtmlTag *elem : *elements) {
        Chapter chapter;

        chapter.manga = m_database->getMangaName(mangaId);

        QHtmlTag *numberElement = html.find(elem, "span", {{"class", "chl-num"}});
        if (numberElement == nullptr)
            qDebug() << "This chapter does not have a number element:";
        else {
            reMatch = numberRegex.match(numberElement->text);
            if (reMatch.hasMatch())
                chapter.number = reMatch.captured(0).toInt();
            else
                qDebug() << "Could not extract chapter number from this string:" << numberElement->text;
        }

        QHtmlTag *nameElement = html.find(elem, "span", {{"class", "chl-titre"}});
        if (nameElement == nullptr) {
            qDebug() << "This chapter does not have a name element";
            chapter.name = "";
        }
        else
            chapter.name = nameElement->text;

        QHtmlTag *urlElement = html.find(elem, "a", {{"class", "ch-left"}});
        if (urlElement == nullptr || !urlElement->attributes.contains("href"))
            qDebug() << "This chapter does not have a URL element";

        else {
            chapter.url = mangaUrl.resolved(QUrl(urlElement->attributes.value("href").at(0)));
            if (!addChapterToDatabase(mangaId, chapter))
                qDebug() << "Could not add chapter" << chapter.number << ":" << chapter.name << "to the database";
        }
    }
    newChaptersAddedToDatabase(mangaId);
}

void ScantradDownloader::extractImagesFromChapter(QPath &chapterFile, uint chapterId) {
    Chapter chapter = m_chaptersList.value(chapterId);

    // TODO use QStringBuilder + format language
    QString chapterName = constants::chapterFolderTemplate.arg(chapter.number).arg(chapter.name);

    QSettings settings;
    QPath chapterDir = (QPath(settings.value("Library/scansPath").toString()) / chapter.manga / chapterName).valid();

    if (!chapterDir.mkdir())
        qDebug() << "Failed to create folder" << chapterDir;

    QHtml html(chapterFile);
    html.parse();

    QList<QHtmlTag*> *elements = html.findAll(nullptr, "div", {{"class", "sc-lel"}});
    QList<QUrl> imageUrlList;

    for (QHtmlTag *elem : *elements) {
        QList<QHtmlTag*> *images = html.findAll(elem, "img");

        for (QHtmlTag *image : *images) {
            if (!image->attributes.contains("data-src"))
                continue;

            QString imageUrl(image->attributes.value("data-src").at(0));
            if (imageUrl.startsWith("lel"))
                imageUrlList.append(QUrl(imageUrl));
        }
    }

    m_nbImagesToDownload.insert(chapterId, imageUrlList.size());

    for (int i = 0; i < imageUrlList.size(); ++i) {
        QFile imageFile(chapterDir / (QString::number(i+1).rightJustified(2, '0') + ".png"));

        m_downloader->download(m_imagesUrl.resolved(imageUrlList.at(i)), imageFile, FileType::Image, {{"id", chapterId}});
    }
    emit chapterDownloaded(chapter);
}


bool ScantradDownloader::addWebsiteToDatabase() {
    return m_database->addWebsiteToDatabase(m_id,
                                            constants::scantradName,
                                            constants::scantradBaseUrl,
                                            constants::scantradAllMangasUrl);
}

void ScantradDownloader::generateMangaList(const QString &htmlFile) {
    QHtml html(htmlFile);
    html.parse();

    QStringList names;
    QStringList hrefs;

    QList<QHtmlTag *> *elements = html.findAll(nullptr, "a", {{"class", "home-manga"}});

    for (QHtmlTag *elem : *elements) {
        QHtmlTag *titleElement = html.find(elem, "div", {{"class", "hmi-titre"}});
        if (titleElement == nullptr)
            continue;
        names.append(html.find(elem, "div", {{"class", "hmi-titre"}})->text);
        hrefs.append(elem->attributes.value("href"));
    }

    for (int i=0; i<names.count(); ++i) {
        QUrl url = m_baseUrl.resolved(hrefs.at(i));
        QString name = names.at(i);
        m_database->insertManga(m_id, url.url(), name);
    }
}
