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
    const QString translationsPath("translations");
    const QString currentVersion("0.1.1");

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
    const QString chapterFolderTemplate = "Chapitre %1 : %2";

    // database
    const QString dbFilename = "database.db";

    /* SCAN DOWNLOADING */

    const int refreshPeriod = 12*3600;  // 12 hours

    // Scantrad constants
    const QString scantradName = "scantrad";
    const QString scantradBaseUrl = "https://scantrad.net";
    const QString scantradAllMangasUrl = scantradBaseUrl + "/mangas";
    const QString scantradImagesUrl = "https://scan-trad.fr";
}

#endif // CONSTANTS_H
