#ifndef MANGAENTRY_H
#define MANGAENTRY_H

#include <QLabel>
#include <QWidget>

class MangaEntry : public QWidget
{
    Q_OBJECT
public:
    explicit MangaEntry(QWidget *parent, QString manga = "");
    explicit MangaEntry(QString manga = "");

private:
    void initEntry();

    QLabel *m_cover;
    QLabel *m_title;
    QString m_manga;

};

#endif // MANGAENTRY_H
