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
    readingModeAction = new QAction(this);

    addAction(prevPagesAction);
    addAction(nextPagesAction);
    addAction(readingModeAction);

    connect(prevPagesAction, SIGNAL(triggered()), this, SLOT(displayPrevPages()));
    connect(nextPagesAction, SIGNAL(triggered()), this, SLOT(displayNextPages()));
    connect(readingModeAction, SIGNAL(triggered(bool)), this, SLOT(readingMode(bool)));
}

void Reader::setPagesDir(QDir value) {
    pagesDir = value;
    pagesList = pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name);

    nPages = pagesList.size();
    nextPageIndex = 0;

    if (leftImg == nullptr || rightImg == nullptr) {
        leftImg = new QLabel;
        rightImg = new QLabel;

//        leftImg->setScaledContents(true);
//        rightImg->setScaledContents(true);

        layout->addWidget(leftImg);
        layout->addWidget(rightImg);
    }

    displayNextPages();
}

bool Reader::isActive() const {
    std::cout << leftImg << " - " << rightImg << std::endl;
    return (leftImg != nullptr);
}

QPixmap Reader::loadPage(const int index) const {
    QString pagePath = pagesDir.absoluteFilePath(pagesList[index]);
    QPixmap page(pagePath);
    return page.scaledToHeight(height(), Qt::SmoothTransformation); // todo compute size dynamically
}

// SLOTS

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

void Reader::readingMode(bool checked) {
    std::cout << checked << std::endl;
    if (checked) {
        setParent(nullptr);
        setStyleSheet("background-color:black;");
        // focuspolicy?
        setWindowFlags(
                      windowFlags()
                    | Qt::Window
                    | Qt::CustomizeWindowHint
                    | Qt::WindowStaysOnTopHint
                    | Qt::WindowMaximizeButtonHint
                    | Qt::WindowCloseButtonHint);
        setWindowState(windowState() | Qt::WindowFullScreen);
        show();
    } else {
        close();
    }
}

void Reader::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        nextPagesAction->trigger();
//        readingModeAction->setChecked(!readingModeAction->isChecked());
    else
        std::cout << "no effect" << std::endl;
}

void Reader::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        readingModeAction->setChecked(!readingModeAction->isChecked());
    else
        std::cout << "no effect" << std::endl;
}
