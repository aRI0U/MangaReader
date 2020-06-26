#include "pixmaplabel.h"
#include <iostream>

PixmapLabel::PixmapLabel(QWidget* parent) :
    QLabel(parent)
{
    setScaledContents(false);
}

void PixmapLabel::updatePixmap (const QPixmap& p) {
    pix = p;
    setPixmap(scaledPixmap());
}

QPixmap PixmapLabel::scaledPixmap() const {
    return pix.scaledToHeight(size().height(), Qt::SmoothTransformation);
}

void PixmapLabel::resizeEvent() {
    if (!pix.isNull())
        setPixmap(scaledPixmap());
}
