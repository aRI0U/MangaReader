#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>

#include "constants.h"

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
    void setAutomaticDownloadState(bool checked);
    void setLanguage(int index);

private:
    QLayout *layout;

    // language settings
    QComboBox *languageChoices;
};

#endif // SETTINGSWINDOW_H
