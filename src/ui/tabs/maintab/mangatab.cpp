#include "mangatab.h"

MangaTab::MangaTab(QTabWidget *parent, QDir mangaDir) :
    mangaDir(mangaDir),
    parent(parent)
{
    QTreeWidget *treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(2);
    treeWidget->setHeaderLabels(QStringList() << "Title" << "Number of chapters/pages");

    treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Add volumes/tomes
    QStringList volumeList = mangaDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(const QString& volStr: volumeList) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(treeWidget);
        treeItem->setText(0, volStr);

        // Add chapters
        QDir volumeDir(mangaDir.absolutePath() + "/" + volStr);
        QStringList chapterList = volumeDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        for(const QString& chapterStr: chapterList) {
            QTreeWidgetItem *treeItemChild = new QTreeWidgetItem();
            treeItemChild->setText(0, chapterStr);
            treeItem->addChild(treeItemChild);

            QDir chapterDir(volumeDir.absolutePath() + "/" + chapterStr);
            QStringList pageList = chapterDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);
            treeItemChild->setText(1, QString::number(pageList.size()) + " pages");
        }

        treeItem->setText(1, QString::number(chapterList.size()) + " chapters");
    }

    // show widget
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(treeWidget);
    setLayout(layout);
}


//    // todo not adaptative
//    QStringList volumeList = mangaDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
//    for(const QString& volStr: volumeList) {
//        std::cout << "\t" << volStr.toStdString() << std::endl;

//        QDir volumeDir(mangaDir.absolutePath() + "/" + volStr);
//        QStringList chapterList = volumeDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
//        for(const QString& chapterStr: chapterList) {
//            std::cout << "\t\t" << chapterStr.toStdString() << std::endl;

//        }
//    }
