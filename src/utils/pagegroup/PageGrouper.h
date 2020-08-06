#ifndef PAGEGROUPER_H
#define PAGEGROUPER_H

#include <QDir>
#include <QObject>
#include <QPixmap>
#include <QPixmapCache>
#include <QStringList>

class PageGrouper : public QObject
{
    Q_OBJECT

public:
    static QList<QList<int>> groupPages(const QDir pagesDir, const QStringList pagesList);

private:
    static QPixmap loadPage(QString path, const char* format = nullptr, Qt::ImageConversionFlags flags = Qt::AutoColor);
    static QString renameFile(const QString fname, const char* format);
};

#endif // PAGEGROUPER_H
