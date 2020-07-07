#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QGuiApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QScrollArea>
#include <QSettings>
#include <QWidget>

#include <iostream>

#include "constants.h"
#include "tabs/maintab/maintab.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {};

    void readSettings();
    void writeSettings();

public slots:
    void closeTab(int index);
    void openLibrary();
    void showFullScreenOrMaximized(bool checked);

private:
    QTabWidget* centralTabs;

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
