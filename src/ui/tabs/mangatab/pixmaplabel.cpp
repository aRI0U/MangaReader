#include "pixmaplabel.h"
#include <iostream>

PixmapLabel::PixmapLabel(QWidget* parent) :
    QLabel(parent)
{
    setScaledContents(false);
}

void PixmapLabel::updatePixmap (const QPixmap& p) {
    pix = p;
    resizeEvent();
}

QPixmap PixmapLabel::scaledPixmap(const QSize size) const {
    return pix.scaledToHeight(size.height(), Qt::SmoothTransformation);
}

void PixmapLabel::resizeEvent() {
    resizeEvent(new QResizeEvent(size(), size()));
}

void PixmapLabel::resizeEvent(QResizeEvent* event) {
    if (!pix.isNull())
        setPixmap(scaledPixmap(event->size()));
}
