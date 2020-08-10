#ifndef TAB_H
#define TAB_H

#include <QWidget>

#include "mangatab/Reader.h"

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);

signals:
    void sendNotification(QString title, QString description);
};

#endif // TAB_H
