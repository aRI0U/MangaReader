#ifndef READER_H
#define READER_H

#include <QAction>
#include <QCache>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QString>
#include <QWidget>

#include "pixmaplabel.h"
#include "utils/pagegroup/PageGrouper.h"

enum direction { RightToLeft = true, LeftToRight = false };

class Reader : public QWidget
{
    Q_OBJECT
public:
    explicit Reader(QWidget* parent, QString manga);
    void setPagesDir(QDir value);
    bool isActive() const;

    QAction* enterReadingModeAction;
    QAction* exitReadingModeAction;

    void initDoublePages();

    void enterReadingMode();
    void exitReadingMode();

    static bool lessThan(QString a, QString b);
    static QStringList sorted(QStringList list);

public slots:
    void swipeLeft();
    void swipeRight();

    void displayPrevPages();
    void displayNextPages();

    void updateReadingDirection();

signals:
    void endOfChapter();

private:
    // attributes
    QHBoxLayout* layout;
    QString mangaName;
    QDir pagesDir;
    QStringList pagesList;

    PixmapLabel* label;

    QList<QList<int>> doublePages;
    int nDoublePages;
    int currentDoublePageIndex;

    QCache<QString, QList<QList<int>>> doublePagesCache;

    direction readingDirection;

    // backups for reading mode
    Qt::WindowFlags bakWindowFlags;
    Qt::WindowStates bakWindowState;

    // methods
    void displayPages(const int index);
    QPixmap loadPage(const int index, const char* format = nullptr, Qt::ImageConversionFlags flags = Qt::AutoColor) const;


protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // READER_H
