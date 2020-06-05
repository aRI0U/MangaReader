#include "mainwindow.h"
#include "tabs/maintab/maintab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MangaReader");

    // Create central zone
    QTabWidget *centralTabs = new QTabWidget;
    setCentralWidget(centralTabs);

    // Create main tab
    MainTab *mainTab = new MainTab;

    // Fill central zone
    centralTabs->addTab(mainTab, tr("Library"));
}

MainWindow::~MainWindow()
{
}

