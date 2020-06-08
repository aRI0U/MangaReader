#ifndef MANGATAB_H
#define MANGATAB_H

#include <QWidget>
#include <QDir>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>

#include <iostream>

class MangaTab : public QWidget
{
    Q_OBJECT

public:
    explicit MangaTab(QTabWidget *parent, QDir mangaDir);

private:
    QDir mangaDir;
    QTabWidget* parent;
};

#endif // MANGATAB_H
