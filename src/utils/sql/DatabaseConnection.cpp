#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection(QObject *parent) : QObject(parent)
{
    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dbPath(localDataLocation.absoluteFilePath(constants::dbFilename));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    createDatabase();
}


bool DatabaseConnection::addWebsiteToDatabase(const int id,
                                              const QString &name,
                                              const QString &baseUrl,
                                              const QString &allMangasUrl,
                                              const QString &mangaUrlFormat,
                                              const QString &chapterUrlFormat) {
    QSqlQuery query(db);
    // check if the website has already been inserted
    query.prepare("SELECT ID FROM Websites WHERE ID = :ID");
    query.bindValue(":ID", id);
    query.exec();

    if (query.next())
        return true;

    // insert the website in the tab
    query.prepare("INSERT INTO Websites (ID, Name, BaseUrl, AllMangasUrl, MangaUrlFormat, ChapterUrlFormat)"
                  "VALUES (:ID, :Name, :BaseUrl, :AllMangasUrl, :MangaUrlFormat, :ChapterUrlFormat)");
    query.bindValue(":ID", id);
    query.bindValue(":Name", name);
    query.bindValue(":BaseUrl", baseUrl);
    query.bindValue(":AllMangasUrl", allMangasUrl);
    query.bindValue(":MangaUrlFormat", mangaUrlFormat);
    query.bindValue(":ChapterUrlFormat", chapterUrlFormat);
    query.exec();

    return (db.transaction() && db.commit());
}


bool DatabaseConnection::createDatabase() {
    qDebug() << "creating database at" << db.databaseName();

    if (!db.open())
        return false;

    QSqlQuery query;
    query.exec(
"                CREATE TABLE IF NOT EXISTS Websites (                          "
"                    ID             UNSIGNED INT	NOT NULL,                   "
"                    Name           VARCHAR(32)     NOT NULL,                   "
"                    BaseUrl		VARCHAR(256)	NOT NULL,                   "
"                    AllMangasUrl	VARCHAR(256)	NOT NULL,                   "
"                    MangaUrlFormat	VARCHAR(256)	NOT NULL,                   "
"                    ChapterUrlFormat VARCHAR(256)	NOT NULL,                   "
"                    PRIMARY KEY (ID)                                           "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Authors (                           "
"                    ID             UNSIGNED INT	NOT NULL,                   "
"                    RomajiName     VARCHAR(32)     NOT NULL,                   "
"                    KanjiName      VARCHAR(32),                                "
"                    PRIMARY KEY (ID)                                           "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Mangas (                            "
"                    ID             UNSIGNED INT	NOT NULL,                   "
"                    Website		UNSIGNED INT	NOT NULL,                   "
"                    Author         UNSIGNED INT	NOT NULL,                   "
"                    FullName       VARCHAR(32)     NOT NULL,                   "
"                    UrlName		VARCHAR(32)     NOT NULL,                   "
"                    Synopsis       TEXT,                                       "
"                    LastDownload	DATETIME,                                   "
"                    PRIMARY KEY (ID),                                          "
"                    FOREIGN KEY (Website) REFERENCES Websites(ID),             "
"                    FOREIGN KEY (Author)  REFERENCES Authors(ID)               "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Chapters (                          "
"                    ID             UNSIGNED INT	NOT NULL,                   "
"                    Manga          UNSIGNED INT	NOT NULL,                   "
"                    No             UNSIGNED INT	NOT NULL,                   "
"                    Title          VARCHAR(256),                               "
"                    Complete       BOOL            DEFAULT false,              "
"                    DownloadDate	DATETIME        DEFAULT CURRENT_TIMESTAMP,  "
"                    PRIMARY KEY (ID),                                          "
"                    FOREIGN KEY (Manga) REFERENCES Mangas(ID)                  "
"                );                                                             "
    );
    return (db.transaction() && db.commit());
}
