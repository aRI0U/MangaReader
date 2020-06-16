#ifndef MANGATAB_H
#define MANGATAB_H

#include <QAction>
#include <QObject>
#include <QWidget>
#include <QDir>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>
#include <QPixmap>
#include <QLabel>

#include <iostream>

#include "../tab.h"
#include "reader.h"

class MangaTab : public Tab
{
    Q_OBJECT

public:
    explicit MangaTab(QTabWidget *parent, QDir mangaDir);
    Reader* getReader() const override;

private:
    void openManga(QTreeWidgetItem *item, int column);

    QDir mangaDir;
    QTabWidget* parent;
    QHBoxLayout* pageLayout;
    QTreeWidget* treeWidget;

    Reader* reader;
};

#endif // MANGATAB_H
