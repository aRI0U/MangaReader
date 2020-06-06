#ifndef MANGACOVER_H
#define MANGACOVER_H

#include <QDir>
#include <QPixmap>

class MangaCover : public QPixmap
{
public:
    explicit MangaCover(QDir path);
};

#endif // MANGACOVER_H
