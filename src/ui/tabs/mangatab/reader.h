#ifndef READER_H
#define READER_H

#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QWidget>

class Reader : public QWidget
{
    Q_OBJECT
public:
    explicit Reader(QWidget *parent);
    void setPagesDir(QDir value);
    bool isActive() const;

    QPixmap loadPage(int index) const;

public slots:
    void displayNextPages();

signals:

private:
    QHBoxLayout *layout;
    QDir pagesDir;
    QStringList pagesList;

    QLabel *leftImg;
    QLabel *rightImg;

    int nPages;
    int nextPageIndex;
};

#endif // READER_H
