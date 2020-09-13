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

    while (query->next()) {
        QMap<QString, QVariant> manga(query->boundValues());
        qDebug() << manga.value("FullName") << manga.value("urlName");
    }
}


void AbstractScansDownloader::downloadMangaList() {
    QString htmlFile = m_htmlDir.absoluteFilePath("list.html");

    if (QFile::exists(htmlFile))
        generateMangaList(htmlFile);
    else
        m_downloader->download(m_listUrl, htmlFile, FileType::ListHTML);
}
