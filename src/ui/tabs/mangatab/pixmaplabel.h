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

    QPixmap scaledPixmap() const;

public slots:
    void updatePixmap( const QPixmap& );
    void resizeEvent( QResizeEvent* );

private:
    QPixmap pix;
};

#endif // PIXMAPLABEL_H


// taken from https://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
