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
#include "utils/scandl/scansDownloaders.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}

public slots:
    void openLibrary();
    void openSettingsWindow();
    void showFullScreenOrMaximized(bool checked);
    void updateLibrary();

signals:
    void openLibraryRequest(QDir scansPath);

private slots:
    void closeTab(int index);
    void refreshLibrary();

private:
    void createCentralWidget();
    void createActions();
    void createStatusBar();

    void initializeNotificationsManager();
    void initializeDownloaders();

    void readSettings();
    void writeSettings();

    QTabWidget* centralTabs;
    MainTab* mainTab;

    NotificationLayout* notificationLayout;

    QList<AbstractScansDownloader *> m_scanDownloaders;

    SettingsWindow *settingsWindow;

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
