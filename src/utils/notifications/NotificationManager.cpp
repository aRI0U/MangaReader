#include "NotificationManager.h"

NotificationManager::NotificationManager(QObject *parent) : QObject(parent)
{

}

void NotificationManager::init(QWidget* w, NotificationLayout* l) {
    widget = w;
    layout = l;
}

void NotificationManager::send(const QString title, const QString description) {
    layout->addNotificationWidget(widget, title, description);
}

NotificationManager& NotificationManager::instance() {
    static NotificationManager manager;
    return manager;
}
