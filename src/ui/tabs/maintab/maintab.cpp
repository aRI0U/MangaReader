#include "maintab.h"
#include "mangaentry.h"
#include "mangalist.h"
#include "constants.h"

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

//int main() {
//    std::string path = "/path/to/directory";
//    for (const auto & entry : fs::directory_iterator(path))
//        std::cout << entry.path() << std::endl;
//}

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    MangaList *mangaList = new MangaList;
    for (const auto &entry : fs::directory_iterator(constants::ScansPath)) {
        std::string path = entry.path().string();
        mangaList->addWidget(new MangaEntry(QString::fromStdString(path)));
    }
    setLayout(mangaList);
}
