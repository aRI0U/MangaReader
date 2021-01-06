#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection(QObject *parent) : QObject(parent)
{
    QPath dbPath = QPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) / constants::dbFilename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    createDatabase();
}

DatabaseConnection::~DatabaseConnection() {
    db.close();
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
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT Mangas.ID, Mangas.Name, Sources.Url FROM Mangas "
                   "JOIN Sources ON Sources.Manga = Mangas.ID "
                   "WHERE Sources.Website = :website "
                   "AND Mangas.Follow "
                   "AND Mangas.LastDownload IS NULL "
                   "    OR STRFTIME('%s', 'now') - strftime('%s', Mangas.LastDownload) > :delay");
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


bool DatabaseConnection::insertManga(const int website, const QString &url, const QString &name, const QString &author, const QString &synopsis) {
    uint authorId = getAuthorId(author);

    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Mangas (Name) "
                  "VALUES (:name)");
    query.bindValue(":name", name);
    query.exec();

    if (!(db.transaction() && db.commit()))
        qDebug() << "Failed to add manga" << name << "to the database:" << db.lastError();

    query.prepare("SELECT ID From Mangas "
                  "WHERE Name = :name");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    uint id = query.value("ID").toInt();

    query.prepare("UPDATE Mangas "
                  "SET Author = :author, Synopsis = :synopsis "
                  "WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":author", authorId);
    query.bindValue(":synopsis", synopsis);

    query.prepare("INSERT OR IGNORE INTO Sources (Manga, Website, Url, Name) "
                  "VALUES (:id, :website, :url, :name)");
    query.bindValue(":id", id);
    query.bindValue(":website", website);
    query.bindValue(":url", url);
    query.bindValue(":name", name);
    query.exec();

    return (db.transaction() && db.commit());
}

int DatabaseConnection::addChapterToDatabase(const uint manga, const uint number, const QString &name, const QUrl &url) {
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Chapters (Manga, No, Title, Url) "
                  "VALUES (:manga, :number, :name, :url)");
    query.bindValue(":manga", manga);
    query.bindValue(":number", number);
    query.bindValue(":name", name);
    query.bindValue(":url", url.url());
    query.exec();

    if (!(db.transaction() && db.commit()))
        qDebug() << "Failed to add chapter" << number << ":" << name << "to database:" << db.lastError();

    query.prepare("SELECT ID FROM Chapters "
                  "WHERE Manga = :manga AND No = :number AND Title = :name AND Url = :url");
    query.bindValue(":manga", manga);
    query.bindValue(":number", number);
    query.bindValue(":name", name);
    query.bindValue(":url", url.url());
    query.exec();
    query.next();
    return query.value(0).toInt();
}

bool DatabaseConnection::chapterAlreadyRegistered(const uint manga, const uint number) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM Chapters "
                  "WHERE Manga = :manga AND No = :number");
    query.bindValue(":manga", manga);
    query.bindValue(":number", number);
    query.exec();
    return query.next();
}


uint DatabaseConnection::getMangaId(const QUrl &mangaUrl) const {
    QSqlQuery query(db);
    query.prepare("SELECT Manga FROM Sources "
                  "WHERE Url = :url");
    query.bindValue(":url", mangaUrl.url());
    query.exec();
    query.next();
    return query.value("Manga").toInt();
}

uint DatabaseConnection::getChapterId(const uint mangaId, const uint number) const {
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Chapters "
                  "WHERE Manga = :manga AND No = :number");
    query.bindValue(":manga", mangaId);
    query.bindValue(":number", number);
    query.exec();
    query.next();
    return query.value("ID").toInt();
}

QString DatabaseConnection::getMangaName(const uint &mangaId) const {
    QSqlQuery query(db);
    query.prepare("SELECT Name FROM Mangas "
                  "WHERE ID = :id");
    query.bindValue(":id", mangaId);
    query.exec();
    query.next();
    return query.value("Name").toString();
}

uint DatabaseConnection::getAuthorId(const QString &author) {
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Authors (RomajiName) "
                  "VALUES (:author)");
    query.bindValue(":author", author);
    query.exec();

    if (!(db.transaction() && db.commit()))
        qDebug() << "Failed to add author" << author;

    query.prepare("SELECT ID FROM Authors "
                  "WHERE RomajiName = :author");
    query.bindValue(":author", author);
    query.exec();
    query.next();
    return query.value("ID").toInt();
}

bool DatabaseConnection::isComplete(const uint chapterId) const {
    QSqlQuery query(db);
    query.prepare("SELECT Complete FROM Chapters "
                  "WHERE ID = :id");
    query.bindValue(":id", chapterId);
    query.exec();
    query.next();
    return query.value("Complete").toBool();
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
    qDebug() << "Creating database at" << db.databaseName();

    if (!db.open()) {
        qDebug() << db.lastError();
        return false;
    }

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
"                    Author         UNSIGNED INT,                               "
"                    Name           VARCHAR(32)     NOT NULL,                   "
"                    Follow         BOOL            DEFAULT false,              "
"                    Synopsis       TEXT,                                       "
"                    LastDownload	DATETIME,                                   "
"                    PRIMARY KEY(ID)                                            "
"                    FOREIGN KEY (Author)  REFERENCES Authors(ID)               "
"                    UNIQUE(Name)                                               "
"                );                                                             "
    );
    query.exec(
"                CREATE TABLE IF NOT EXISTS Sources (                           "
"                    Manga          INTEGER          NOT NULL,                  "
"                    Website        INTEGER          NOT NULL,                  "
"                    Url    		VARCHAR(32)      NOT NULL,                  "
"                    Name           VARCHAR(32)      NOT NULL,                  "
"                    FOREIGN KEY (Manga)     REFERENCES Mangas(ID)              "
"                    FOREIGN KEY (Website)   REFERENCES Websites(ID)            "
"                    UNIQUE (Url)                                               "
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
"                    UNIQUE (Manga, No, Title, Url)                             "
"                );                                                             "
    );
    return (db.transaction() && db.commit());
}
