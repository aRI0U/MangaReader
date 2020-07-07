#include "reader.h"


#include <iostream>

Reader::Reader(QWidget *parent) :
    QWidget(parent),
    layout(new QHBoxLayout),
    label(nullptr)
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

    nextPagesAction->setShortcut(Qt::Key_Left);
    prevPagesAction->setShortcut(Qt::Key_Right);
    enterReadingModeAction->setShortcut(Qt::Key_A);
    exitReadingModeAction->setShortcut(Qt::Key_Q);

    connect(prevPagesAction, SIGNAL(triggered()), this, SLOT(displayPrevPages()));
    connect(nextPagesAction, SIGNAL(triggered()), this, SLOT(displayNextPages()));
}

void Reader::setPagesDir(QDir value) {
    pagesDir = value;
    pagesList = pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name);

    if (!isActive()) {
        label = new PixmapLabel;
        label->setAlignment(Qt::AlignHCenter);

        layout->addWidget(label);
    }
    initDoublePages();
    displayPages(0);
}

bool Reader::isActive() const {
    return (label != nullptr);
}

void Reader::initDoublePages() {
    doublePages.clear();
    currentDoublePageIndex = 0;

    int nPages = pagesList.size();

    if (nPages != 0) {
        QList<QList<int>> pageGroups = {{0}, {}};

        for (int i = 1; i < nPages; i++) {
            QPixmap image = loadPage(i);

            if (image.width() > image.height()) {
                if (pageGroups.last().isEmpty())
                    pageGroups.last().append(i);
                else
                    pageGroups.append({i});

                // TODO do this directly
                QList<int> empty;
                pageGroups.append(empty);
            } else
                pageGroups.last().append(i);
        }

        // eventually remove the last group (if empty)
        if (pageGroups.last().isEmpty())
            pageGroups.removeLast();

        for (QList<int> group : pageGroups) {
            if (group.size() <= 2)
                doublePages.append(group);
            else {
                if (group.size() % 2 == 0) {
                    while (!group.isEmpty())
                        doublePages.append({group.takeFirst(), group.takeFirst()});
                } else { // TODO find a clever way to solve this problem
                    doublePages.append({group.takeFirst()});
                    while (!group.isEmpty())
                        doublePages.append({group.takeFirst(), group.takeFirst()});
                }
            }
        }
    }
    nDoublePages = doublePages.size();
}

// SLOTS

void Reader::displayPrevPages() {
    if (--currentDoublePageIndex >= 0)
        displayPages(currentDoublePageIndex);
    else
        currentDoublePageIndex = 0;
}

void Reader::displayNextPages() {
    if (++currentDoublePageIndex < nDoublePages)
        displayPages(currentDoublePageIndex);
    else {
        emit endOfChapter();
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


// PRIVATE

QPixmap Reader::loadPage(const int index) const {
    QString pagePath = pagesDir.absoluteFilePath(pagesList[index]);
    QPixmap page(pagePath);
    return page; // todo compute size dynamically
}

void Reader::displayPages(const int index) {
    QList<int> currentDoublePage = doublePages.at(index);
    switch (currentDoublePage.size()) {
        case 1:
            label->updatePixmap(loadPage(currentDoublePage.at(0)));
            break;

        case 2:
        {
            // load both images
            QPixmap rightPage = loadPage(currentDoublePage.at(0));
            QPixmap leftPage = loadPage(currentDoublePage.at(1));

            // rescale images to have the same height
            if (rightPage.height() > leftPage.height())
                leftPage = leftPage.scaledToHeight(rightPage.height(), Qt::SmoothTransformation);
            else
                rightPage = rightPage.scaledToHeight(leftPage.height(), Qt::SmoothTransformation);

            // draw both images on a new pixmap
            QPixmap collage = QPixmap(leftPage.width() + rightPage.width(), leftPage.height());
            QPainter painter(&collage);

            painter.drawPixmap(0, 0, leftPage.width(), leftPage.height(), leftPage);
            painter.drawPixmap(leftPage.width(), 0, rightPage.width(), rightPage.height(), rightPage);

            label->updatePixmap(collage);
            break;
        }

        default:
            throw std::runtime_error(
                        std::string("Reader can only display 1 or 2 images at the same time. Got a list of ") +
                        std::to_string(currentDoublePage.size()) +
                        " images.");
            break;
    };
}
