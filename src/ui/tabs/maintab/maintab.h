#ifndef MAINTAB_H
#define MAINTAB_H

#include <QDir>
#include <QWidget>
#include <QTabWidget>

#include "mangaentry.h"
#include "mangalist.h"
#include "../tab.h"
#include "../mangatab/mangatab.h"
#include "../mangatab/reader.h"

class MainTab : public Tab
{
    Q_OBJECT
public:
    explicit MainTab(QTabWidget *parent = nullptr, QDir scansPath = QDir());

public slots:
    void clickedManga(QDir mangaDir);
    void openLibrary(QDir scansPath);

private:
    QTabWidget* parent;
};

#endif // MAINTAB_H
