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
}

#endif // CONSTANTS_H
