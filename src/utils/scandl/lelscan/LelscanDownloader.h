#ifndef LELSCANDOWNLOADER_H
#define LELSCANDOWNLOADER_H

#include <QMessageBox>

#include "../AbstractScansDownloader.h"

#include "utils/html/QSgml.h"

class LelscanDownloader : public AbstractScansDownloader
{
public:
    LelscanDownloader(QObject *parent = nullptr);

private slots:
    void downloadFinished(QDownload *download) override;
//    void downloadChapter(const uint mangaId, const uint chapterId, const Chapter &chapter) override;
//    void extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) override;
//    void extractImagesFromChapter(QPath &chapterFile, uint chapterId) override;

private:
//    bool addWebsiteToDatabase() override;
    void generateMangaList(const QString &htmlFile) override;
};

#endif // LELSCANDOWNLOADER_H
