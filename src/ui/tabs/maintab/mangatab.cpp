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
        treeItem->setForeground(0, QBrush(Qt::darkBlue));
        treeItem->setForeground(1, QBrush(Qt::blue));

        QDir volumeDir(mangaDir.absolutePath() + "/" + volStr);
        QStringList chapterList = volumeDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

        if(!chapterList.empty()) {
            // Add chapters then pages
            for(const QString& chapterStr: chapterList) {
                QTreeWidgetItem *treeItemChild = new QTreeWidgetItem();
                treeItemChild->setText(0, chapterStr);
                treeItemChild->setForeground(1, QBrush(Qt::darkGray));
                treeItem->addChild(treeItemChild);

                QDir chapterDir(volumeDir.absolutePath() + "/" + chapterStr);
                QStringList pageList = chapterDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);
                treeItemChild->setText(1, QString::number(pageList.size()) + " pages");
            }

            treeItem->setText(1, QString::number(chapterList.size()) + " chapters");
        }
        else {
            // Add pages directly
            QStringList pageList = volumeDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);
            treeItem->setText(1, QString::number(pageList.size()) + " pages");
            treeItem->setForeground(0, QBrush(Qt::black));
            treeItem->setForeground(1, QBrush(Qt::darkGray));
        }
    }

    // show widget
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(treeWidget);
    setLayout(layout);
}
