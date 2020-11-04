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
    QSqlQuery *query = m_database->followedMangas(m_id);
    qDebug() << "look" << m_id;
    while (query->next()) {
        // retrieve relevant information from database
        QString mangaName(query->value("Name").toString());
        QUrl mangaUrl(query->value("Url").toString());

        // look for html file
        QDir mangaAuxDir(m_htmlDir.absoluteFilePath(mangaName));
        QFile htmlFile(mangaAuxDir.absoluteFilePath("main.html"));

        if (!mangaAuxDir.mkpath("."))
            qDebug() << "Failed to create dir" << mangaAuxDir;

        // TODO: find condition for redownloading
        if (htmlFile.exists())
            extractChaptersFromHtml(mangaUrl, htmlFile);
        else
            m_downloader->download(mangaUrl, htmlFile, FileType::MangaHTML);
    }
}


void AbstractScansDownloader::downloadMangaList() {
    QString htmlFile = m_htmlDir.absoluteFilePath("list.html");

    if (QFile::exists(htmlFile))
        generateMangaList(htmlFile);
    else
        m_downloader->download(m_listUrl, htmlFile, FileType::ListHTML);
}

bool AbstractScansDownloader::addChapterToDatabase(const QUrl &mangaUrl, const Chapter &chapter) {
    int mangaId = m_database->getMangaId(mangaUrl);
    m_database->addChapterToDatabase(mangaId, chapter.number, chapter.name, chapter.url);
}
