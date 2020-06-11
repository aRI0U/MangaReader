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

signals:

private:
    QHBoxLayout *layout;
    QDir pagesDir;
    QLabel *leftImg;
    QLabel *rightImg;
};

#endif // READER_H
