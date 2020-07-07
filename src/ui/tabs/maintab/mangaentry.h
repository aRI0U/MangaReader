#ifndef MANGAENTRY_H
#define MANGAENTRY_H

#include <QDir>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>


#include <iostream>

class MangaEntry : public QFrame
{
    Q_OBJECT

public:
    explicit MangaEntry(QDir path);

public slots:
    void changeReadDirection();

signals:
    void clicked(QDir mangaDir);

private:
    void initEntry();

    void mousePressEvent(QMouseEvent *event) override;

    QLabel *m_cover;
    QLabel *m_title;
    QDir m_path;

    bool readDirectionLR = false;
};

#endif // MANGAENTRY_H
