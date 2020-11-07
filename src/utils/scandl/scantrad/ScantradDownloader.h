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
    void downloadChapter(const QString &file, const Chapter &chapter) override;
    void extractChaptersFromHtml(const QUrl &mangaUrl, QFile &htmlFile) override;
    void extractImagesFromChapter(QFile &chapterFile) override;

private:
    bool addWebsiteToDatabase() override;
    void generateMangaList(const QString &htmlFile) override;
};

#endif // SCANTRADDOWNLOADER_H
