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
    centralTabs->setTabsClosable(true);
    // maybe make a library a button and not a tab so it cant be moved/closed

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
    if(folder.exists()) {
        std::cout << "Todo: try to open library located at " << folderPath.toStdString() << std::endl;
        // todo
    }
}

void MainWindow::readingMode() {
    QWindow* w = new QWindow();
//    w->showFullScreen();
}

void MainWindow::showFullScreenOrMaximized(bool checked) {
    if (checked)
        showFullScreen();
    else
        showMaximized();
}
