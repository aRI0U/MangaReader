#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QGuiApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QScrollArea>
#include <QWidget>

#include <QPushButton> // tmp

#include <iostream>

#include "tabs/maintab/maintab.h"
#include "tabs/mangatab/reader.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {};
    Reader* getReader();

public slots:
    void closeTab(int index);
    void openLibrary();
    void readingMode();
    void showFullScreenOrMaximized(bool checked);

private:
    QTabWidget* centralTabs;


};

#endif // MAINWINDOW_H
