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
      downloader(new FileDownloader(this))
{

}
