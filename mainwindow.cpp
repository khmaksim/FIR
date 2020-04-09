#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "obstraclesform.h"
#include "databaseaccess.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QToolBar>
#include <QToolButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QSaveFile>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include "checkboxitemdelegate.h"
#include "mapview.h"
#include "helper.h"
#include "settingsdialog.h"
#include "model/sortsearchfilterobstraclemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << tr("*") << tr("Name") << tr("Name sector"));

    mapView = nullptr;
    toolBar = addToolBar(QString());

    exportButton = new QToolButton(this);
    exportButton->setEnabled(false);
    exportButton->setText(tr("Export"));
    exportButton->setIconSize(QSize(32, 32));
    exportButton->setIcon(QIcon(":/images/res/img/icons8-save-48.png"));
    exportButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    displayOnMapButton = new QToolButton(this);
    displayOnMapButton->setEnabled(false);
    displayOnMapButton->setText(tr("Unhide"));
    displayOnMapButton->setIconSize(QSize(32, 32));
    displayOnMapButton->setIcon(QIcon(":/images/res/img/icons8-map-marker-48.png"));
    displayOnMapButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsButton = new QToolButton(this);
    settingsButton->setText(tr("Settings"));
    settingsButton->setIconSize(QSize(32, 32));
    settingsButton->setIcon(QIcon(":/images/res/img/icons8-settings-48.png"));
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QLineEdit *searchLine = new QLineEdit(this);
    searchLine->setPlaceholderText(tr("Search..."));
    searchLine->setClearButtonEnabled(true);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    toolBar->addWidget(exportButton);
    toolBar->addWidget(displayOnMapButton);
    toolBar->addWidget(settingsButton);
    toolBar->addSeparator();
    toolBar->addWidget(searchLine);
    toolBar->addWidget(spacer);

    SortSearchFilterObstracleModel *sortSearchFilterObstracleModel = new SortSearchFilterObstracleModel(this);
    sortSearchFilterObstracleModel->setSourceModel(model);

    ui->tableView->setModel(sortSearchFilterObstracleModel);
    ui->tableView->setItemDelegateForColumn(0, new CheckboxItemDelegate(this));
//    ObstraclesForm *form = new ObstraclesForm(this);
//    ui->stackedWidget->addWidget(form);
//    ui->stackedWidget->setCurrentIndex(1);

//    connect(obstraclesHandler, SIGNAL(finished(Airfield,QVector<QVector<QString> >&)), DatabaseAccess::getInstance(), SLOT(update(Airfield,QVector<QVector<QString> >&)));
//    connect(ui->obstracleButton, SIGNAL(clicked(bool)), this, SLOT(showObstracles()));
    readSettings();
    updateZoneTable();

    connect(searchLine, SIGNAL(textChanged(const QString&)), sortSearchFilterObstracleModel, SLOT(setFilterRegExp(QString)));
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(enabledToolButton()));
    connect(exportButton, SIGNAL(clicked(bool)), this, SLOT(exportToFile()));
    connect(displayOnMapButton, SIGNAL(clicked(bool)), this, SLOT(showZones()));
    connect(settingsButton, SIGNAL(clicked(bool)), this, SLOT(showSettings()));
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::updateZoneTable()
{
    // remove all rows
    while (model->rowCount() > 0) {
        model->removeRow(0);
    }

    QVector<Record> zones = DatabaseAccess::getInstance()->getZones();
    // uncheked header
//    qobject_cast<QGroupHeaderView*>(ui->tableView->horizontalHeader())->setChecked(false);

    for (int i = 0; i < zones.size(); i++) {
        QList<QStandardItem *> items;
        Record record = zones.at(i);

        QStandardItem *item = new QStandardItem();
        item->setData(record.first().toUInt(), Qt::UserRole + 1);      // set tags for obstracles to first column
//        item->setData(fields.takeLast().toString(), Qt::UserRole + 1);      // set tags for obstracles to first column
//        item->setData(fields.takeLast().toString(), Qt::UserRole + 2);      // set datetime last updated
        items.append(item);
        // get name type obstracles
        for (int j = 1; j < record.size(); j++) {
            items.append(new QStandardItem(record.value(j).toString()));
        }
        model->appendRow(items);
    }
}

void MainWindow::showObstracles()
{
//    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    settings.setValue("maximized", this->isMaximized());
    settings.setValue(ui->tableView->objectName(), ui->tableView->horizontalHeader()->saveState());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("geometry");
    if (settings.value("maximized").toBool())
        this->showMaximized();
    ui->tableView->horizontalHeader()->restoreState(settings.value(ui->tableView->objectName()).toByteArray());
    settings.endGroup();
    settings.beginGroup("database");
    settings.endGroup();
}

void MainWindow::enabledToolButton()
{
    bool isEnable = false;
    for (int row = 0; row < model->rowCount(); row++) {
        if (model->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            isEnable = true;
            break;
        }
    }
    exportButton->setEnabled(isEnable);
    displayOnMapButton->setEnabled(isEnable);

    return;
}

void MainWindow::showZones()//QVariant coordinate, QVariant radius)
{
    if (mapView == nullptr) {
        mapView = new MapView;
//        connect(mapView, SIGNAL(checked(bool, QString)), this, SLOT(setChecked(bool, QString)));
    }
    mapView->clearMap();

//    QPointF centerMap = coordinate.toPointF();
    bool setCenterMap = false;
    for (int row = 0; row < model->rowCount(); row++) {
        if (model->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            QVector<Record> points = DatabaseAccess::getInstance()->getPoints(model->index(row, 0).data(Qt::UserRole + 1).toInt());

            QVector<Record>::iterator it;
            QList<QVariant> listPoint;
            for (it = points.begin(); it != points.end(); ++it) {
                double lat = Helper::convertCoordinateInDec(it->first().toString());
                double lon = Helper::convertCoordinateInDec(it->last().toString());

                if (!setCenterMap) {
                    mapView->setCenter(QPointF(lat, lon));
                    setCenterMap = true;
                }

                listPoint << QVariant(QPointF(lat, lon));
            }
            mapView->addZone(listPoint);


        }
    }
//    setCheckedAllRowTable();
//    mapView->setRadius(radius);
//    if (centerMap.isNull()) {
//        QMessageBox::warning(this, tr("Warning"), tr("You must select the obstacles displayed in the table!"));
//        return;
//    }
    mapView->show();
}

void MainWindow::showSettings()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        updateZoneTable();
    }
}

void MainWindow::exportToFile()
{
    QString nameFile = QFileDialog::getSaveFileName(this, tr("Save file"), QString("D:/fir.txt"));
    if (nameFile.isEmpty()) {
        qDebug() << "Empty name save file";
        return;
    }

    QSaveFile file(nameFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (int row = 0; row < model->rowCount(); row++) {
        if (model->index(row, 0).data(Qt::CheckStateRole).toBool()) {
            QVector<Record> points = DatabaseAccess::getInstance()->getPoints(model->index(row, 0).data(Qt::UserRole + 1).toInt());

            QVector<Record>::iterator it;
            QList<QVariant> listPoint;
            for (it = points.begin(); it != points.end(); ++it) {
                out << it->first().toString().replace("с", "N").replace("ю", "S").remove(QRegExp("[\\s\\.]")).append("0") << endl;
                out << it->last().toString().replace("в", "E").replace("з", "W").remove(QRegExp("[\\s\\.]")).append("0") << endl;
            }
        }
    }
    file.commit();
}
