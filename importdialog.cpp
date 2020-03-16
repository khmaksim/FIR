#include "importdialog.h"
#include "ui_importdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QRegExpValidator>

ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);

    ui->nameAirfieldLineEdit->setValidator(new QRegExpValidator(QRegExp("[A-ZА-Яa-zа-я\\s-\\d]+")));
    ui->codeIcaoLineEdit->setValidator(new QRegExpValidator(QRegExp("[\\dA-Za-z]+")));
    ui->latitudeKtaLineEdit->setValidator(new QRegExpValidator(QRegExp("\\d+(\\.|\\,)?\\d{,2}")));
    ui->longitudeKtaLineEdit->setValidator(new QRegExpValidator(QRegExp("\\d+(\\.|\\,)?\\d{,2}")));

    connect(ui->selectFileButton, SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->importButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->fileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableButton()));
    connect(ui->nameAirfieldLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableButton()));
    connect(ui->latitudeKtaLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableButton()));
    connect(ui->longitudeKtaLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableButton()));
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::selectFile()
{
    QString nameFile = QFileDialog::getOpenFileName(this, tr("Import from Excel file"), QDir::homePath(), tr("Excel files (*.xlsx)"));
    if (nameFile.isEmpty()) {
        qDebug() << "Empty name open file";
        return;
    }
    ui->fileLineEdit->setText(QDir::toNativeSeparators(nameFile));
}

void ImportDialog::enableButton()
{
    ui->importButton->setEnabled(!ui->fileLineEdit->text().isEmpty() && !ui->nameAirfieldLineEdit->text().isEmpty() &&
                                 !ui->latitudeKtaLineEdit->text().isEmpty() && !ui->longitudeKtaLineEdit->text().isEmpty());
}

QString ImportDialog::getFileName() const
{
    return ui->fileLineEdit->text().simplified();
}

QString ImportDialog::getAirfield() const
{
    return ui->nameAirfieldLineEdit->text().simplified();
}

QString ImportDialog::getCodeICAO() const
{
    return ui->codeIcaoLineEdit->text().simplified();
}

QString ImportDialog::getLatitude() const
{
    return ui->northRadioButton->isChecked() ? tr("n") : tr("s");
}

QString ImportDialog::getLongetude() const
{
    return ui->eastRadioButton->isChecked() ? tr("e") : tr("w");
}

QStringList ImportDialog::getCoordiantesKTA() const
{
    return QStringList() << ui->latitudeKtaLineEdit->text().simplified().replace(",", ".")
                         << ui->longitudeKtaLineEdit->text().simplified().replace(",", ".");
}
