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
#include "tabs/maintab/MainTab.h"
#include "settings/SettingsWindow.h"

#include "utils/notifications/NotificationLayout.h"
#include "utils/notifications/NotificationManager.h"

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
    void openSettingsWindow();

signals:
    void openLibraryRequest(QDir scansPath);

private:
    void createCentralWidget();
    void createActions();
    void createStatusBar();
    void initializeNotificationsManager();
    void readSettings();
    void writeSettings();

    QTabWidget* centralTabs;
    MainTab* mainTab;

    NotificationLayout* notificationLayout;

    SettingsWindow *settingsWindow;

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
