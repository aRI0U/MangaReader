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
                                              const QString &allMangasUrl) {
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Websites (ID, Name, BaseUrl, AllMangasUrl) "
                  "VALUES (:ID, :Name, :BaseUrl, :AllMangasUrl)");
    query.bindValue(":ID", id);
    query.bindValue(":Name", name);
    query.bindValue(":BaseUrl", baseUrl);
    query.bindValue(":AllMangasUrl", allMangasUrl);
    query.exec();

    return (db.transaction() && db.commit());
}

QSqlQuery *DatabaseConnection::followedMangas(const uint website, const uint delay) const {
    QString delayCondition = (delay > 0)
            ? " AND LastDownload IS NULL OR STRFTIME('%s', 'now') - strftime('%s', LastDownload) > :delay"
            : "";
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT ID, Name, Url FROM Mangas "
                   "WHERE Website = :website "
                   "AND Follow "
                   "AND LastDownload IS NULL "
                   "    OR STRFTIME('%s', 'now') - strftime('%s', LastDownload) > :delay");
    query->bindValue(":website", website);
    query->bindValue(":delay", delay);
    query->exec();

    return query;
}

QSqlQuery *DatabaseConnection::chaptersToDownload() const {
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT Chapters.ID, Chapters.Manga, Mangas.Name, No, Title, Chapters.Url FROM Chapters "
                   "JOIN Mangas ON Mangas.ID = Chapters.Manga "
                   "WHERE NOT Complete");
    query->exec();
    return query;
}


bool DatabaseConnection::insertManga(const QString &url, const QString &name, const int website) {
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Mangas (Name, Url, Website) "
                  "VALUES (:name, :url, :website)");
    query.bindValue(":name", name);
    query.bindValue(":url", url);
    query.bindValue(":website", website);
    query.exec();

    return (db.transaction() && db.commit());
}

bool DatabaseConnection::addChapterToDatabase(const uint manga, const uint number, const QString &name, const QUrl &url) {
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Chapters (Manga, No, Title, Url) "
                  "VALUES (:manga, :number, :name, :url)");
    query.bindValue(":manga", manga);
    query.bindValue(":number", number);
    query.bindValue(":name", name);
    query.bindValue(":url", url.url());
    query.exec();

    return (db.transaction() && db.commit());
}

int DatabaseConnection::getMangaId(const QUrl &mangaUrl) const {
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Mangas "
                  "WHERE Url = :url");
    query.bindValue(":url", mangaUrl.url());
    query.exec();
    query.next();
    return query.value("ID").toInt();
}

bool DatabaseConnection::markAsComplete(const uint chapterId) {
    QSqlQuery query(db);
    query.prepare("UPDATE Chapters "
                  "SET Complete = True, "
                  "    DownloadDate = DATETIME() "
                  "WHERE ID = :id");
    query.bindValue(":id", chapterId);
    query.exec();

    return (db.transaction() && db.commit());
}

bool DatabaseConnection::updateLastDownloadDatetime(const uint mangaId) {
    QSqlQuery query(db);

    query.prepare("UPDATE Mangas "
                  "SET LastDownload = DATETIME() "
                  "WHERE ID = :id");
    query.bindValue(":id", mangaId);
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
"                    ID             INTEGER     	NOT NULL,                   "
"                    Name           VARCHAR(32)     NOT NULL,                   "
"                    BaseUrl		VARCHAR(256)	NOT NULL,                   "
"                    AllMangasUrl	VARCHAR(256)	NOT NULL,                   "
"                    PRIMARY KEY (ID)                                           "
"                    UNIQUE(Name)                                               "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Authors (                           "
"                    ID             INTEGER     	NOT NULL,                   "
"                    RomajiName     VARCHAR(32)     NOT NULL,                   "
"                    KanjiName      VARCHAR(32),                                "
"                    PRIMARY KEY (ID)                                           "
"                    UNIQUE(RomajiName)                                         "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Mangas (                            "
"                    ID             INTEGER         NOT NULL,                   "
"                    Website		UNSIGNED INT	NOT NULL,                   "
"                    Author         UNSIGNED INT,                               "
"                    Name           VARCHAR(32)     NOT NULL,                   "
"                    Url    		VARCHAR(32)     NOT NULL,                   "
"                    Follow         BOOL            DEFAULT false,              "
"                    Synopsis       TEXT,                                       "
"                    LastDownload	DATETIME,                                   "
"                    PRIMARY KEY(ID)                                            "
"                    FOREIGN KEY (Website) REFERENCES Websites(ID),             "
"                    FOREIGN KEY (Author)  REFERENCES Authors(ID)               "
"                    UNIQUE(Name,Url)                                           "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Chapters (                          "
"                    ID             INTEGER     	NOT NULL,                   "
"                    Manga          UNSIGNED INT	NOT NULL,                   "
"                    No             UNSIGNED INT	NOT NULL,                   "
"                    Title          VARCHAR(256),                               "
"                    Url            VARCHAR(256),                               "
"                    Complete       BOOL            DEFAULT false,              "
"                    Read           BOOL            DEFAULT false,              "
"                    ReleaseDate	DATETIME        DEFAULT CURRENT_TIMESTAMP,  "
"                    DownloadDate   DATETIME,                                   "
"                    PRIMARY KEY (ID),                                          "
"                    FOREIGN KEY (Manga) REFERENCES Mangas(ID)                  "
"                    UNIQUE (No, Title, Url)                                    "
"                );                                                             "
    );
    return (db.transaction() && db.commit());
}
