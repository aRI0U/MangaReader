#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QFont>
#include <QSize>
#include <QStandardPaths>
#include <QString>

namespace constants {
    // settings
    const QString organizationName("aRI0U-nathanlct");
    const QString applicationName("MangaReader");
    const QString translationsPath("../" + applicationName + "/translations");

    // Sizes
    const QSize MangaCoverSize = QSize(210, 297);

    const QSize mainWindowMinimumSize(800, 600);
    const QSize settingsWindowMinimumSize(600, 400);
    const int settingsMenuWidth(140);
    const int settingsWidgetWidth(200);

    // Fonts
    const QFont MangaTitleFont = QFont("Arial", 20, QFont::Bold);

    /* SCAN STORING */
    // folder name template
    const QString chapterFolderTemplate = QSysInfo::productType() == "windows"
            ? "Chapitre %1_ %2"
            : "Chapitre %1 : %2";

    // database
    const QString dbFilename = "database.db";

    /* SCAN DOWNLOADING */

    const int refreshPeriod = 12*3600;  // 12 hours

    // Scantrad constants
    const QString scantradName = "scantrad";
    const QString scantradBaseUrl = "https://scantrad.net";
    const QString scantradAllMangasUrl = scantradBaseUrl + "/mangas";

    // Manga name mappings (TEMP)
    const QHash<QString, QString> mappings = {
        {"hunter-x-hunter", "Hunter x Hunter"},
        {"one-piece", "One Piece"}
    };
}

#endif // CONSTANTS_H
