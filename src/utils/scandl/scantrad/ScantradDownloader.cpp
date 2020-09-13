#include "ScantradDownloader.h"

ScantradDownloader::ScantradDownloader(QObject *parent)
    : AbstractScansDownloader(parent)
{
    addWebsiteToDatabase();  // TODO put this in base class

    m_baseUrl = QUrl(constants::scantradBaseUrl);

    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dirName = "html_" + constants::scantradName;

    if (!localDataLocation.mkpath(dirName))
        qDebug() << "Failed to create path" << dirName;

    m_htmlDir = QDir(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, dirName, QStandardPaths::LocateDirectory));

    connect(m_downloader, &QDownloader::downloadTerminated,
            this, &ScantradDownloader::downloadFinished);
}


void ScantradDownloader::lookForNewChapters() {

}

void ScantradDownloader::downloadChapters(const QString &mangaName) {
    QDir mangaAuxDir(m_htmlDir.absoluteFilePath(mangaName));
    QUrl mangaUrl(m_baseUrl.resolved(QUrl(mangaName)));
    QFile htmlFile(mangaAuxDir.absoluteFilePath("main.html"));

    if (!mangaAuxDir.mkpath("."))
        qDebug() << "Failed to create dir" << mangaAuxDir;

    // eventually download html
    if (htmlFile.exists())
        extractChaptersFromHtml(mangaUrl, htmlFile);
    else
        m_downloader->download(mangaUrl, htmlFile, FileType::MangaHTML);
}

void ScantradDownloader::downloadFinished(QDownload *download) {
    if (!download->success()) {
        qDebug() << "Failed to download" << download->targetFile() << ":" << download->error();
        return;
    }

    QFile targetFile(download->targetFile());

    switch (download->kind()) {
        case FileType::MangaHTML:
            extractChaptersFromHtml(download->targetUrl(), targetFile);
            break;
        case FileType::ChapterHTML:
            extractImagesFromChapter(targetFile);
            break;
        default:
            break;
    }
}

void ScantradDownloader::extractChaptersFromHtml(const QUrl &mangaUrl, QFile &htmlFile) {
    QSgml *html = new QSgml(htmlFile);
    QDir parentDir(QFileInfo(htmlFile).dir());

    QList<QSgmlTag*> elements;
    QRegularExpression numberRegex("\\d+");
    QRegularExpressionMatch reMatch;

    html->getElementsByAttribute("class", "chapitre", &elements);
    for (QSgmlTag *elem : elements) {
        Chapter chapter;

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
            downloadChapter(parentDir, chapter);
        }
    }
}

void ScantradDownloader::extractImagesFromChapter(QFile &chapterFile) {
    Chapter chapter = chapterMetadata.value(chapterFile.fileName());

    QSgml html(chapterFile);

    QList<QSgmlTag*> elements;

    html.getElementsByAttribute("class", "sc-lel", &elements);

    QStringList tmp = chapterFile.fileName().split("/");
    QString mangaName = constants::mappings.value(tmp.at(tmp.size() - 2));

    // TODO use QStringBuilder + format language
    QString chapterName = tr("Chapitre ")
            + QString::number(chapter.number)
            + tr(" : ")
            + chapter.name;

    QSettings settings;
    QDir libraryDir(settings.value("Library/scansPath").toString());

    QDir chapterDir(libraryDir.absoluteFilePath(mangaName + "/" + chapterName));

    chapterDir.mkpath(".");

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

    for (int i = 0; i < imageUrlList.size(); ++i) {
        QFile imageFile(chapterDir.absoluteFilePath(QString::number(i+1).rightJustified(2, '0') + ".png"));

        if (imageFile.exists())
            continue;

        m_downloader->download(m_baseUrl.resolved(imageUrlList.at(i)), imageFile, FileType::Image);
    }
}

void ScantradDownloader::downloadChapter(const QDir &dir, const Chapter &chapter) {
    qDebug() << chapter;

    QFile chapterHtml(dir.absoluteFilePath("chapter_" + QString::number(chapter.number) + ".html")); // TODO use QStringBuilder

    chapterMetadata.insert(chapterHtml.fileName(), chapter);

    if (chapterHtml.exists())
        extractImagesFromChapter(chapterHtml);
    else
        m_downloader->download(chapter.url, chapterHtml, FileType::ChapterHTML);
}


bool ScantradDownloader::addWebsiteToDatabase() {
    qDebug() << "pouet";
    return m_database->addWebsiteToDatabase(constants::scantradId,
                                            constants::scantradName,
                                            constants::scantradBaseUrl,
                                            constants::scantradAllMangasUrl,
                                            constants::scantradMangaUrlFormat,
                                            constants::scantradChapterUrlFormat);
}
