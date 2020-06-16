#include "reader.h"
#include <iostream>

Reader::Reader(QWidget *parent) :
    QWidget(parent),
    layout(new QHBoxLayout),
    leftImg(nullptr),
    rightImg(nullptr)
{
    layout->setSpacing(0);
    setLayout(layout);

    prevPagesAction = new QAction(this);
    nextPagesAction = new QAction(this);

    addAction(prevPagesAction);
    addAction(nextPagesAction);

    connect(prevPagesAction, SIGNAL(triggered()), this, SLOT(displayPrevPages()));
    connect(nextPagesAction, SIGNAL(triggered()), this, SLOT(displayNextPages()));
}

void Reader::setPagesDir(QDir value) {
    pagesDir = value;
    pagesList = pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name);

    nPages = pagesList.size();
    nextPageIndex = 0;

    if (leftImg == nullptr || rightImg == nullptr) {
        leftImg = new QLabel;
        rightImg = new QLabel;

        leftImg->setScaledContents(true);
        rightImg->setScaledContents(true);

        layout->addWidget(leftImg);
        layout->addWidget(rightImg);
    }

    displayNextPages();
}

bool Reader::isActive() const {
    std::cout << leftImg << " - " << rightImg << std::endl;
    return (leftImg != nullptr);
}

QPixmap Reader::loadPage(int index) const {
    QString pagePath = pagesDir.absoluteFilePath(pagesList[index]);
    QPixmap page(pagePath);
    return page.scaledToWidth(1000, Qt::SmoothTransformation); // todo compute size dynamically
}

void Reader::displayPrevPages() {
    std::cout << "TODO display previous pages" << std::endl;
}

void Reader::displayNextPages() {
    for (QLabel* img : {rightImg, leftImg}) {
        if (nextPageIndex < nPages)
            img->setPixmap(loadPage(nextPageIndex++));
        else
            img->clear();
    }
}

void Reader::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        nextPagesAction->trigger();
    else
        std::cout << "no effect" << std::endl;
}
