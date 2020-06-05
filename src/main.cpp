#include "ui/mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load(QString("translations/MangaReader_") + QLocale::system().name());
    app.installTranslator(&translator);

    MainWindow w;
    w.show();
    return app.exec();
}
