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
    void openLibrary();
};

#endif // MAINWINDOW_H
