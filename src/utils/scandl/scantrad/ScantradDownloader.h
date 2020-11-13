#ifndef SCANTRADDOWNLOADER_H
#define SCANTRADDOWNLOADER_H

#include <QRegularExpression>
#include <QSettings>
#include <QStandardPaths>

#include "../AbstractScansDownloader.h"

#include "utils/html/QSgml.h"

class ScantradDownloader : public AbstractScansDownloader
{
    Q_OBJECT
public:
    explicit ScantradDownloader(QObject *parent = nullptr);

signals:

private slots:
    void downloadFinished(QDownload *download) override;
    void downloadChapter(const uint mangaId, const uint chapterId, const Chapter &chapter) override;
    void extractChaptersFromHtml(const QUrl &mangaUrl, QPath &htmlFile) override;
    void extractImagesFromChapter(QPath &chapterFile) override;

private:
    bool addWebsiteToDatabase() override;
    void generateMangaList(const QString &htmlFile) override;
};

#endif // SCANTRADDOWNLOADER_H
