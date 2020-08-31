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
    const QSize MangaCoverSize = QSize(210, 297);
    const QFont MangaTitleFont = QFont("Arial", 20, QFont::Bold);

    // Scantrad constants
    const QString scantradHTMLDir = "html_scantrad";
    const QString scantradBaseUrl = "https://scantrad.net";

    // Manga name mappings
    const QHash<QString, QString> mappings = {
        {"hunter-x-hunter", "Hunter x Hunter"},
        {"one-piece", "One Piece"}
    };
}

#endif // CONSTANTS_H
