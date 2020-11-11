#include "MangaSqlModel.h"

MangaSqlModel::MangaSqlModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    db.open();
    setTable("Mangas");
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    // keep only relevant columns
    removeColumns(0, 3);
    removeColumn(1);
    removeColumns(2, 2);

    setSort(0, Qt::AscendingOrder);

    select();

    // TODO: use https://www.wouterspekkink.org/software/q-sopra/technical/c++/qt/2018/01/19/qsltablemodels-booleans-and-check-boxes.html
}


QSqlDatabase MangaSqlModel::mangaDatabase() {
    QPath dbPath = QPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) / constants::dbFilename;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    return db;
}
