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

    internalLayout->addWidget(new QLabel(tr("Available series:")), 2, 0, 1, 1, Qt::AlignLeft);

    QPushButton *applyButton = new QPushButton(tr("Apply changes"));
    internalLayout->addWidget(applyButton, 2, 1, 1, 1, Qt::AlignRight);

    MangaSqlModel *model = new MangaSqlModel(this, MangaSqlModel::mangaDatabase());

    connect(applyButton, &QPushButton::clicked,
            model, &MangaSqlModel::submitAll);

    QTableView *view = new QTableView;
    internalLayout->addWidget(view, 3, 0, 1, -1);

    view->setModel(model);
    view->setMinimumWidth(settingsWidget->width());
    for (int column : {0, 1, 2, 4, 6, 7, 8, 9})
        view->hideColumn(column);
    view->horizontalHeader()->setStretchLastSection(true);
    view->resizeColumnsToContents();
    view->show();

    // automatic download of last chapters
    QCheckBox *checkCheckBox = new QCheckBox(tr("Automatically check for new chapters"));
    internalLayout->addWidget(checkCheckBox, 4, 0, 1, -1);
    connect(checkCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(setAutomaticCheckState(bool)));

    // automatic download of last chapters
    QCheckBox *downloadCheckBox = new QCheckBox(tr("Automatically download new chapters"));
    internalLayout->addWidget(downloadCheckBox, 5, 0, 1, -1);
    connect(downloadCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(setAutomaticDownloadState(bool)));

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

    settingsWidget->setLayout(internalLayout);
    layout->addWidget(settingsWidget);
    settingsWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
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
    settingsWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
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


void SettingsWindow::closeEvent(QCloseEvent *event) {
    deleteLater();
    event->accept();
}
