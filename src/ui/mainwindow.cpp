#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MangaReader");
    setMinimumSize(800, 600);
//    showMaximized();  // don't open maximized while debugging

    // Create central zone
    QTabWidget *centralTabs = new QTabWidget;
    centralTabs->setMovable(true);
    centralTabs->setTabsClosable(false);
    // maybe make a library a button and not a tab so it cant be moved/closed

    // Create main tab
    MainTab *mainTab = new MainTab(centralTabs);

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidget(mainTab);
    scrollArea->setWidgetResizable(true);

    centralTabs->addTab(scrollArea, tr("Library"));
    setCentralWidget(centralTabs);

    // Menu bar
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* openLibraryAction = fileMenu->addAction(tr("Open Library"));
    openLibraryAction->setShortcut(Qt::CTRL | Qt::Key_O);
    openLibraryAction->setStatusTip(tr("Open a library of mangas"));

    connect(openLibraryAction, SIGNAL(triggered()), this, SLOT(openLibrary()));
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
