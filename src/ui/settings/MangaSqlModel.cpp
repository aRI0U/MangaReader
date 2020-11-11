#include "MangaSqlModel.h"

MangaSqlModel::MangaSqlModel(QObject *parent)
    : QSqlTableModel(parent, QSqlDatabase::addDatabase("QSQLITE"))
{
    QPath dbPath = QPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)) / constants::dbFilename;
    database().setDatabaseName(dbPath);
    qDebug() << dbPath;
    setTable("Mangas");
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    if (!select())
        qDebug() << "Failed to display Sql model:" << selectStatement();

    setHeaderData(0, Qt::Horizontal, "");
    setHeaderData(1, Qt::Horizontal, tr("Manga"));

    // TODO: use https://www.wouterspekkink.org/software/q-sopra/technical/c++/qt/2018/01/19/qsltablemodels-booleans-and-check-boxes.html
}
