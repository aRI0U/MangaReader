#include "MainTab.h"

MainTab::MainTab(QTabWidget* parent) :
    parent(parent)
{
}

void MainTab::openLibrary(const QDir scansPath) {
    MangaList *mangaList = new MangaList;
    for (const auto &dir : scansPath.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        MangaEntry *mangaEntry = new MangaEntry(QDir(scansPath.filePath(dir)));
        mangaList->addWidget(mangaEntry);
        connect(mangaEntry, SIGNAL(clicked(QDir)),
                this, SLOT(clickedManga(QDir)));
    }

    setStyleSheet("background-color: lightblue;");
    setLayout(mangaList);
    emit reportLibraryOpen(tr("Successfully opened library located at ") + scansPath.absolutePath());
//    emit libraryOpen(scansPath.absolutePath());
}

void MainTab::clickedManga(QDir mangaDir)
{
    std::cout << "Opening manga at " << mangaDir.absolutePath().toStdString() << std::endl;

    // If manga is already open, set focus on the concerned tab
    for (int k = 0; k < parent->count(); ++k) {
        if (parent->tabText(k) == mangaDir.dirName()) {
            parent->setCurrentIndex(k);
            return;
        }
    }

    // Otherwise, open a new tab
    MangaTab* manga_tab = new MangaTab(parent, mangaDir);
    parent->addTab(manga_tab, mangaDir.dirName());
    parent->setCurrentIndex(parent->count() - 1);
}
