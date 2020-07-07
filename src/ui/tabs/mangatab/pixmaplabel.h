#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QSize>

class PixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget* parent = nullptr);

    void resizeEvent();
    void updatePixmap( const QPixmap& );

protected:
    void resizeEvent(QResizeEvent* event);

private:
    QPixmap pix;

    QPixmap scaledPixmap(const QSize size) const;
};

#endif // PIXMAPLABEL_H


// taken from https://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
