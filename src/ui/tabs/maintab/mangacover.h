#ifndef MANGACOVER_H
#define MANGACOVER_H

#include <QDir>
#include <QPixmap>
#include <QLabel>

#include <iostream>

#include "constants.h"


class MangaCover : public QLabel
{
public:
    explicit MangaCover(QDir path);

private:
    QPixmap cover;
};

#endif // MANGACOVER_H
