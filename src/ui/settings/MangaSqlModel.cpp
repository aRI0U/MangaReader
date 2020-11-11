#include "MangaSqlModel.h"

MangaSqlModel::MangaSqlModel(QObject *parent)
    : QSqlTableModel(parent, QSqlDatabase::addDatabase("QSQLITE"))
{
    QPath dbPath = QPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) / constants::dbFilename;
    database().setDatabaseName(dbPath);

    setTable("Mangas");
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    if (!select())
        qDebug() << "Failed to display Sql model:" << selectStatement();
}
