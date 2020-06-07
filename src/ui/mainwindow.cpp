#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MangaReader");
    setMinimumSize(800, 600);
//    showMaximized();  // don't open maximized while debugging

    // Create central zone

    // Create main tab
    MainTab *mainTab = new MainTab; // todo center the content and add min size

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidget(mainTab);
    scrollArea->setWidgetResizable(true);

// tmp dont display tabs
//    QTabWidget *centralTabs = new QTabWidget;
//    setCentralWidget(centralTabs);
//    centralTabs->addTab(mainTab, tr("Library"));

    setCentralWidget(scrollArea); //(mainTab);

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

    if(!folderPath.isEmpty()) {
        std::cout << "Todo: try to open library located at " << folderPath.toUtf8().constData() << std::endl;
        // todo
    }
}
