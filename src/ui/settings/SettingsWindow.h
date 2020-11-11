#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

#include "constants.h"
#include "libs/QPath/QPath.h"

#include "MangaSqlModel.h"

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);

public slots:
    void openDownloadSettings();
    void openLanguageSettings();

signals:

private slots:
    void setAutomaticCheckState(bool checked);
    void setAutomaticDownloadState(bool checked);
    void setLanguage(int index);

private:
    QLayout *layout;

    // language settings
    QComboBox *languageChoices;
};

#endif // SETTINGSWINDOW_H
