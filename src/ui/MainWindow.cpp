#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_versionChecker(new VersionChecker(this))
{
    setWindowTitle(constants::applicationName + " v" + constants::currentVersion);
    setMinimumSize(constants::mainWindowMinimumSize);

    createStatusBar();
    createCentralWidget();
    createActions();

    initializeNotificationsManager();
    initializeDownloaders();

    readSettings();

    updateLibrary();

    connect(m_versionChecker, &VersionChecker::existsNewVersion,
            this, &MainWindow::newVersionWarning);
    m_versionChecker->checkVersion();
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

    connect(centralTabs, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
    connect(mainTab, SIGNAL(reportLibraryOpen(QString)),
            statusBar(), SLOT(showMessage(QString)));

    QVariant scansPath = settings.value("Library/scansPath");
    if (scansPath.isNull())
        openLibrary();
    else
        mainTab->openLibrary(QDir(scansPath.toString()));
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* openLibraryAction = fileMenu->addAction(tr("&Open..."));
    openLibraryAction->setShortcut(Qt::CTRL | Qt::Key_O);
    openLibraryAction->setStatusTip(tr("Open a library of mangas"));
    connect(openLibraryAction, SIGNAL(triggered()),
            this, SLOT(openLibrary()));
    connect(this, SIGNAL(openLibraryRequest(QDir)),
            mainTab, SLOT(openLibrary(QDir)));
    // quit
    QAction* quitAction = fileMenu->addAction(tr("&Quit"));
    quitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    quitAction->setStatusTip(tr("Quit the application"));
    connect(quitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    // settings
    QAction *settingsAction = editMenu->addAction(tr("&Settings"));
    settingsAction->setShortcut(Qt::CTRL | Qt::ALT | Qt::Key_S);
    settingsAction->setStatusTip("Open the settings");
    connect(settingsAction, &QAction::triggered,
            this, &MainWindow::openSettingsWindow);

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

    // enable fullscreen
    QAction* fullScreenAction = viewMenu->addAction(tr("Full Screen"));
    fullScreenAction->setCheckable(true);
    fullScreenAction->setShortcut(QKeySequence::FullScreen);
    connect(fullScreenAction, SIGNAL(triggered(bool)),
            this, SLOT(showFullScreenOrMaximized(bool)));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("");
}

void MainWindow::initializeNotificationsManager() {
    notificationLayout = new NotificationLayout();
    notifications.init(this, notificationLayout);
}

void MainWindow::initializeDownloaders() {
    DatabaseConnection *db = new DatabaseConnection(this);
    m_scanDownloaders.clear();
    m_scanDownloaders << new ScantradDownloader(db, this);
//    m_scanDownloaders << new LelscanDownloader(db, this);

    db->restoreChaptersList();

    // TODO: refresh library after downloading
    QSettings settings;
    bool autoDownload = settings.value("Download/autoDownload", false).toBool();

    for (AbstractScansDownloader *downloader : m_scanDownloaders) {
        connect(downloader, &AbstractScansDownloader::chapterDownloaded,
                this, &MainWindow::refreshLibrary);
        if (autoDownload)
            connect(downloader, &AbstractScansDownloader::databaseUpdated,
                    downloader, &AbstractScansDownloader::downloadNewChapters);
    }
}


// SLOTS

void MainWindow::closeTab(int index) {
    if (index > 0)
        centralTabs->removeTab(index);
}

void MainWindow::refreshLibrary(Chapter &chapter) {
    notifications.send(chapter.manga + tr(": New chapter"), QString("#%1 : %2").arg(chapter.number).arg(chapter.name));
    QSettings settings;
    // eventually open library
    QString scansPath = settings.value("Library/scansPath").toString();
    mainTab->refreshLibrary(QDir(scansPath));
}

void MainWindow::newVersionWarning(QString version) {
    QMessageBox::information(this,
                             tr("New version available"),
                             tr("You are currently using MangaReader version %1 but version %2 is available. "
                                "Use the maintenance tool to download it.").arg(constants::currentVersion).arg(version));
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

void MainWindow::openSettingsWindow() {
    settingsWindow = new SettingsWindow(this);
    settingsWindow->show();
}

void MainWindow::showFullScreenOrMaximized(bool checked) {
    if (checked)
        showFullScreen();
    else
        showMaximized();
}

void MainWindow::updateLibrary() {
    QSettings settings;

    if (!settings.value("Download/autoCheck", false).toBool())
        return;

    for (AbstractScansDownloader *downloader : m_scanDownloaders) {
        downloader->lookForNewChapters();
    };
}

void MainWindow::readSettings() {
    QSettings settings;

    settings.beginGroup("MainWindow");
    // resize
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    settings.endGroup();
}

void MainWindow::writeSettings() {
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();
    event->accept();
}
