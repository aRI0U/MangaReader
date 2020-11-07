#ifndef MAINTAB_H
#define MAINTAB_H

#include <QDir>
#include <QWidget>
#include <QTabWidget>

#include "MangaEntry.h"
#include "MangaList.h"
#include "../Tab.h"
#include "../mangatab/MangaTab.h"
#include "../mangatab/Reader.h"

class MainTab : public Tab
{
    Q_OBJECT
public:
    explicit MainTab(QTabWidget* parent = nullptr);

public slots:
    void clickedManga(QDir mangaDir);
    void openLibrary(QDir scansPath);
    void refreshLibrary(const QDir &scansPath);

signals:
    void reportLibraryOpen(QString message);

private:
    QTabWidget *parent;
    MangaList *m_mangaList;
    QStringList m_foldersList;
};

#endif // MAINTAB_H
