#include "mangacover.h"
#include "constants.h"

MangaCover::MangaCover(QDir path)// : QPixmap(path.filePath("cover.png"))
{
    QString extensions[] = {"png", "jpg"};  // TODO: make it clean
    for (const auto &ext : extensions) {
        QFile coverFile(path.filePath("cover." + ext));
        if (coverFile.exists()) {
            QImage cover = QPixmap(coverFile.fileName()).scaled(constants::MangaCoverSize, Qt::KeepAspectRatioByExpanding).toImage();
            convertFromImage(cover);
            break;
        }
    };
    scaledToHeight(200);
}
