#include "NotificationWidget.h"

NotificationWidget::NotificationWidget(QWidget* parent, const QString title, const QString message)
    : QWidget(parent)
{
    //operator | declared in the global namespace
    //without this string compilation will be failed
    Qt::WindowFlags flags = (
                  Qt::FramelessWindowHint    // Disable window decoration
                | Qt::Tool                   // Discard display in a separate window
                | Qt::WindowStaysOnTopHint);
    setWindowFlags(flags);

    setAttribute(Qt::WA_TranslucentBackground); // Indicates that the background will be transparent
    setAttribute(Qt::WA_ShowWithoutActivating); // At the show, the widget does not get the focus automatically

    initUI(title, message);

    QDesktopWidget* desktop = QApplication::desktop();
    QRect geometry = desktop->availableGeometry(parent);
    setFixedWidth(geometry.width() / 5);
    setMaximumHeight(geometry.height() / 3);
}

void NotificationWidget::OnCloseButtonClicked()
{
    emit CloseButtonClicked(this);
}

void NotificationWidget::initUI(const QString title, const QString message) {
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QVBoxLayout* messageLayout = new QVBoxLayout();
    messageLayout->setSpacing(5);
    mainLayout->addItem(messageLayout);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(10);
    messageLayout->addItem(titleLayout);

    QLabel* labelTitle = new QLabel(title);
    labelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    labelTitle->setStyleSheet("font-weight: bold; color: blue;");
    titleLayout->addWidget(labelTitle);

    QLabel* labelMessage = new QLabel(message);
    labelMessage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    labelMessage->setWordWrap(true);
    messageLayout->addWidget(labelMessage);

    QPalette palette;
    QColor baseColor = palette.color(QPalette::Midlight);
    QColor buttonColor = baseColor;
    buttonColor.setAlpha(0);
    QColor pressedColor = baseColor;
    pressedColor.setAlpha(255);

//    QString styleSheet = QString("QPushButton {"
//                                 "border-radius: 1px;"
//                                 "background-color: " +
//                                 NotificationWidgetDetails::ColorToHTML(buttonColor) + ";"
//                                                                                       "padding: 5px;"
//                                                                                       "}"
//                                                                                       "QPushButton:pressed {"
//                                                                                       "background-color: " +
//                                 NotificationWidgetDetails::ColorToHTML(pressedColor) + ";"
//                                                                                        "}");

    QVBoxLayout* buttonsLayout = new QVBoxLayout();
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(5);
    {
        closeButton = new QPushButton(tr("Close"));
//        closeButton->setStyleSheet(styleSheet);
        closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
        buttonsLayout->addWidget(closeButton);
        connect(closeButton, SIGNAL(clicked()), this, SLOT(OnCloseButtonClicked()));
    }
    mainLayout->addItem(buttonsLayout);
    setLayout(mainLayout);
}

void NotificationWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    const int radius = 10;
    roundedRect.setX(rect().x() + radius / 2);
    roundedRect.setY(rect().y() + radius / 2);
    roundedRect.setWidth(rect().width() - radius);
    roundedRect.setHeight(rect().height() - radius);

    QPalette palette;
    QColor rectColor = palette.color(QPalette::Window);
    painter.setBrush(QBrush(rectColor));
    QPen roundedRectPen(Qt::black);
    painter.setPen(roundedRectPen);

    painter.drawRoundedRect(roundedRect, radius, radius);

    QRect closeButtonGeometry = closeButton->geometry();
    QColor lineColor = palette.color(QPalette::Text);
    QPen pen(lineColor);
    pen.setWidth(1);
    painter.setPen(pen);
    //horizontal line
    if (detailsButton != nullptr)
    {
        QRect detailsButtonGeometry = detailsButton->geometry();
        int y = (closeButtonGeometry.bottom() + detailsButtonGeometry.top()) / 2;
        QPoint left(qMin(closeButtonGeometry.left(), detailsButtonGeometry.left()), y);
        QPoint right(qMax(closeButtonGeometry.right(), detailsButtonGeometry.right()), y);
        painter.drawLine(left, right);
    }

    //vertical line
    //close button and details button have Preferred size policy
    int x = closeButtonGeometry.left() - pen.width();
    QPoint top(x, roundedRect.top() + roundedRectPen.width());
    QPoint bottom(x, roundedRect.bottom() - roundedRectPen.width());
    painter.drawLine(top, bottom);
}
