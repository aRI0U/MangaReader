#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <functional>

class NotificationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QPoint position READ pos WRITE move)
    Q_PROPERTY(float opacity READ windowOpacity WRITE setWindowOpacity)

public:
    explicit NotificationWidget(QWidget* parent = nullptr, const QString title = "", const QString message = "");

signals:
    void CloseButtonClicked(NotificationWidget* current);

private slots:
    void OnCloseButtonClicked();

private:
    void initUI(const QString title, const QString message);

    void paintEvent(QPaintEvent* event) override;

    QPushButton* closeButton = nullptr;
    QPushButton* detailsButton = nullptr;
};
#endif // NOTIFICATIONWIDGET_H
