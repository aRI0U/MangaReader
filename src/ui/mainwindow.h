#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include <iostream>

#include "tabs/maintab/maintab.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}

public slots:
    void closeTab(int index);
    void openLibrary();

private:
    QTabWidget *centralTabs;
};

#endif // MAINWINDOW_H
