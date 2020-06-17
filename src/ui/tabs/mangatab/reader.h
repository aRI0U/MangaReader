#ifndef READER_H
#define READER_H

#include <QAction>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
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

    QPixmap loadPage(const int index) const;

    QAction* enterReadingModeAction;
    QAction* exitReadingModeAction;

    void enterReadingMode();
    void exitReadingMode();

public slots:
    void displayPrevPages();
    void displayNextPages();

signals:

private:
    QHBoxLayout *layout;
    QDir pagesDir;
    QStringList pagesList;

    PixmapLabel* leftImg;
    PixmapLabel* rightImg;

    int nPages;
    int nextPageIndex;

    QAction* prevPagesAction;
    QAction* nextPagesAction;

    // backups for reading mode
    Qt::WindowFlags bakWindowFlags;
    Qt::WindowStates bakWindowState;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // READER_H
