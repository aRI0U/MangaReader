#include "maintab.h"
#include "mangaentry.h"
#include "mangalist.h"
#include "constants.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    MangaList *mangaList = new MangaList;
    for (const auto &dir : constants::ScansPath.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        mangaList->addWidget(new MangaEntry(QDir(constants::ScansPath.filePath(dir))));
    }

    setStyleSheet("background-color: lightblue;");
    setLayout(mangaList);
}
