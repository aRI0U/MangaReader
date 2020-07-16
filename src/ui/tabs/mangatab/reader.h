#ifndef READER_H
#define READER_H

#include <QAction>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

#include "pixmaplabel.h"

class Reader : public QWidget
{
    Q_OBJECT
public:
    explicit Reader(QWidget *parent);
    void setPagesDir(QDir value);
    bool isActive() const;

    QAction* enterReadingModeAction;
    QAction* exitReadingModeAction;

    void initDoublePages();

    void enterReadingMode();
    void exitReadingMode();

public slots:
    void displayPrevPages();
    void displayNextPages();

signals:
    void endOfChapter();

private:
    // attributes
    QHBoxLayout *layout;
    QDir pagesDir;
    QStringList pagesList;

    PixmapLabel* label;

    QList<QList<int>> doublePages;
    int nDoublePages;
    int currentDoublePageIndex;

    QAction* prevPagesAction;
    QAction* nextPagesAction;

    // backups for reading mode
    Qt::WindowFlags bakWindowFlags;
    Qt::WindowStates bakWindowState;

    // methods
    void displayPages(const int index);
    QPixmap loadPage(const int index, const char* format = nullptr, Qt::ImageConversionFlags flags = Qt::AutoColor) const;
    void renameFile(const int index, const char* format);


protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // READER_H
