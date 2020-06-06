#ifndef MANGAENTRY_H
#define MANGAENTRY_H

#include <QDir>
#include <QLabel>
#include <QWidget>

class MangaEntry : public QWidget
{
    Q_OBJECT
public:
    explicit MangaEntry(QWidget *parent, QDir path);
    explicit MangaEntry(QDir path);

private:
    void initEntry();

    QLabel *m_cover;
    QLabel *m_title;
    QDir m_path;

};

#endif // MANGAENTRY_H
