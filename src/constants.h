#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QFont>
#include <QSize>
#include <QString>

namespace constants {
    // settings
    const QString organizationName("aRI0U-nathanlct");
    const QString applicationName("MangaReader");

    // Manga entries constants
    const QSize MangaCoverSize = QSize(200, 300);
    const QFont MangaTitleFont = QFont("Arial", 20, QFont::Bold);
}

#endif // CONSTANTS_H
