#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setWindowTitle(tr("Settings"));
    qDebug() << "opening settings window" << windowFlags();
    qDebug() << isVisible();
}
