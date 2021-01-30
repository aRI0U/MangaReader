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

QString DatabaseConnection::lastError() const {
    return m_lastError;
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
                   "WHERE NOT Complete AND Url IS NOT NULL");
    query->exec();
    return query;
}

bool DatabaseConnection::restoreChaptersList() {
    QSettings settings;
    QPath libraryPath(settings.value("Library/scansPath").toString());

    QStringList mangaPattern;
    mangaPattern << "*";

    QList<QPath> mangas = libraryPath.iter(mangaPattern, QPath::Dirs | QPath::NoDotAndDotDot);
    QRegularExpression numberRegex("\\d+");

    for (QString manga : mangas) {
        const uint mangaId = getMangaId(manga);
        const QPath mangaPath = libraryPath / manga;
        if (mangaId == 0)
            continue;
        qDebug() << mangaId << mangaPath;

        QStringList volumePattern;

        volumePattern << "Volume *";
        for (QString volume : mangaPath.iter(volumePattern, QPath::Dirs | QPath::NoDotAndDotDot)) {
             qDebug() << volume;
             QRegularExpressionMatch reMatch = numberRegex.match(volume);
             const uint volumeNumber = reMatch.captured(0).toInt();
             QStringList chapterPattern;
             chapterPattern << "Chapitre *";
             for (QString chapter : (mangaPath / volume).iter(chapterPattern, QPath::Dirs | QPath::NoDotAndDotDot)) {
                 qDebug() << chapter;
                 QRegularExpressionMatch reMatch = numberRegex.match(chapter);
                 const uint chapterNumber = reMatch.captured(0).toInt();
                 QStringList split = chapter.split(": ");
                 const QString chapterTitle = (split.size() == 2)
                         ? split.at(1)
                         : "";
                 qDebug() << mangaId << chapterNumber << chapterTitle;
                 addExistingChapter(mangaId, chapterNumber, chapterTitle, volumeNumber);
             }
        }
    }
    return false;
}


bool DatabaseConnection::insertManga(const int website, const QString &url, const QString &name, const QString &author, const QString &synopsis) {
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

    bool authorDefined = (author.size() > 0);
    bool synopsisDefined = (synopsis.size() > 0);

    QStringList fieldsToSet;
    if (authorDefined)
        fieldsToSet << "Author = :author";
    if (synopsisDefined)
        fieldsToSet << "Synopsis = :synopsis";

    if (!fieldsToSet.isEmpty()) {
        QString queryString = QString("UPDATE Mangas SET %1 WHERE ID = :id").arg(fieldsToSet.join(", "));
        query.prepare(queryString);
        query.bindValue(":id", id);
        if (authorDefined) {
            uint authorId = getAuthorId(author);
            query.bindValue(":author", authorId);
        }
        if (synopsisDefined)
            query.bindValue(":synopsis", synopsis);
        query.exec();
    }

    query.prepare("INSERT OR IGNORE INTO Sources (Manga, Website, Url, Name) "
                  "VALUES (:id, :website, :url, :name)");
    query.bindValue(":id", id);
    query.bindValue(":website", website);
    query.bindValue(":url", url);
    query.bindValue(":name", name);
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

bool DatabaseConnection::addExistingChapter(const uint mangaId, const uint number, const QString &title, const uint volume) {
    return exec("INSERT OR IGNORE INTO Chapters (Manga, No, Title, Volume) "
         "VALUES (:manga, :number, :title, :volume)",
         {
             {":manga", mangaId},
             {":number", number},
             {":title", title},
             {":volume", volume}
         }
    );
}

bool DatabaseConnection::addExistingChapter(const QVariantList &mangaId, const QVariantList &number, const QVariantList &title, const QVariantList &volume) {
    return execBatch("INSERT OR IGNORE INTO Chapters (Manga, No, Title, Volume) "
         "VALUES (:manga, :number, :title, :volume)",
         {
             {":manga", mangaId},
             {":number", number},
             {":title", title},
             {":volume", volume}
         }
    );
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


uint DatabaseConnection::getMangaId(const QString &mangaName) const {
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Mangas "
                  "WHERE Name = :name");
    query.bindValue(":name", mangaName);
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

    if (!db.open())
        return false;

    QFile scriptFile(":/scripts/create-database.sql");

    return execFile(scriptFile);
}


bool DatabaseConnection::exec(QString queryString, QHash<QString, QVariant> metadata) {
    QSqlQuery query;
    query.prepare(queryString);

    QHashIterator<QString, QVariant> it(metadata);
    while (it.hasNext()) {
        it.next();
        query.bindValue(it.key(), it.value());
    }
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    if (db.transaction() && db.commit())
        return true;
    m_lastError = db.lastError().text();
    return false;
}

bool DatabaseConnection::execBatch(QString queryString, QHash<QString, QVariantList> metadata) {
    QSqlQuery query;
    query.prepare(queryString);

    QHashIterator<QString, QVariantList> it(metadata);
    while (it.hasNext()) {
        it.next();
        query.bindValue(it.key(), it.value());
    }
    if (!query.execBatch()) {
        m_lastError = query.lastError().text();
        return false;
    }
    if (db.transaction() && db.commit())
        return true;
    m_lastError = db.lastError().text();
    return false;
}

bool DatabaseConnection::execFile(QFile &file) {
    if (!file.open(QIODevice::ReadOnly)) {
        m_lastError = file.errorString();
        return false;
    }
    QString script(file.readAll());
    file.close();

    QSqlQuery query;

    for (QString queryString : script.split(';'))
        if (!query.exec(queryString.simplified())) {
            m_lastError = query.lastError().text();
            return false;
        }

    if (db.transaction() && db.commit())
        return true;
    m_lastError = db.lastError().text();
    return false;
}
