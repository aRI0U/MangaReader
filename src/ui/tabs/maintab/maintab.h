#ifndef MAINTAB_H
#define MAINTAB_H

#include <QDir>
#include <QWidget>
#include <QTabWidget>

#include "../mangatab/mangatab.h"

class MainTab : public QWidget
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
