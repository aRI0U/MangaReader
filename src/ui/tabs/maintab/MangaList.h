#ifndef MANGALIST_H
#define MANGALIST_H

#include <QLayout>
#include <QRect>
#include <QStyle>

// strongly inspired by https://doc.qt.io/qt-5/qtwidgets-layouts-flowlayout-example.html
class MangaList : public QLayout
{
    Q_OBJECT
public:
    explicit MangaList(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit MangaList(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~MangaList();

    void addItem(QLayoutItem *item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

    void clean();

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // MANGALIST_H
