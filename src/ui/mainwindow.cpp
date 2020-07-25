#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("MangaReader");
    setMinimumSize(800, 600);
//    setFocusPolicy(Qt::StrongFocus);

//    createStatusBar();
    createCentralWidget();
    createActions();

    readSettings();
}

void MainWindow::createCentralWidget() {
    QSettings settings;

    // Create central zone
    centralTabs = new QTabWidget;
    centralTabs->setMovable(true);
    centralTabs->setTabsClosable(false);
    // maybe make a library a button and not a tab so it cant be moved/closed
    // TODO library should not be neither movable nor closable

    // Create main tab
    mainTab = new MainTab(centralTabs);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(mainTab);
    scrollArea->setWidgetResizable(true);
    centralTabs->addTab(scrollArea, tr("Library"));
    setCentralWidget(centralTabs);

    connect(centralTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(mainTab, SIGNAL(reportLibraryOpen(QString)), statusBar(), SLOT(showMessage(QString)));

    // eventually open library
    QVariant scansPath = settings.value("Library/scansPath");
    if (!scansPath.isNull())
        mainTab->openLibrary(QDir(scansPath.toString()));
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* openLibraryAction = fileMenu->addAction(tr("Open Library"));
    openLibraryAction->setShortcut(Qt::CTRL | Qt::Key_O);
    openLibraryAction->setStatusTip(tr("Open a library of mangas"));
    connect(openLibraryAction, SIGNAL(triggered()), this, SLOT(openLibrary()));
    connect(this, SIGNAL(openLibraryRequest(QDir)), mainTab, SLOT(openLibrary(QDir)));
    // quit
    QAction* quitAction = fileMenu->addAction(tr("Quit"));
    quitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    quitAction->setStatusTip(tr("Quit the application"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));


    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));


    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

    // enable fullscreen
    QAction* fullScreenAction = viewMenu->addAction(tr("Full Screen"));
    fullScreenAction->setCheckable(true);
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    connect(fullScreenAction, SIGNAL(triggered(bool)), this, SLOT(showFullScreenOrMaximized(bool)));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("");
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
        QSettings settings;
        settings.setValue("Library/scansPath", folderPath);
        emit openLibraryRequest(folder);
    }
}

void MainWindow::showFullScreenOrMaximized(bool checked) {
    if (checked)
        showFullScreen();
    else
        showMaximized();
}

void MainWindow::readSettings() {
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", minimumSize()).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    settings.endGroup();
}

void MainWindow::writeSettings() {
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();
    event->accept();
}
