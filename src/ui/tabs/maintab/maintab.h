#ifndef MAINTAB_H
#define MAINTAB_H

#include <QDir>
#include <QWidget>

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = nullptr);

public slots:
    void clickedManga(QDir mangaDir);

signals:

};

#endif // MAINTAB_H
