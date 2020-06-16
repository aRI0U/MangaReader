#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("MangaReader");
    setMinimumSize(800, 600);
    showMaximized();
//    setFocusPolicy(Qt::StrongFocus);

    // Create central zone
    centralTabs = new QTabWidget;
    centralTabs->setMovable(true);
    centralTabs->setTabsClosable(false);
    // maybe make a library a button and not a tab so it cant be moved/closed
    // TODO library should not be neither movable nor closable

    // Create main tab
    MainTab *mainTab = new MainTab(centralTabs);

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidget(mainTab);
    scrollArea->setWidgetResizable(true);
    centralTabs->addTab(scrollArea, tr("Library"));
    setCentralWidget(centralTabs);

    connect(centralTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // Menu bar

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* openLibraryAction = fileMenu->addAction(tr("Open Library"));
    openLibraryAction->setShortcut(Qt::CTRL | Qt::Key_O);
    openLibraryAction->setStatusTip(tr("Open a library of mangas"));
    connect(openLibraryAction, SIGNAL(triggered()), this, SLOT(openLibrary()));
    // quit
    QAction* quitAction = fileMenu->addAction(tr("Quit"));
    quitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    quitAction->setStatusTip(tr("Quit the application"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));


    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));


    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    // reading mode
    QAction* readingModeAction = viewMenu->addAction(tr("ReadingMode"));
    readingModeAction->setShortcut(Qt::Key_A); // TODO change for F5 or sth
    connect(readingModeAction, SIGNAL(triggered()), this, SLOT(readingMode()));

    // enable fullscreen
    QAction* fullScreenAction = viewMenu->addAction(tr("Full Screen"));
    fullScreenAction->setCheckable(true);
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    connect(fullScreenAction, SIGNAL(triggered(bool)), this, SLOT(showFullScreenOrMaximized(bool)));
}

// SLOTS

void MainWindow::closeTab(int index) {
    if (index > 0)
        centralTabs->removeTab(index);
}

void MainWindow::openLibrary() {
    // Let user select a folder
    QString folderPath = QFileDialog::getExistingDirectory(
        this, tr("Open Library"), QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir folder(folderPath);
    if (folder.exists()) {
        std::cout << "Todo: try to open library located at " << folderPath.toStdString() << std::endl;
        // todo
    }
}

void MainWindow::readingMode() {
    QWidget* w = new QWidget(this, Qt::Window);
    w->setStyleSheet("background-color:black;");
    w->setFocusPolicy(Qt::StrongFocus);

    // todo add reader in it
    QHBoxLayout* l = new QHBoxLayout;
    w->setLayout(l);
    // TODO avoid reading mode when no reading (current solution sucks)
    // TODO solve bug of not displaying after first fullscreen
    Tab* currentTab = static_cast<Tab*>(centralTabs->currentWidget());
    Reader* reader = currentTab->getReader();

    if ( reader->isActive() ) {
        std::cout << reader << std::endl;
        l->addWidget(reader);

        // actions
        QAction* closeAction = new QAction(tr("Close"));
        closeAction->setShortcuts({Qt::Key_Q, Qt::Key_Z}); // TODO choose appropriate shortcuts
        w->addAction(closeAction);
        connect(closeAction, SIGNAL(triggered()), w, SLOT(close()));

        // display
//        w->show(); // easier for debugging
        w->showFullScreen(); // TODO adapt to actual screen's size
    } else {
        std::cout << "no active reader" << std::endl;
    }
}

void MainWindow::showFullScreenOrMaximized(bool checked) {
    if (checked)
        showFullScreen();
    else
        showMaximized();
}
