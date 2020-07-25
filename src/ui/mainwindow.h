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
#include <QStatusBar>
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

public slots:
    void closeTab(int index);
    void openLibrary();
    void showFullScreenOrMaximized(bool checked);

signals:
    void openLibraryRequest(QDir scansPath);

private:
    void createCentralWidget();
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    QTabWidget* centralTabs;
    MainTab* mainTab;

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
