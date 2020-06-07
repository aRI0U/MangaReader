#include "mangacover.h"

MangaCover::MangaCover(QDir path) : cover()
{
    QStringList coverNames = path.entryList(QStringList("*[Cc]over*.*"));
    if(coverNames.length() > 0) {
        QString coverPath = path.filePath(coverNames[0]);
        std::cout << "Loading cover at " << coverPath.toUtf8().constData() << std::endl;
        if(!cover.load(coverPath)) {
            std::cout << "Error: loading failed (not an image type or corrupted file?)" << std::endl;
        } else {
            std::cout << "\t(original) w=" << cover.width() << "; h=" << cover.height();
            cover = cover.scaled(constants::MangaCoverSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            std::cout << "\t-> (scaled) w=" << cover.width() << "; h=" << cover.height() << std::endl;
        }
    } else {
        std::cout << "Failed to find cover in " << path.absolutePath().toUtf8().constData() << std::endl;
    }

    setFixedSize(constants::MangaCoverSize);
    setPixmap(cover);
    setStyleSheet("border:1px solid black;");
    setAlignment(Qt::AlignCenter);
}
