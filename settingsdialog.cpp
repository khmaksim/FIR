#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->selectFileButton, SIGNAL(clicked()), this, SLOT(selectFileDatabase()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(writeSettings()));

    readSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::selectFileDatabase()
{
    QString fileDatabase = QFileDialog::getOpenFileName(this, tr("Select file database"), QDir::homePath(), tr("Microsoft Access (*.mdb)"));

    if (!fileDatabase.isEmpty()) {
        ui->fileDatabaseLineEdit->setText(QDir::toNativeSeparators(fileDatabase));
    }
}

void SettingsDialog::writeSettings()
{
    QSettings settings;

    settings.beginGroup("database");
    settings.setValue("file", ui->fileDatabaseLineEdit->text().simplified());
    settings.endGroup();

    this->accept();
}

void SettingsDialog::readSettings()
{
    QSettings settings;

    settings.beginGroup("database");
    ui->fileDatabaseLineEdit->setText(settings.value("file").toString());
    settings.endGroup();
}
