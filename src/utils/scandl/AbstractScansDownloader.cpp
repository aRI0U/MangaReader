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

AbstractScansDownloader::AbstractScansDownloader(DatabaseConnection *database, QObject *parent)
    : QObject(parent),
      m_database(database),
      m_downloader(new QDownloader(this))
{
    m_downloader->setDefaultPolicy(QOverwritePolicy::NoOverwrite);
}

void AbstractScansDownloader::lookForNewChapters() {
    QSqlQuery *query = m_database->followedMangas(m_id, constants::refreshPeriod);
    while (query->next()) {
        // retrieve relevant information from database
        QString mangaId(query->value("ID").toString());
        QString mangaName(query->value("Name").toString());
        QUrl mangaUrl(query->value("Url").toString());

        qDebug() << "Looking for new chapters of" << mangaName;

        // look for html file
        QPath mangaAuxDir = m_htmlDir / mangaId;
        QPath htmlFile(mangaAuxDir / "main.html");

        if (!mangaAuxDir.mkdir())
            qDebug() << "Failed to create dir" << mangaAuxDir;

        m_followedMangas << mangaId.toUInt();

        m_downloader->download(mangaUrl, htmlFile, FileType::MangaHTML, {{"id", mangaId}}, QOverwritePolicy::Overwrite);
    }
    if (m_followedMangas.isEmpty())
        emit databaseUpdated();
}

void AbstractScansDownloader::downloadNewChapters() {
    QSqlQuery *query = m_database->chaptersToDownload();

    while (query->next()) {
        uint mangaId = query->value("Manga").toUInt();
        uint chapterId = query->value("ID").toUInt();

        // get chapter metadata
        Chapter chapter;
        chapter.number = query->value("No").toInt();
        chapter.name = query->value("Title").toString();
        chapter.manga = query->value("Name").toString();
        chapter.url = QUrl(query->value("Url").toString());

        // add chapter to mappings
        m_chaptersList.insert(chapterId, chapter);

        // download
        downloadChapter(mangaId, chapterId, chapter);
    }
}

void AbstractScansDownloader::downloadChapter(const uint mangaId, const uint chapterId, const Chapter &chapter) {
    QPath chapterHtml = m_htmlDir / QString::number(mangaId) / (QString::number(chapterId) + ".html");

    m_downloader->download(chapter.url, chapterHtml, FileType::ChapterHTML, {{"id", chapterId}});
}

void AbstractScansDownloader::imageDownloaded(uint chapterId) {
    if (--m_nbImagesToDownload[chapterId] == 0) {
        m_nbImagesToDownload.remove(chapterId);
        m_database->markAsComplete(chapterId);
    }
}

void AbstractScansDownloader::newChaptersAddedToDatabase(uint mangaId) {
    m_followedMangas.removeOne(mangaId);
    if (m_followedMangas.isEmpty())
        emit databaseUpdated();
}


void AbstractScansDownloader::downloadMangaList() {
    QString htmlFile = m_htmlDir / "list.html";

    if (QFile::exists(htmlFile))
        generateMangaList(htmlFile);
    else
        m_downloader->download(m_listUrl, htmlFile, FileType::ListHTML);
}

bool AbstractScansDownloader::addChapterToDatabase(const uint mangaId, const Chapter &chapter) {
    return m_database->addChapterToDatabase(mangaId, chapter.number, chapter.name, chapter.url);
}
