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
    void openLibrary(QDir scansPath);
    void clickedManga(QDir mangaDir);

signals:
    void reportLibraryOpen(QString message);

private:
    QTabWidget* parent;
};

#endif // MAINTAB_H
