#include "maintab.h"
#include "mangalist.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    MangaList *mangaList = new MangaList(this);
}
