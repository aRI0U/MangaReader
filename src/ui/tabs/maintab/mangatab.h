#ifndef MANGATAB_H
#define MANGATAB_H

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

class MangaTab : public QWidget
{
    Q_OBJECT

public:
    explicit MangaTab(QTabWidget *parent, QDir mangaDir);

private:
    void openManga(QTreeWidgetItem *item, int column);

    QDir mangaDir;
    QTabWidget* parent;
    QHBoxLayout* pageLayout;
    QTreeWidget* treeWidget;
    QLabel* leftImg;
    QLabel* rightImg;
};

#endif // MANGATAB_H
