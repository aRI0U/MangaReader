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
                treeItemChild->setData(0, Qt::UserRole, chapterDir.absolutePath());
            }

            treeItem->setText(1, QString::number(chapterList.size()) + " chapters");
        }
        else {
            // Add pages directly
            QStringList pageList = volumeDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files);
            treeItem->setText(1, QString::number(pageList.size()) + " pages");
            treeItem->setForeground(0, QBrush(Qt::black));
            treeItem->setForeground(1, QBrush(Qt::darkGray));
            treeItem->setData(0, Qt::UserRole, volumeDir.absolutePath());
        }
    }

    // show widget
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(treeWidget);
    setLayout(layout);

    // connect click events on tree
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &MangaTab::openManga);
}

void MangaTab::openManga(QTreeWidgetItem* item, int column) {
    if(column == 0 && item->text(1).endsWith("pages")) {
        std::cout << "Reading manga chapter at " << item->data(0, Qt::UserRole).toString().toStdString() << std::endl;
        // todo prevent reading is chapter has 0 pages
        // (add boolean "readable" setData in column 1)

        QDir pagesDir(item->data(0, Qt::UserRole).toString());
        if(pagesDir.exists()) {
            QVector<QPixmap> pages;
            QStringList pageList = pagesDir.entryList(QStringList() << "*.png" << "*.jpg", QDir::Files, QDir::Name);
            for(QString& pageName: pageList) {
                QString pagePath = pagesDir.absoluteFilePath(pageName);
                QPixmap page(pagePath);
                page = page.scaledToHeight(600, Qt::SmoothTransformation); // todo compute size dynamically
                pages.push_back(page);
            }

            if(pages.size() >= 2) { // tmp hardcoded first 2 pages
                QHBoxLayout* layout = new QHBoxLayout;

                QLabel* left = new QLabel;
                left->setPixmap(pages[0]);
                QLabel* right = new QLabel;
                right->setPixmap(pages[1]);

                layout->addWidget(left);
                layout->addWidget(right);

                delete this->layout(); // todo widget needs to be removed from layout -> probably cleaner way
                setLayout(layout);
            }
        }
    }
}

