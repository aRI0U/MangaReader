#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDir>
#include <QFont>
#include <QSize>

namespace constants {
    const QDir ScansPath = QDir("/Users/nathan/projects/MangaReader/scans");



    // Manga entries constants
    const QSize MangaCoverSize = QSize(200, 300);
    const QFont MangaTitleFont = QFont("Arial", 20, QFont::Bold);
}

#endif // CONSTANTS_H
