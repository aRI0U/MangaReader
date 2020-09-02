#include <QApplication>
#include <QTranslator>

#include "constants.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(constants::organizationName);
    QCoreApplication::setApplicationName(constants::applicationName);

    QSettings settings;

    QTranslator translator;
    QString language = settings.value("language", QLocale::system().name()).toString();
    QString translationFile = QDir(constants::translationsPath).absoluteFilePath(language);

    if (!translator.load(translationFile))
        qDebug() << translationFile << "is not a valid translation file";
    app.installTranslator(&translator);

    MainWindow w;
    w.show();
    return app.exec();
}
