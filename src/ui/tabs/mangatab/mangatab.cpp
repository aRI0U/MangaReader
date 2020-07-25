#include "mangatab.h"

MangaTab::MangaTab(QTabWidget* parent, QDir mangaDir) :
    parent(parent),
    mangaDir(mangaDir),
    pageLayout(new QHBoxLayout),
    reader(nullptr)
{
    treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(2);
    treeWidget->setHeaderLabels(QStringList() << "Title" << "Number of chapters/pages");

    treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Add volumes/tomes
    QStringList volumeList = Reader::sorted(mangaDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));
    for(const QString& volStr: volumeList) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(treeWidget);
        treeItem->setText(0, volStr);
        treeItem->setForeground(0, QBrush(Qt::darkBlue));
        treeItem->setForeground(1, QBrush(Qt::blue));

        QDir volumeDir(mangaDir.absoluteFilePath(volStr));
        QStringList chapterList = Reader::sorted(volumeDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

        if(!chapterList.empty()) {
            // Add chapters then pages
            for(const QString& chapterStr: chapterList) {
                QTreeWidgetItem* treeItemChild = new QTreeWidgetItem(treeItem);
                treeItemChild->setText(0, chapterStr);
                treeItemChild->setForeground(1, QBrush(Qt::darkGray));
//                treeItem->addChild(treeItemChild);

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

    // reader
    reader = new Reader(this, mangaDir.dirName());

    // show widget
    pageLayout->addWidget(treeWidget);
    pageLayout->addWidget(reader);
    setLayout(pageLayout);

    // connect click events on tree
    connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(openChapter(QTreeWidgetItem*,int)));
    connect(reader, SIGNAL(endOfChapter()), this, SLOT(openNextChapter()));

    // connect reader's actions to enter/exit reading mode
    connect(reader->enterReadingModeAction, SIGNAL(triggered()), this, SLOT(enterReadingMode()));
    connect(reader->exitReadingModeAction, SIGNAL(triggered()), this, SLOT(exitReadingMode()));
}


QString MangaTab::mangaName() {
    return mangaDir.dirName();
}

// SLOTS

void MangaTab::enterReadingMode() {
    reader->enterReadingMode();
}

void MangaTab::exitReadingMode() {
    reader->exitReadingMode();
    pageLayout->addWidget(reader);
}

void MangaTab::openChapter(QTreeWidgetItem* item, int column) {
    if (column == 0 && item->text(1).endsWith("pages")) {
        // update the current chapter index
        currentChapterItem = item;

        // open chapter
        std::cout << "Reading manga chapter at " << item->data(0, Qt::UserRole).toString().toStdString() << std::endl;
        // todo prevent reading is chapter has 0 pages
        // (add boolean "readable" setData in column 1)

        QDir pagesDir(item->data(0, Qt::UserRole).toString());
        if(pagesDir.exists()) {
            treeWidget->setColumnCount(1);
            reader->setPagesDir(pagesDir);
        }
    }
}

void MangaTab::openNextChapter() {
    QTreeWidgetItem* parent = currentChapterItem->parent();
    if (parent != nullptr) {
        int index = parent->indexOfChild(currentChapterItem);
        if (++index < parent->childCount()) {
            QTreeWidgetItem* nextChapterItem = parent->child(index);
            currentChapterItem->setSelected(false);
            nextChapterItem->setSelected(true);
            openChapter(nextChapterItem, 0);
        }
    }
}
