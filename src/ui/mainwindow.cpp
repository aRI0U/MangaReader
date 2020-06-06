#include "mainwindow.h"
#include "tabs/maintab/maintab.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MangaReader");

    // Create central zone

    // Create main tab
    MainTab *mainTab = new MainTab;
    mainTab->setStyleSheet("background-color:lightblue;");

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidget(mainTab);
    scrollArea->setWidgetResizable(true);

// tmp dont display tabs
//    QTabWidget *centralTabs = new QTabWidget;
//    setCentralWidget(centralTabs);
//    centralTabs->addTab(mainTab, tr("Library"));

    setCentralWidget(scrollArea); //(mainTab);

}

MainWindow::~MainWindow()
{
}

