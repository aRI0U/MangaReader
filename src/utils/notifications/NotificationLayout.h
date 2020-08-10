#ifndef NOTIFICATIONLAYOUT_H
#define NOTIFICATIONLAYOUT_H

#include <QBasicTimer>
#include <QElapsedTimer>
#include <QMap>
#include <QObject>

#include "NotificationWidget.h"

class NotificationLayout : public QObject
{
    Q_OBJECT

public:
    NotificationLayout();
    ~NotificationLayout() override;

    void addNotificationWidget(QWidget* parent, const QString title, const QString message);
    void SetLayoutType(Qt::Alignment align);
    void SetDisplayTimeMs(int displayTimeMS);

private slots:
    void onCloseClicked(NotificationWidget* notification);
    void onWidgetDestroyed();
    void onParentWidgetDestroyed();

private:
    void layoutWidgets(QWidget* parent);

    bool eventFilter(QObject* object, QEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

    struct NotificationWidgetParams;
    using NotificationPair = std::pair<NotificationWidget*, NotificationWidgetParams>;
    using WindowNotifications = std::vector<NotificationPair>;
    using AllNotifications = QMap<QWidget*, WindowNotifications>;
    AllNotifications notifications;

    Qt::Alignment layoutType = Qt::AlignTop | Qt::AlignRight;

    int displayTimeMs = 10000;
    const int maximumDisplayCount = 5;
    QElapsedTimer elapsedTimer;
    QBasicTimer basicTimer;
};

#endif // NOTIFICATIONLAYOUT_H
