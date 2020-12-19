#include "Reader.h"

Reader::Reader(QWidget* parent, QString manga) :
    QWidget(parent),
    m_layout(new QHBoxLayout),
    m_label(nullptr),
    mangaName(manga)
{
    m_layout->setSpacing(0);
    setLayout(m_layout);

    QAction* leftAction = new QAction(this);
    QAction* rightAction = new QAction(this);
    QAction* nextAction = new QAction(this);
    enterReadingModeAction = new QAction(this);
    exitReadingModeAction = new QAction(this);

    addAction(leftAction);
    addAction(rightAction);
    addAction(nextAction);
    addAction(enterReadingModeAction);
    addAction(exitReadingModeAction);

    exitReadingModeAction->setEnabled(false);

    leftAction->setShortcut(Qt::Key_Left);
    rightAction->setShortcut(Qt::Key_Right);
    nextAction->setShortcut(Qt::Key_Space);
    enterReadingModeAction->setShortcut(Qt::Key_F5);
    exitReadingModeAction->setShortcuts({Qt::Key_Q,
                                         Qt::Key_F5,
                                         Qt::Key_Escape,
                                         Qt::Key_Backspace
                                        });

    connect(leftAction, SIGNAL(triggered()), this, SLOT(swipeLeft()));
    connect(rightAction, SIGNAL(triggered()), this, SLOT(swipeRight()));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(displayNextPages()));

    updateReadingDirection();

    QSettings settings;
    settings.beginGroup("reader/caches");
    doublePagesCache.setMaxCost(settings.value("doublePages", 1000).toInt());
    QPixmapCache::setCacheLimit(settings.value("Pixmap", 131072).toInt());
    settings.endGroup();
}

void Reader::setPagesDir(QDir value) {
    pagesDir = value;
    QStringList pagesListRelative = sorted(pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name));
    pagesList.clear();

    foreach (const QString& fname, pagesListRelative)
        pagesList.append(pagesDir.absoluteFilePath(fname));

    if (!isActive()) {
        m_label = new PixmapLabel;
        m_label->setAlignment(Qt::AlignHCenter);

        m_layout->addWidget(m_label);
    }

    // check if volume has already been opened
    QString key = pagesDir.absolutePath();

    if (doublePagesCache.contains(key))
        doublePages = *doublePagesCache.take(key);

    else {  // if not check that extensions are valid and compute double pages
        pagesList = PageGrouper::correctExtensions(pagesList);
        initDoublePages();
        doublePagesCache.insert(key, &doublePages, doublePages.size());
    }

    displayPages(0);
}

bool Reader::isActive() const {
    return (m_label != nullptr);
}

void Reader::initDoublePages() {
    doublePages = PageGrouper::groupPages(pagesList);

    currentDoublePageIndex = 0;
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
        currentDoublePageIndex = nDoublePages - 1;
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


// PRIVATE

QPixmap Reader::loadPage(const int index, const char* format, Qt::ImageConversionFlags flags) const {
    QString pagePath = pagesList.at(index);

    QPixmap page;

    if (!QPixmapCache::find(pagePath, &page)) {
        page.load(pagePath, format, flags);
        QPixmapCache::insert(pagePath, page);
    }

    return page;
}

void Reader::displayPages(const int index) {
    QList<int> currentDoublePage = doublePages.at(index);
    switch (currentDoublePage.size()) {
        case 1:
            m_label->updatePixmap(loadPage(currentDoublePage.at(0)));
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

            m_label->updatePixmap(collage);
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
