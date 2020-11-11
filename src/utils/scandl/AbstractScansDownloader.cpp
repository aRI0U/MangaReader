#include "AbstractScansDownloader.h"

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

AbstractScansDownloader::AbstractScansDownloader(QObject *parent)
    : QObject(parent),
      m_database(new DatabaseConnection(this)),
      m_downloader(new QDownloader(this))
{
}

void AbstractScansDownloader::lookForNewChapters() {
    QSqlQuery *query = m_database->followedMangas(m_id, constants::refreshPeriod);
    while (query->next()) {
        qDebug() << "Looking for new chapters of " << query->value("Name").toString();
        // retrieve relevant information from database
        QString mangaId(query->value("ID").toString());
        QUrl mangaUrl(query->value("Url").toString());

        // look for html file
        QPath mangaAuxDir = m_htmlDir / mangaId;
        QFile htmlFile(mangaAuxDir / "main.html");

        if (!mangaAuxDir.mkdir())
            qDebug() << "Failed to create dir" << mangaAuxDir;

        // download main.html
        m_htmlToMangaId.insert(htmlFile.fileName(), mangaId.toInt());
        m_downloader->download(mangaUrl, htmlFile, FileType::MangaHTML);
    }
}

void AbstractScansDownloader::downloadNewChapters() {
    QSqlQuery *query = m_database->chaptersToDownload();

    while (query->next()) {
        uint chapterId = query->value("ID").toUInt();
        // get chapter metadata
        Chapter chapter;
        chapter.number = query->value("No").toInt();
        chapter.name = query->value("Title").toString();
        chapter.manga = query->value("Name").toString();
        chapter.url = QUrl(query->value("Url").toString());

        // choose a file to store html
        QPath html = m_htmlDir / query->value("Manga").toString() / (query->value("ID").toString() + ".html");

        // add chapter to mappings
        m_chaptersList.insert(chapterId, chapter);
        m_htmlToChapterId.insert(html, chapterId);

        // download
        downloadChapter(html, chapter);
    }
}


void AbstractScansDownloader::imageDownloaded(uint chapterId) {
    if (--m_nbImagesToDownload[chapterId] == 0) {
        m_nbImagesToDownload.remove(chapterId);
        m_database->markAsComplete(chapterId);
    }
}


void AbstractScansDownloader::downloadMangaList() {
    QString htmlFile = m_htmlDir / "list.html";

    if (QFile::exists(htmlFile))
        generateMangaList(htmlFile);
    else
        m_downloader->download(m_listUrl, htmlFile, FileType::ListHTML);
}

bool AbstractScansDownloader::addChapterToDatabase(const QUrl &mangaUrl, const Chapter &chapter) {
    int mangaId = m_database->getMangaId(mangaUrl);
    return m_database->addChapterToDatabase(mangaId, chapter.number, chapter.name, chapter.url);
}
