/* The major part of this code is inspired by the following article:
 * https://www.wouterspekkink.org/software/q-sopra/technical/c++/qt/2018/01/19/qsltablemodels-booleans-and-check-boxes.html
 */

#ifndef MANGASQLMODEL_H
#define MANGASQLMODEL_H

#include <QSqlTableModel>

#include "constants.h"
#include "libs/QPath/QPath.h"

class MangaSqlModel : public QSqlTableModel
{
public:
    explicit MangaSqlModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~MangaSqlModel();

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &idx, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    static QSqlDatabase mangaDatabase();

    const int nameColumn = 1;
    const int followColumn = 5;
};

#endif // MANGASQLMODEL_H
