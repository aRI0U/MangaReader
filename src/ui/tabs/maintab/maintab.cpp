#include "maintab.h"
#include "mangaentry.h"
#include "mangalist.h"
#include "constants.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
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
    std::cout << "(Todo) Opening manga at " << mangaDir.absolutePath().toStdString() << std::endl;
}
