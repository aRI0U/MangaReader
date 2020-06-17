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
    enterReadingModeAction = new QAction(this);
    exitReadingModeAction = new QAction(this);

    addAction(prevPagesAction);
    addAction(nextPagesAction);
    addAction(enterReadingModeAction);
    addAction(exitReadingModeAction);

    exitReadingModeAction->setEnabled(false);

    enterReadingModeAction->setShortcut(Qt::Key_A);
    exitReadingModeAction->setShortcut(Qt::Key_Q);

    connect(prevPagesAction, SIGNAL(triggered()), this, SLOT(displayPrevPages()));
    connect(nextPagesAction, SIGNAL(triggered()), this, SLOT(displayNextPages()));
//    connect(enterReadingModeAction, SIGNAL(triggered()), this, SLOT(enterReadingMode()));
//    connect(exitReadingModeAction, SIGNAL(triggered()), this, SLOT(exitReadingMode()));
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

void Reader::enterReadingMode() {
    // backup
    bakWindowFlags = windowFlags();
    bakWindowState = windowState();

    enterReadingModeAction->setEnabled(false);
    exitReadingModeAction->setEnabled(true);
    setStyleSheet("background-color:black;");
    setWindowFlags(
                  windowFlags()
                | Qt::Window
                | Qt::CustomizeWindowHint
                | Qt::WindowStaysOnTopHint
                | Qt::WindowMaximizeButtonHint
                | Qt::WindowCloseButtonHint);
    setWindowState(windowState() | Qt::WindowFullScreen);
    activateWindow();
    show();
}

void Reader::exitReadingMode() {
    enterReadingModeAction->setEnabled(true);
    exitReadingModeAction->setEnabled(false);

    setWindowFlags(bakWindowFlags);
    setWindowState(bakWindowState);
}

void Reader::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        nextPagesAction->trigger();
}

void Reader::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        enterReadingModeAction->trigger();
}
