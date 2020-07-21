#include "reader.h"

Reader::Reader(QWidget* parent, QString manga) :
    QWidget(parent),
    layout(new QHBoxLayout),
    label(nullptr),
    mangaName(manga)
{
    layout->setSpacing(0);
    setLayout(layout);

    leftAction = new QAction(this);
    rightAction = new QAction(this);
    enterReadingModeAction = new QAction(this);
    exitReadingModeAction = new QAction(this);

    addAction(leftAction);
    addAction(rightAction);
    addAction(enterReadingModeAction);
    addAction(exitReadingModeAction);

    exitReadingModeAction->setEnabled(false);

    leftAction->setShortcut(Qt::Key_Left);
    rightAction->setShortcut(Qt::Key_Right);
    enterReadingModeAction->setShortcut(Qt::Key_A);
    exitReadingModeAction->setShortcut(Qt::Key_Q);

    connect(leftAction, SIGNAL(triggered()), this, SLOT(swipeLeft()));
    connect(rightAction, SIGNAL(triggered()), this, SLOT(swipeRight()));

    updateReadingDirection();
}

void Reader::setPagesDir(QDir value) {
    pagesDir = value;
    pagesList = pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name);

    pagesList = sorted(pagesList);

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

            if (image.isNull()) {
                for (const char* format : {"png", "jpg"}) {
                    image = loadPage(i, format);
                    if (!image.isNull())
                        renameFile(i, format);
                }
            }

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
        qDebug() << "Double pages successfully initialized:" << doublePages;
    }
    nDoublePages = doublePages.size();
}

// SLOTS
void Reader::swipeLeft() {
    if (readingDirection == RightToLeft)
        displayNextPages();
    else
        displayPrevPages();
}

void Reader::swipeRight() {
    if (readingDirection == LeftToRight)
        displayNextPages();
    else
        displayPrevPages();
}

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
        displayNextPages();
}

void Reader::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        enterReadingModeAction->trigger();
}


// PRIVATE

QPixmap Reader::loadPage(const int index, const char* format, Qt::ImageConversionFlags flags) const {
    QString pagePath = pagesDir.absoluteFilePath(pagesList.at(index));

    QPixmap page(pagePath, format, flags);

    if (page.isNull())
        qDebug() << "Error loading " << pagePath;

    return page;
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
            QPixmap leftPage, rightPage;

            if (readingDirection == RightToLeft) {
                rightPage = loadPage(currentDoublePage.at(0));
                leftPage = loadPage(currentDoublePage.at(1));
            }
            else {
                leftPage = loadPage(currentDoublePage.at(0));
                rightPage = loadPage(currentDoublePage.at(1));
            }

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

void Reader::renameFile(const int index, const char* format) {
    // build old and new name
    const QString oldName = pagesDir.absoluteFilePath(pagesList.at(index));
    const QString newName = oldName.left(oldName.size() - 3) + format;

    // rename file
    QFile::rename(oldName, newName);

    // update pagesList
    QString fname = newName.split("/").last();
    pagesList.replace(index, fname);
}


void Reader::updateReadingDirection() {
    QSettings settings;

    readingDirection = static_cast<direction>(settings.value(mangaName + "/readingDirection", RightToLeft).toBool());
}

bool Reader::lessThan(QString a, QString b) {
    // if there is a number in the string, compare those numbers
    QRegularExpression re("\\d+");
    QRegularExpressionMatch match_a, match_b;

    match_a = re.match(a);
    match_b = re.match(b);

    int matches = 2*match_a.hasMatch() + match_b.hasMatch(); // trick to compare two booleans at the same time

    switch (matches) {
        case 3:  // (true, true)
            return (match_a.captured(0).toInt() < match_b.captured(0).toInt());

        case 2:  // (true, false)
            return false;

        case 1:  // (false, true)
            return true;

        case 0:  // (false, false)
            return (a < b);

        default:
            throw std::runtime_error("matches is composed of 2 bits and is thus in [0, 3]");
    }
}

QStringList Reader::sorted(QStringList list) {
    std::sort(list.begin(),
              list.end(),
              Reader::lessThan);
    return list;
}
