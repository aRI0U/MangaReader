#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection(QObject *parent) : QObject(parent)
{
    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QString dbPath(localDataLocation.absoluteFilePath(constants::dbFilename));

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    createDatabase(db);
}


bool DatabaseConnection::createDatabase(QSqlDatabase &db) {
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
        qDebug() << query.isActive() << query.lastError().text();
    query.exec(
"                CREATE TABLE IF NOT EXISTS Authors (                           "
"                    ID             UNSIGNED INT	NOT NULL,                   "
"                    RomajiName     VARCHAR(32)     NOT NULL,                   "
"                    KanjiName      VARCHAR(32),                                "
"                    PRIMARY KEY (ID)                                           "
"                );                                                             "
    );
        qDebug() << query.isActive() << query.lastError().text();
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
        qDebug() << query.isActive() << query.lastError().text();
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
    qDebug() << query.isActive() << query.lastError().text();
    db.transaction();
    db.commit();
    qDebug() << db.lastError().text();
    db.close();
    return true;
}
