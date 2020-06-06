#include "maintab.h"
#include "mangaentry.h"
#include "mangalist.h"
#include "constants.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    MangaList *mangaList = new MangaList;
    for (const auto &path : constants::ScansPath.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        mangaList->addWidget(new MangaEntry(path));
    setLayout(mangaList);
}
