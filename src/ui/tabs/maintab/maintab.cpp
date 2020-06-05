#include "maintab.h"
#include "mangalist.h"

#include <QPushButton>

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    MangaList *mangaList = new MangaList;

    mangaList->addWidget(new QPushButton("Short"));
    mangaList->addWidget(new QPushButton("Longer"));
    mangaList->addWidget(new QPushButton(tr("Different text")));
    mangaList->addWidget(new QPushButton(tr("More text")));
    mangaList->addWidget(new QPushButton(tr("Even longer button text")));
    setLayout(mangaList);
}
