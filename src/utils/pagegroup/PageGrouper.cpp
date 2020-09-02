#include "PageGrouper.h"


QPixmap PageGrouper::loadPage(const QString path,  const char* format, const Qt::ImageConversionFlags flags) {
    QPixmap page;

    if (!QPixmapCache::find(path, &page)) {
        if (page.load(path, format, flags))
            QPixmapCache::insert(path, page);
    }

    return page;
}

QString PageGrouper::loadCorruptedImage(QString path, QPixmap &image) {
    QList<QByteArray> supportedFormats = QImageReader::supportedImageFormats();
    supportedFormats.removeAt(4);
    qDebug() << supportedFormats;
    for (const char* format : supportedFormats) {
        qDebug() << format;
        if (image.load(path, format)) {
            QPixmapCache::insert(path, image);
            return renameFile(path, format);
        }
    }
    qDebug() << "no valid format";
    return QString();
}

QString PageGrouper::renameFile(const QString fname, const char *format) {
    // build new name
    const QString newName = fname.left(fname.size() - 3) + format;

    // rename file
    QFile::rename(fname, newName);

    // return new file name
    return newName;
}

QStringList PageGrouper::correctExtensions(QStringList pagesList) {
    for (int i = 0; i < pagesList.size(); i++) {
        QString path = pagesList[i];
        QPixmap image = loadPage(path);
        if (image.isNull()) {
            path = loadCorruptedImage(path, image);
        }
    }
    return pagesList;
}

QList<QList<int>> PageGrouper::groupPages(QStringList pagesList) {
    int nPages = pagesList.size();
    QList<QList<int>> doublePages;

    if (nPages != 0) {
        QList<QList<int>> pageGroups = {{0}, {}};

        for (int i = 0; i < nPages; i++) {
            QString pagePath = pagesList.at(i);
            QPixmap image = loadPage(pagePath);

            if (image.isNull())
                qDebug() << "invalid image:" << pagePath;

            if (i == 0)
                continue;

            if (image.width() > image.height()) {
                if (pageGroups.last().isEmpty())
                    pageGroups.last().append(i);
                else
                    pageGroups.append({i});

                // TODO do this directly
                QList<int> empty;
                pageGroups.append(empty);
            } else
                pageGroups.last().append(i);
        }

        // eventually remove the last group (if empty)
        if (pageGroups.last().isEmpty())
            pageGroups.removeLast();

        for (QList<int> group : pageGroups) {
            if (group.size() <= 2)
                doublePages.append(group);
            else {
                if (group.size() % 2 == 0) {
                    while (!group.isEmpty())
                        doublePages.append({group.takeFirst(), group.takeFirst()});
                } else { // TODO find a clever way to solve this problem
                    doublePages.append({group.takeFirst()});
                    while (!group.isEmpty())
                        doublePages.append({group.takeFirst(), group.takeFirst()});
                }
            }
        }
    }
    return doublePages;
}
