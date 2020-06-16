#ifndef MAINTAB_H
#define MAINTAB_H

#include <QDir>
#include <QWidget>
#include <QTabWidget>

#include "../tab.h"
#include "../mangatab/mangatab.h"
#include "../mangatab/reader.h"

class MainTab : public Tab
{
    Q_OBJECT
public:
    explicit MainTab(QTabWidget *parent = nullptr);

    Reader* getReader() const override;

public slots:
    void clickedManga(QDir mangaDir);

private:
    QTabWidget* parent;
};

#endif // MAINTAB_H
