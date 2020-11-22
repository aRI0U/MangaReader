#include "MangaSqlModel.h"

#include <QSqlError>

MangaSqlModel::MangaSqlModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    db.open();
    setTable("Mangas");
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    setSort(3, Qt::AscendingOrder);

    select();
}

MangaSqlModel::~MangaSqlModel() {
    QSqlDatabase::removeDatabase("QSQLITE");
}


Qt::ItemFlags MangaSqlModel::flags(const QModelIndex &index) const {
    // Column 1 always records the mark variable (our boolean).
    if (index.column() == followColumn)
        return QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable;

    return QSqlTableModel::flags(index) & ~Qt::ItemIsEditable;
}

QVariant MangaSqlModel::data(const QModelIndex &index, int role) const {
    if (index.column() == followColumn) {
        if (role == Qt::CheckStateRole) {
            int checked = QSqlTableModel::data(index).toInt();
            return checked ? Qt::Checked : Qt::Unchecked;
        }
        else
            return QVariant();
    }
    return QSqlTableModel::data(index, role);
}

bool MangaSqlModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.column() == followColumn && role == Qt::CheckStateRole) {
        if (value == Qt::Checked)
            return setData(index, true);
        else if (value == Qt::Unchecked)
            return setData(index, false);
    }
    return QSqlTableModel::setData(index, value, role);
}

QSqlDatabase MangaSqlModel::mangaDatabase() {
    QPath dbPath = QPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) / constants::dbFilename;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    return db;
}
