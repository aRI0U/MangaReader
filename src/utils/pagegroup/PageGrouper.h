#ifndef PAGEGROUPER_H
#define PAGEGROUPER_H

#include <QDebug>
#include <QDir>
#include <QImageReader>
#include <QObject>
#include <QPixmap>
#include <QPixmapCache>
#include <QStringList>

class PageGrouper : public QObject
{
    Q_OBJECT

public:
    static QStringList correctExtensions(QStringList pagesList);
    static QList<QList<int>> groupPages(const QStringList pagesList);

private:
    static QPixmap loadPage(QString path, const char* format = nullptr, Qt::ImageConversionFlags flags = Qt::AutoColor);
    static QString loadCorruptedImage(QString path, QPixmap& image);
    static QString renameFile(const QString fname, const char* format);
};

#endif // PAGEGROUPER_H
