#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QWidget>

#include "NotificationLayout.h"

class NotificationManager : public QObject
{
    Q_OBJECT
public:
    explicit NotificationManager(QObject *parent = nullptr);

    void init(QWidget* w, NotificationLayout* l);

    static NotificationManager& instance();

public slots:
    void send(const QString title, const QString description);

private:
    NotificationLayout* layout;
    QWidget* widget;
};

namespace {
    NotificationManager& notifications = NotificationManager::instance();
}

#endif // NOTIFICATIONMANAGER_H
