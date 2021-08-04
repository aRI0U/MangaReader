//#include "LelscanDownloader.h"

//LelscanDownloader::LelscanDownloader(DatabaseConnection *database, QObject *parent)
//    : AbstractScansDownloader(database, parent)
//{
//    m_id = Downloader::Lelscan;

//    addWebsiteToDatabase();

//    m_baseUrl = QUrl(constants::lelscanBaseUrl);
//    m_listUrl = QUrl(constants::lelscanBaseUrl);
////    m_imagesUrl = QUrl(constants::lelscanImagesUrl);

//    QDir localDataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
//    QString dirName = "html_" + constants::lelscanName;

//    if (!localDataLocation.mkpath(dirName))
//        qDebug() << "Failed to create path" << dirName;

//    m_htmlDir = QPath(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, dirName, QStandardPaths::LocateDirectory));

//    connect(m_downloader, &QDownloader::downloadTerminated,
//            this, &LelscanDownloader::downloadFinished);

//    downloadMangaList();
//}

//void LelscanDownloader::downloadFinished(QDownload *download) {
//    if (!download->success()) {
//        QMessageBox::warning(nullptr,
//                             download->targetFile(),
//                             download->targetUrl().url() + download->error());
//        return;
//    }

//    QPath targetFile(download->targetFile());
//    uint id = download->metadata("id", 0).toUInt();

//    switch (download->kind()) {
//        case FileType::ListHTML:
//            generateMangaList(targetFile);
//            break;
//        case FileType::MangaHTML:
//            m_database->updateLastDownloadDatetime(id);
//            extractChaptersFromHtml(download->targetUrl(), targetFile, id);
//            break;
//        case FileType::ChapterHTML:
//            extractImagesFromChapter(targetFile, id);
//            break;
//        case FileType::Image:
//            imageDownloaded(id);
//            break;
//        default:
//            break;
//    }
//}


//void LelscanDownloader::extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) {
//    QFile f(htmlFile);
//    QSgml html(f);

//    QList<QSgmlTag*> *forms = new QList<QSgmlTag *>();
//    html.getElementsByName("form", "action", "/lecture-en-ligne.php", forms);
//    if (forms->size() != 1) {
//        qDebug() << htmlFile << "doesn't have a lel form";
//        return;
//    }

//    QSgmlTag *select = forms->at(0)->getElementsByName("select").at(0);
//    QList<QSgmlTag *> elements = select->findAll("option");

//    qDebug() << "tag" << elements.size();
//    for (QSgmlTag *elem : elements) {
//        Chapter chapter;

//        chapter.manga = m_database->getMangaName(mangaId);

//        chapter.number = elem->getText().toUInt();

//        chapter.url = elem->getArgValue("value").split('"').at(0);

//        qDebug() << chapter.number << chapter.manga << chapter.url << m_database->chapterAlreadyRegistered(mangaId, chapter.number);

//        if (m_database->chapterAlreadyRegistered(mangaId, chapter.number))
//            continue;

//        if (!addChapterToDatabase(mangaId, chapter))
//            qDebug() << "Could not add chapter" << chapter.number << ":" << chapter.name << "to the database";
//    }
//    newChaptersAddedToDatabase(mangaId);
//}


//void LelscanDownloader::generateMangaList(const QString &htmlFile) {
//    QFile f(htmlFile);
//    QSgml html(f);

//    QStringList names;
//    QStringList hrefs;

//    QList<QSgmlTag *> elements;

//    html.getElementsByAttribute("class", "hot_manga_img", &elements);

//    qDebug() << "lelscan" << elements.size();
//    for (QSgmlTag *elem : elements) {
//        names.append(elem->getArgValue("title").chopped(5));
//        hrefs.append(elem->getArgValue("href"));
//    }

//    for (int i=0; i<names.count(); ++i) {
//        QString url = hrefs.at(i);
//        QString name = names.at(i);
//        m_database->insertManga(m_id, url, name);
//    }
//}
