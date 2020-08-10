#include "NotificationLayout.h"

struct NotificationLayout::NotificationWidgetParams
{
    void InitAnimation(NotificationWidget* target)
    {
        positionAnimation = new QPropertyAnimation(target, "position", target);
        const int durationTimeMs = 120;
        positionAnimation->setDuration(durationTimeMs);

        opacityAnimation = new QPropertyAnimation(target, "opacity", target);
        opacityAnimation->setDuration(durationTimeMs * 2);
    }

    void DecrementTime(int elapsedMs)
    {
        if (elapsedMs > remainTimeMs)
            remainTimeMs = 0;
        else
            remainTimeMs -= elapsedMs;
    }

    int remainTimeMs = 0;
    QPropertyAnimation* positionAnimation = nullptr;
    QPropertyAnimation* opacityAnimation = nullptr;
};

NotificationLayout::NotificationLayout()
    : QObject(nullptr)
{
    basicTimer.start(60, this);
}

NotificationLayout::~NotificationLayout()
{
}

void NotificationLayout::addNotificationWidget(QWidget* parent, const QString title, const QString message)
{
    if (!notifications.contains(parent))
    {
        parent->installEventFilter(this);
        connect(parent, &QObject::destroyed,
                this, &NotificationLayout::onParentWidgetDestroyed);
    }

    NotificationWidget* widget = new NotificationWidget(parent, title, message);
    connect(widget, &NotificationWidget::CloseButtonClicked,
            this, &NotificationLayout::onCloseClicked);
    connect(widget, &QObject::destroyed, this,
            &NotificationLayout::onWidgetDestroyed);

    NotificationWidgetParams widgetParams;
    widgetParams.InitAnimation(widget);

    widgetParams.remainTimeMs = displayTimeMs;

    notifications[parent].emplace_back(widget, widgetParams);

    layoutWidgets(parent);
}

void NotificationLayout::layoutWidgets(QWidget* parent)
{
    int totalHeight = 0;
    Q_ASSERT(notifications.contains(parent));
    WindowNotifications& widgets = notifications[parent];

    int size = std::min(static_cast<int>(widgets.size()), maximumDisplayCount);
    std::vector<NotificationPair> widgetsToDisplay(widgets.begin(), widgets.begin() + size);
    if (layoutType & Qt::AlignTop)
    {
        std::reverse(widgetsToDisplay.begin(), widgetsToDisplay.end());
    }

    for (const NotificationPair& pair : widgetsToDisplay)
    {
        NotificationWidget* widget = pair.first;
        bool justCreated = false;
        if (widget->isVisible() == false)
        {
            justCreated = true;
            QPropertyAnimation* opacityAnimation = pair.second.opacityAnimation;
            widget->show();
            opacityAnimation->setStartValue(0.0f);
            opacityAnimation->setEndValue(1.0f);
            opacityAnimation->start();
        }

        int x = (layoutType & Qt::AlignLeft) ? 0 : (parent->width() - widget->width());
        int y = (layoutType & Qt::AlignTop) ? totalHeight : (parent->height() - widget->height() - totalHeight);
        QPoint widgetPos(x, y);

        //noticationWidget marked as window inside Qt, in this case we need to use global coordinates
        //if not mark it as window - on OS X notification will be behind from RenderWidget
        widgetPos = parent->mapToGlobal(widgetPos);

        if (justCreated)
        {
            widget->move(widgetPos);
        }
        else
        {
            QPropertyAnimation* positionAnimation = pair.second.positionAnimation;
            positionAnimation->stop();
            positionAnimation->setStartValue(widget->pos());
            positionAnimation->setEndValue(widgetPos);
            positionAnimation->start();
        }

        totalHeight += widget->size().height();
    }
}

bool NotificationLayout::eventFilter(QObject* object, QEvent* event)
{
    QEvent::Type type = event->type();
    QWidget* sender = qobject_cast<QWidget*>(object);
    if (type == QEvent::Resize || type == QEvent::Move)
    {
        layoutWidgets(sender);
    }
    return QObject::eventFilter(object, event);
}

void NotificationLayout::timerEvent(QTimerEvent* /*event*/)
{
    int elapsedMs = elapsedTimer.restart();
    for (AllNotifications::Iterator iter = notifications.begin(); iter != notifications.end(); ++iter)
    {
        QWidget* parent = iter.key();
        if (parent->isActiveWindow())
        {
            WindowNotifications& widgets = iter.value();
            for (WindowNotifications::iterator iter = widgets.begin(); iter != widgets.end();)
            {
                NotificationWidget* widget = iter->first;
                NotificationWidgetParams& params = iter->second;
                params.DecrementTime(elapsedMs);
                if (params.remainTimeMs == 0)
                {
                    iter = widgets.erase(iter);
                    disconnect(widget, &QObject::destroyed, this, &NotificationLayout::onWidgetDestroyed);
                    delete widget;
                }
                else
                {
                    ++iter;
                }
            }
            layoutWidgets(parent);
            return;
        }
    }
}

void NotificationLayout::SetDisplayTimeMs(int displayTimeMs_)
{
    displayTimeMs = displayTimeMs_;
}

void NotificationLayout::onCloseClicked(NotificationWidget* notification)
{
    delete notification;
}

void NotificationLayout::onWidgetDestroyed() {
    NotificationWidget* notification = static_cast<NotificationWidget*>(sender());
    for (AllNotifications::Iterator iter = notifications.begin(); iter != notifications.end(); ++iter) {
        WindowNotifications& widgets = *iter;
        WindowNotifications::iterator widgetsIter = std::find_if(widgets.begin(), widgets.end(), [notification](const NotificationPair& pair) {
            return pair.first == notification;
        });
        if (widgetsIter != widgets.end())
        {
            widgets.erase(widgetsIter);
            layoutWidgets(iter.key());
            return;
        }
    }
}

void NotificationLayout::onParentWidgetDestroyed()
{
    QWidget* senderWidget = static_cast<QWidget*>(sender());
    notifications.remove(senderWidget);
}
