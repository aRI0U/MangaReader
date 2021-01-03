#ifndef SCANTRADDOWNLOADER_H
#define SCANTRADDOWNLOADER_H

#include <QMessageBox>
#include <QRegularExpression>
#include <QStandardPaths>

#include "../AbstractScansDownloader.h"

#include "utils/html/QSgml.h"

class ScantradDownloader : public AbstractScansDownloader
{
    Q_OBJECT
public:
    explicit ScantradDownloader(DatabaseConnection *database, QObject *parent = nullptr);

signals:

private slots:
    void downloadFinished(QDownload *download) override;
    void extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile, uint mangaId) override;
    void extractImagesFromChapter(QPath &chapterFile, uint chapterId) override;

private:
    bool addWebsiteToDatabase() override;
    void generateMangaList(const QString &htmlFile) override;
};

#endif // SCANTRADDOWNLOADER_H
