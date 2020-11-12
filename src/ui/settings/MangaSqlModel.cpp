#include "MangaSqlModel.h"

#include <QSqlError>

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
}


Qt::ItemFlags MangaSqlModel::flags(const QModelIndex &index) const {
    // Column 1 always records the mark variable (our boolean).
    switch (index.column()) {
        case 0:
            return QSqlTableModel::flags(index) & ~Qt::ItemIsEditable;
        case 1:
            return QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable;
        default:
            return QSqlTableModel::flags(index);
    }
}

QVariant MangaSqlModel::data(const QModelIndex &index, int role) const {
    if (index.column() == 1) {
        if (role == Qt::CheckStateRole) {
            int checked = QSqlTableModel::data(index).toInt();
            qDebug() << index.row() << checked;
            return checked ? Qt::Checked : Qt::Unchecked;
        }
        else
            return QVariant();
    }
    return QSqlTableModel::data(index, role);
}

bool MangaSqlModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.column() == 1 && role == Qt::CheckStateRole) {
        if (value == Qt::Checked)
            return setData(index, true);
        else if (value == Qt::Unchecked)
            return setData(index, false);
    }
    return QSqlTableModel::setData(index, value, role);
}


bool MangaSqlModel::submitAll() {
    qDebug() << isDirty();
    bool val = QSqlTableModel::submitAll();
    qDebug() << val << lastError().text();
    qDebug() << isDirty();
    return val;
}

QSqlDatabase MangaSqlModel::mangaDatabase() {
    QPath dbPath = QPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) / constants::dbFilename;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    return db;
}
