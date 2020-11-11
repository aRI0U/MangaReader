#ifndef MANGASQLMODEL_H
#define MANGASQLMODEL_H

#include <QSqlTableModel>

#include "constants.h"
#include "libs/QPath/QPath.h"

class MangaSqlModel : public QSqlTableModel
{
public:
    MangaSqlModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    static QSqlDatabase mangaDatabase();
};

#endif // MANGASQLMODEL_H
