#include "MainTab.h"

MainTab::MainTab(QTabWidget* parent) :
    parent(parent),
    m_mangaList(new MangaList(this))
{
    setLayout(m_mangaList);
}

void MainTab::openLibrary(const QDir scansPath) {
    m_mangaList->clean();
    m_foldersList = QStringList();
    refreshLibrary(scansPath);

    setStyleSheet("background-color: lightblue;");
    emit reportLibraryOpen(tr("Successfully opened library located at ") + scansPath.absolutePath());
//    emit libraryOpen(scansPath.absolutePath());
}

void MainTab::refreshLibrary(const QDir &scansPath) {
    for (const QString &dir : scansPath.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (m_foldersList.contains(dir))
            continue;
        m_foldersList.append(dir);
        MangaEntry *mangaEntry = new MangaEntry(QDir(scansPath.filePath(dir)));
        m_mangaList->addWidget(mangaEntry);
        connect(mangaEntry, SIGNAL(clicked(QDir)),
                this, SLOT(clickedManga(QDir)));
    }
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
