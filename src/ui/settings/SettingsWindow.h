#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMenu>
#include <QScrollArea>

#include <QVBoxLayout>
#include <QWidget>

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);

signals:

private:
};

#endif // SETTINGSWINDOW_H
