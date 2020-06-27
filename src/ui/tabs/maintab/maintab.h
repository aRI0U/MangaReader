#ifndef MAINTAB_H
#define MAINTAB_H

#include <QDir>
#include <QWidget>
#include <QTabWidget>

#include "mangaentry.h"
#include "mangalist.h"
#include "config.h"
#include "../tab.h"
#include "../mangatab/mangatab.h"
#include "../mangatab/reader.h"

class MainTab : public Tab
{
    Q_OBJECT
public:
    explicit MainTab(QTabWidget *parent = nullptr);

public slots:
    void clickedManga(QDir mangaDir);

private:
    QTabWidget* parent;
};

#endif // MAINTAB_H
