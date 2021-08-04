#ifndef LELSCANDOWNLOADER_H
#define LELSCANDOWNLOADER_H

#include <QMessageBox>

#include "../AbstractScansDownloader.h"

#include "libs/QHtml/QHtml.h"

class LelscanDownloader : public AbstractScansDownloader
{
public:
    LelscanDownloader(DatabaseConnection *database, QObject *parent = nullptr);

private slots:
    void downloadFinished(QDownload *download) override;
    void extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) override;
//    void extractImagesFromChapter(QPath &chapterFile, uint chapterId) override;

private:
//    bool addWebsiteToDatabase() override;
    void generateMangaList(const QString &htmlFile) override;
};

#endif // LELSCANDOWNLOADER_H
