#include "maintab.h"
#include "mangaentry.h"
#include "mangalist.h"
#include "constants.h"

MainTab::MainTab(QTabWidget *parent) :
    parent(parent)
{
    MangaList *mangaList = new MangaList;
    for (const auto &dir : constants::ScansPath.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        MangaEntry *mangaEntry = new MangaEntry(QDir(constants::ScansPath.filePath(dir)));
        mangaList->addWidget(mangaEntry);
        QObject::connect(mangaEntry, SIGNAL(clicked(QDir)), this, SLOT(clickedManga(QDir)));
    }

    setStyleSheet("background-color: lightblue;");
    setLayout(mangaList);
}

void MainTab::clickedManga(QDir mangaDir)
{
    std::cout << "Opening manga at " << mangaDir.absolutePath().toStdString() << std::endl;

    // If manga is already open, set focus on the concerned tab
    for(int k = 0; k < parent->count(); ++k) {
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

Reader* MainTab::getReader() const {
    std::cout << "no reader" << std::endl;
    return nullptr;
}
