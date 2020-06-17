#include "pixmaplabel.h"
#include <iostream>

PixmapLabel::PixmapLabel(QWidget* parent) :
    QLabel(parent)
{
    setScaledContents(false);
}

void PixmapLabel::updatePixmap (const QPixmap& p) {
    std::cout << "init" << std::endl;
    pix = p;
    setPixmap(scaledPixmap());
}

QPixmap PixmapLabel::scaledPixmap() const {
    std::cout << size().height() << "-" << size().width() << std::endl;
    return pix.scaledToHeight(size().height(), Qt::SmoothTransformation);
}

void PixmapLabel::resizeEvent(QResizeEvent* event) {
    std::cout << "resize" << std::endl;
    if (!pix.isNull())
        setPixmap(scaledPixmap());
    else
        std::cout << "null" << std::endl;
}
