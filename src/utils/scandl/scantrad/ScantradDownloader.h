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

    void lookForNewChapters() override;
    void downloadChapters(const QString &mangaName) override;

signals:
    void chapterDownloaded(QString &chapterName);

private slots:
    void downloadFinished(QDownload *download);
    void extractChaptersFromHtml(const QUrl &mangaUrl, QFile &htmlFile);
    void extractImagesFromChapter(QFile &chapterFile);
    void downloadChapter(const QDir &dir, const Chapter &chapter);

private:
    QHash<QString, Chapter> chapterMetadata;
};

#endif // SCANTRADDOWNLOADER_H
