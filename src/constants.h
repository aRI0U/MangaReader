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

    // where data is stored
//    const QString localDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    // database
    const QString dbFilename = "database.db";

    // Scantrad constants
    const QString scantradName = "scantrad";
    const QString scantradBaseUrl = "https://scantrad.net";
    const QString scantradAllMangasUrl = scantradBaseUrl + "/mangas";
    const QString scantradMangaUrlFormat = scantradBaseUrl + "/%1";
    const QString scantradChapterUrlFormat = scantradBaseUrl + "/mangas/%1/%2";

    // Manga name mappings (TEMP)
    const QHash<QString, QString> mappings = {
        {"hunter-x-hunter", "Hunter x Hunter"},
        {"one-piece", "One Piece"}
    };
}

#endif // CONSTANTS_H
