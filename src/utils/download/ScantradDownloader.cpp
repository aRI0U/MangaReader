#include "ScantradDownloader.h"

QDebug operator<<(QDebug debug, const Chapter &chapter) {
    QDebugStateSaver saver(debug);
    debug.nospace().noquote()
            << "Chapter "
            << chapter.number
            << ": "
            << chapter.name
            << "<"
            << chapter.url.toString()
            << ">";
    return debug;
}

ScantradDownloader::ScantradDownloader(QObject *parent) : QObject(parent) {
    downloader = new FileDownloader(this);

    baseURL = QUrl(constants::scantradBaseUrl);

    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dirName = constants::scantradHTMLDir;

    if (!localDataLocation.mkpath(dirName))
        qDebug() << "Failed to create path" << dirName;

    htmlDir = QDir(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, dirName, QStandardPaths::LocateDirectory));

    connect(downloader, SIGNAL(fileDownloaded(QUrl,QFile&)),
            this, SLOT(downloadFinished(QUrl,QFile&)));
}

void ScantradDownloader::downloadLastChapters(const QString &mangaName) {
    QDir mangaAuxDir(htmlDir.absoluteFilePath(mangaName));
    QUrl mangaUrl(baseURL.resolved(QUrl(mangaName)));
    QFile htmlFile(mangaAuxDir.absoluteFilePath("main.html"));

    if (!mangaAuxDir.mkpath("."))
        qDebug() << "Failed to create dir" << mangaAuxDir;

    // eventually download html
    if (htmlFile.exists())
        emit downloader->fileDownloaded(mangaUrl, htmlFile);
    else
        downloader->download(mangaUrl, htmlFile);
}

void ScantradDownloader::downloadFinished(QUrl url, QFile &file) {
    QString fname = QFileInfo(file).fileName();

    if (fname.startsWith("main"))
        extractChaptersFromHtml(url, file);

    else if (fname.startsWith("chapter"))
        return;
}

void ScantradDownloader::extractChaptersFromHtml(const QUrl &mangaUrl, QFile &htmlFile) {
    QDir parentDir = QFileInfo(htmlFile).dir();
    QSgml *html = new QSgml(htmlFile);

    QList<QSgmlTag*> elements;
    QRegularExpression numberRegex("\\d+");
    QRegularExpressionMatch reMatch;

    html->getElementsByAttribute("class", "chapitre", &elements);
    for (QSgmlTag* elem : elements) {
        Chapter chapter;

        QSgmlTag* numberElement = elem->find("span", "class", "chl-num");
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

void ScantradDownloader::downloadChapter(const QDir &dir, const Chapter &chapter) {
    qDebug() << chapter;

    QFile chapterHtml(dir.absoluteFilePath("chapter_" + QString::number(chapter.number) + ".html"));

    if (!chapterHtml.exists())
        downloader->download(chapter.url, chapterHtml);
}
