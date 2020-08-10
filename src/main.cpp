#include <QApplication>
#include <QTranslator>

#include "constants.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(constants::organizationName);
    QCoreApplication::setApplicationName(constants::applicationName);

    QTranslator translator;
    translator.load(QString("translations/MangaReader_") + QLocale::system().name());
    app.installTranslator(&translator);

    MainWindow w;
    w.show();
    return app.exec();
}
