#include "PageGrouper.h"


QPixmap PageGrouper::loadPage(const QString path,  const char* format, const Qt::ImageConversionFlags flags) {
    QPixmap page;

    if (!QPixmapCache::find(path, &page)) {
        page.load(path, format, flags);
        QPixmapCache::insert(path, page);
    }

    return page;
}

QString PageGrouper::renameFile(const QString fname, const char* format) {
    // build new name
    const QString newName = fname.left(fname.size() - 3) + format;

    // rename file
    QFile::rename(fname, newName);

    // return new file name
    return newName.split("/").last();
}

QList<QList<int>> PageGrouper::groupPages(QDir pagesDir, QStringList pagesList) {
    int nPages = pagesList.size();
    QList<QList<int>> doublePages;

    if (nPages != 0) {
        QList<QList<int>> pageGroups = {{0}, {}};

        for (int i = 0; i < nPages; i++) {
            QString pagePath = pagesDir.absoluteFilePath(pagesList.at(i));
            QPixmap image = loadPage(pagePath);

            if (image.isNull()) {
                for (const char* format : {"png", "jpg"}) {
                    image = loadPage(pagePath, format);
                    if (!image.isNull()) {
                        QString newName = renameFile(pagePath, format);
                        pagesList.replace(i, newName);
                    }
                }
            }

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
