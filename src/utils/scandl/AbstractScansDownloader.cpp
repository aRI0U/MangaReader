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
      m_downloader(new QDownloader(this))
{

}

void AbstractScansDownloader::downloadChapters(const QString &mangaName) {}
