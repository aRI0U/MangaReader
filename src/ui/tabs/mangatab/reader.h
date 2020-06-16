#ifndef READER_H
#define READER_H

#include <QAction>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QWidget>

class Reader : public QWidget
{
    Q_OBJECT
public:
    explicit Reader(QWidget *parent);
    void setPagesDir(QDir value);
    bool isActive() const;

    QPixmap loadPage(const int index) const;

public slots:
    void displayPrevPages();
    void displayNextPages();
    void readingMode(bool enabled);

signals:

private:
    QHBoxLayout *layout;
    QDir pagesDir;
    QStringList pagesList;

    QLabel* leftImg;
    QLabel* rightImg;

    int nPages;
    int nextPageIndex;

    QAction* prevPagesAction;
    QAction* nextPagesAction;
    QAction* readingModeAction;

    // backups for reading mode

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // READER_H
