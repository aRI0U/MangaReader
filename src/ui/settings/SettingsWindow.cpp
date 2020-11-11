#include "SettingsWindow.h"


SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent, Qt::Window),
      layout(new QHBoxLayout(this))
{
    setWindowTitle(tr("Settings"));
    setFocusPolicy(Qt::StrongFocus);
    setWindowModality(Qt::ApplicationModal);
    setMinimumSize(constants::settingsWindowMinimumSize);

    QScrollArea *scrollArea = new QScrollArea();
    QMenu *menu = new QMenu();

    scrollArea->setFixedWidth(constants::settingsMenuWidth);
    menu->setFixedWidth(scrollArea->width());

    // download settings
    QAction *downloadSettings = new QAction(tr("Download"));
    menu->addAction(downloadSettings);
    connect(downloadSettings, SIGNAL(triggered()),
            this, SLOT(openDownloadSettings()));

    // language settings
    QAction *languageSettings = new QAction(tr("Language"));
    menu->addAction(languageSettings);
    connect(languageSettings, SIGNAL(triggered()),
            this, SLOT(openLanguageSettings()));

//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));
//    menu->addAction(new QAction("test"));

    scrollArea->setWidget(menu);

    layout->addWidget(scrollArea);
    layout->setAlignment(scrollArea, Qt::AlignLeft);
}


void SettingsWindow::openDownloadSettings() {
    QGroupBox *settingsWidget = new QGroupBox(tr("Download"), this);
    QGridLayout *internalLayout = new QGridLayout(settingsWidget);


    internalLayout->addWidget(new QLabel(tr("Followed series:")), 0, 0, Qt::AlignLeft);
    internalLayout->addWidget(new QLabel("Available series:"), 1, 0, 1, -1, Qt::AlignLeft);

    MangaSqlModel *model = new MangaSqlModel(this, MangaSqlModel::mangaDatabase());

    QTableView *view = new QTableView;
    internalLayout->addWidget(view, 2, 0, 1, -1, Qt::AlignLeft);

    view->setModel(model);
    view->horizontalHeader()->setStretchLastSection(true);
    view->resizeColumnsToContents();
    view->show();

    // automatic download of last chapters
    QCheckBox *checkCheckBox = new QCheckBox(tr("Automatically check for new chapters"));
    internalLayout->addWidget(checkCheckBox, 4, 0);
    connect(checkCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(setAutomaticCheckState(bool)));

    // automatic download of last chapters
    QCheckBox *downloadCheckBox = new QCheckBox(tr("Automatically download new chapters"));
    internalLayout->addWidget(downloadCheckBox, 5, 0);
    connect(downloadCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(setAutomaticDownloadState(bool)));

    QPushButton *downloadButton = new QPushButton(tr("Download"));
    internalLayout->addWidget(downloadButton, 5, 1, Qt::AlignRight);

    // enable download only when automatic checking is activated
    connect(checkCheckBox, SIGNAL(toggled(bool)),
            downloadCheckBox, SLOT(setEnabled(bool)));

    // restore previous/default values
    QSettings settings;
    settings.beginGroup("Download");
    checkCheckBox->setChecked(settings.value("autoCheck", false).toBool());
    downloadCheckBox->setChecked(settings.value("autoDownload", false).toBool());
    settings.endGroup();


    QLayoutItem *old = layout->takeAt(1);

    if (old != nullptr)
        delete old->widget();

    layout->addWidget(settingsWidget);
}

void SettingsWindow::openLanguageSettings() {
    QGroupBox *settingsWidget = new QGroupBox(tr("Language"), this);
    QGridLayout *internalLayout = new QGridLayout(settingsWidget);


    internalLayout->addWidget(new QLabel(tr("Application language:")), 0, 0, Qt::AlignRight);

    languageChoices = new QComboBox(this);
    QPath translationsDir(constants::translationsPath);
    QRegularExpression translationRegex(constants::applicationName + "_*.qm");
    for (const QString &translationFile : translationsDir.iter(QStringList() << translationRegex.pattern(), QPath::Files | QPath::NoDotAndDotDot)) {
        QString languageString = translationFile.split('_').at(1).split('.').at(0);  // TODO: do this properly with regex or sth...
        QLocale::Language language = QLocale(languageString).language();
        languageChoices->addItem(QLocale::languageToString(language), languageString);
    }
    internalLayout->addWidget(languageChoices, 0, 1, Qt::AlignLeft);
    connect(languageChoices, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setLanguage(int)));


    QLayoutItem *old = layout->takeAt(1);

    if (old != nullptr)
        delete old->widget();

    layout->addWidget(settingsWidget);
}


void SettingsWindow::setAutomaticCheckState(bool checked) {
    QSettings settings;
    settings.setValue("Download/autoCheck", checked);
}

void SettingsWindow::setAutomaticDownloadState(bool checked) {
    QSettings settings;
    settings.setValue("Download/autoDownload", checked);
}

void SettingsWindow::setLanguage(int index) {
    QSettings settings;
    settings.setValue("Language/language", languageChoices->itemData(index));

    QMessageBox msgBox;
    msgBox.setText(tr("Language settings have been modified. Changes will be effective next time you open the app."));
    msgBox.exec();
}
