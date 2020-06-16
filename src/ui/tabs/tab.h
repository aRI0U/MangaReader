#ifndef TAB_H
#define TAB_H

#include <QWidget>

#include "mangatab/reader.h"

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);

    virtual Reader* getReader() const;

signals:

};

#endif // TAB_H
