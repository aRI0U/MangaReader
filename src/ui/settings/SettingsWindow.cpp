#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setWindowTitle(tr("Settings"));

    QLayout *layout = new QVBoxLayout(this);

    QScrollArea *scrollArea = new QScrollArea();
    QMenu *menu = new QMenu();

    menu->addAction(new QAction(tr("Language")));


    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));
    menu->addAction(new QAction("test"));

    scrollArea->setWidget(menu);

    layout->addWidget(scrollArea);
}
