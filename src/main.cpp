#include <QApplication>
#include <QTranslator>

#include "constants.h"
#include "ui/MainWindow.h"

#include "utils/scandl/scansDownloaders.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(constants::organizationName);
    QCoreApplication::setApplicationName(constants::applicationName);

    QSettings settings;

    QTranslator translator;

    QLocale language = settings.contains("Language/language")
            ? QLocale(settings.value("Language/language").toString())
            : QLocale::system();

    if (translator.load(language, constants::applicationName, "_", constants::translationsPath))
        app.installTranslator(&translator);
    else
        qDebug() << "Failed to load translation file";


    MainWindow w;
    w.show();
    return app.exec();
}
