#include "reader.h"

Reader::Reader(QWidget *parent) :
    QWidget(parent),
    layout(new QHBoxLayout),
    leftImg(nullptr),
    rightImg(nullptr)
{
    layout->setSpacing(0);
    setLayout(layout);
}

void Reader::setPagesDir(QDir value)
{
    pagesDir = value;
    QVector<QPixmap> pages;
    QStringList pageList = pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name);
    for(QString& pageName: pageList) {
        QString pagePath = pagesDir.absoluteFilePath(pageName);
        QPixmap page(pagePath);
//                page = page.scaledToWidth(1000, Qt::SmoothTransformation); // todo compute size dynamically
        pages.push_back(page);
    }

    if(pages.size() >= 2) { // tmp hardcoded first 2 pages

        if(leftImg == nullptr || rightImg == nullptr) {
            leftImg = new QLabel;
            rightImg = new QLabel;

            leftImg->setScaledContents(true);
            rightImg->setScaledContents(true);

            layout->addWidget(leftImg);
            layout->addWidget(rightImg);
        }

        rightImg->setPixmap(pages[0]); // manga should be read from right to left
        leftImg->setPixmap(pages[1]);
    }
}
