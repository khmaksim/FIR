#include "sidebar.h"
#include "ui_sidebar.h"
#include <QDebug>
#include <QInputDialog>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QBitmap>
#include <QIntValidator>
#include <QSettings>
#include <QMessageBox>
#include "flowlayout.h"
#include "obstraclesform.h"
#include "databaseaccess.h"
#include "helper.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    QPalette palette = this->palette();
//    palette.setBrush(QPalette::Background, QBrush(QColor(128, 128, 128)));
//    setPalette(palette);
//    QPixmap maskPix(":/images/res/img/mask.png");
//    setMask(maskPix.scaled(size()).mask());

    ui->arrow->installEventFilter(this);
    isShown = false;
    this->parent = parent;

    ui->toHeightLineEdit->setValidator(new QIntValidator());
    ui->fromHeightLineEdit->setValidator(new QIntValidator());
    ui->minRadiusLabel->setNum(ui->radiusSlider->minimum());
    ui->maxRadiusLabel->setNum(ui->radiusSlider->maximum());
    ui->scrollAreaWidgetContents->setLayout(new FlowLayout());

    updateTags();

    connect(ui->searchLineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(searchTextChanged(QString)));
    connect(ui->markingDayCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->nightMarkingCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabelValueRadius(int)));
    connect(ui->radiusSlider, SIGNAL(valueChanged(int)), this, SIGNAL(filterRadius()));
    connect(ui->addTagButton, SIGNAL(clicked(bool)), this, SLOT(showAddTag()));
    connect(ui->removeTagButton, SIGNAL(clicked(bool)), this, SLOT(removeTag()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(setRadius()));
    connect(ui->resetFilterButton, SIGNAL(clicked(bool)), this, SLOT(resetFilter()));
    connect(ui->displayObstraclesButton, SIGNAL(clicked(bool)), this, SLOT(clickedDisplayObstraclesButton()));
    connect(ui->fromHeightLineEdit, SIGNAL(textChanged(QString)), this, SLOT(heightFilterChanged()));
    connect(ui->toHeightLineEdit, SIGNAL(textChanged(QString)), this, SLOT(heightFilterChanged()));
    connect(ui->firstAreaCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxAreaChanged()));
    connect(ui->secondAreaCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxAreaChanged()));
    connect(ui->thridAreaCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxAreaChanged()));
    connect(ui->fourthAreaCheckBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxAreaChanged()));
    readSettings();
}

SideBar::~SideBar()
{
    writeSettings();
    delete ui;
}

void SideBar::writeSettings()
{
    QSettings settings;

    settings.beginGroup("sideBar");
    settings.setValue(ui->latLineEdit->objectName(), ui->latLineEdit->text());
    settings.setValue(ui->lonLineEdit->objectName(), ui->lonLineEdit->text());
    settings.endGroup();
}

void SideBar::readSettings()
{
    QSettings settings;

    settings.beginGroup("sideBar");
    ui->latLineEdit->setText(settings.value(ui->latLineEdit->objectName()).toString());
    ui->lonLineEdit->setText(settings.value(ui->lonLineEdit->objectName()).toString());
    settings.endGroup();
}

bool SideBar::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->arrow && (event->type() == QEvent::MouseButtonPress ||
                                 event->type() == QEvent::MouseButtonDblClick)) {
        showHide();
    }
    return QWidget::eventFilter(watched, event);
}

void SideBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPixmap maskPix(":/images/res/img/mask.png");
    ui->arrow->setMask(maskPix.scaled(ui->arrow->size()).mask());
}

QSize SideBar::sizeHint()
{
    return QSize(300, 400);
}

void SideBar::updateTypeObstracleFilter(const QStringList types)
{
    if (types.isEmpty())
        return;

    // remove type checkbox in filter
    QList<QWidget*> typesCheckBox = ui->typeObstracleScrollAreaWidgetContents->findChildren<QWidget*>();
    for (QList<QWidget*>::iterator it = typesCheckBox.begin(); it != typesCheckBox.end(); ++it) {
        ui->typeObstracleScrollAreaWidgetContents->layout()->removeWidget(*it);
        (*it)->hide();
        delete (*it);
    }
    delete ui->typeObstracleScrollAreaWidgetContents->layout();

    ui->typeObstracleScrollAreaWidgetContents->setLayout(new QVBoxLayout(ui->typeObstracleScrollAreaWidgetContents));
    for (int i = 0; i < types.size(); i++) {
        QCheckBox *type = new QCheckBox(types.at(i), ui->typeObstracleScrollAreaWidgetContents);
        connect(type, SIGNAL(clicked(bool)), this, SLOT(checkBoxTypesChanged()));
        ui->typeObstracleScrollAreaWidgetContents->layout()->addWidget(type);
        ui->typeObstracleScrollAreaWidgetContents->adjustSize();
    }
    qobject_cast<QVBoxLayout*>(ui->typeObstracleScrollAreaWidgetContents->layout())->addStretch();
}

void SideBar::updateTags()
{
    QVector<QString> tags = DatabaseAccess::getInstance()->getTags();

    if (tags.isEmpty())
        return;

    for (int i = 0; i < tags.size(); i++)
        addTagToScrollArea(tags.at(i));
}

void SideBar::checkBoxChanged(bool f)
{
    emit changedFilterProperty(sender()->objectName(), QVariant(f));
}

void SideBar::updateLabelValueRadius(int value)
{
    ui->valueRadiusLabel->setText(QString("%1 %2").arg(value).arg(tr("km")));
    ui->pushButton->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
}

QString SideBar::getLat()
{
    return ui->latLineEdit->text();
}

QString SideBar::getLon()
{
    return ui->lonLineEdit->text();
}

int SideBar::getRadius()
{
    return ui->radiusSlider->value();
}

void SideBar::showAddTag()
{
    QInputDialog *inputDialog = new QInputDialog(this, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    inputDialog->setInputMode(QInputDialog::TextInput);
    inputDialog->setLabelText(tr("Name tag:"));
    inputDialog->setWindowTitle(tr("Create tag"));
    inputDialog->setOkButtonText(tr("Create"));
    inputDialog->setCancelButtonText(tr("Cancel"));
    if (inputDialog->exec() == QInputDialog::Accepted) {
        if (DatabaseAccess::getInstance()->createTag(inputDialog->textValue())) {
            addTagToScrollArea(inputDialog->textValue());
        }
    }
}

void SideBar::addTagToScrollArea(const QString &nameTag)
{
    QCheckBox *tag = new QCheckBox(nameTag, ui->scrollAreaWidgetContents);
    connect(tag, SIGNAL(clicked(bool)), this, SLOT(setTagForObstracles(bool)));
    connect(tag, SIGNAL(clicked(bool)), this, SLOT(checkBoxTagsChanged()));
    connect(tag, SIGNAL(clicked(bool)), this, SLOT(enabledRemoveTagButton()));
    ui->scrollAreaWidgetContents->layout()->addWidget(tag);
    ui->scrollAreaWidgetContents->adjustSize();
}

void SideBar::showSelectColorTag()
{

}

void SideBar::setRadius()
{
    int value = qobject_cast<QPushButton*>(sender())->text().remove(QRegExp("\\D+")).toInt();
    ui->radiusSlider->setValue(value);
}

void SideBar::resetFilter()
{
    ui->markingDayCheckBox->setChecked(false);
    ui->nightMarkingCheckBox->setChecked(false);
    ui->radiusGroupBox->setChecked(false);
    ui->latLineEdit->clear();
    ui->lonLineEdit->clear();
    ui->radiusSlider->setValue(50);
    ui->fromHeightLineEdit->clear();
    ui->toHeightLineEdit->clear();
}

void SideBar::showHide()
{
    animationSideBar = new QPropertyAnimation(this, "geometry");
    animationSideBar->setDuration(300);

//    QRect startRect(parent->width() - 20, y(), width(), height());
//    QRect endRect(parent->width() - width(), y(), width(), height());
    QRect startRect(0 - width() + 20, y(), width(), height());
    QRect endRect(0, y(), width(), height());

    if (!isShown) {
        animationSideBar->setStartValue(startRect);
        animationSideBar->setEndValue(endRect);
    }
    else {
        animationSideBar->setStartValue(endRect);
        animationSideBar->setEndValue(startRect);
    }
    animationSideBar->start();
    QTimer::singleShot(animationSideBar->duration(), this, SLOT(changeArrow()));

    isShown = !isShown;
}

void SideBar::changeArrow()
{
    if (isShown)
        ui->arrow->setPixmap(QPixmap(":/images/res/img/arrow-left.png"));
    else
        ui->arrow->setPixmap(QPixmap(":/images/res/img/arrow-right.png"));

}

void SideBar::setTagForObstracles(bool checked)
{
    if (!checked)
        return;
    QVariantList idObstracles = qobject_cast<ObstraclesForm*>(this->parent)->getCheckedObstralcles();
    QCheckBox *selectedTag = qobject_cast<QCheckBox*>(sender());
    DatabaseAccess::getInstance()->setTag(selectedTag->text(), idObstracles);
}

void SideBar::checkBoxTagsChanged()
{
    QStringList tags;
    QList<QCheckBox*> tagsCheckBox = ui->scrollAreaWidgetContents->findChildren<QCheckBox*>();
    for (QList<QCheckBox*>::iterator it = tagsCheckBox.begin(); it != tagsCheckBox.end(); ++it) {
        if ((*it)->isChecked())
            tags << (*it)->text();
    }
    emit changedFilterProperty("tags", QVariant(tags));
}

void SideBar::checkBoxTypesChanged()
{
    QStringList types;
    QList<QCheckBox*> typesCheckBox = ui->typeObstracleScrollAreaWidgetContents->findChildren<QCheckBox*>();
    for (QList<QCheckBox*>::iterator it = typesCheckBox.begin(); it != typesCheckBox.end(); ++it) {
        if ((*it)->isChecked())
            types << (*it)->text();
    }
    emit changedFilterProperty("types", QVariant(types));
}

void SideBar::checkBoxAreaChanged()
{
    QStringList areas;
    QList<QCheckBox*> areasCheckBox = ui->areaGroupBox->findChildren<QCheckBox*>();
    for (QList<QCheckBox*>::iterator it = areasCheckBox.begin(); it != areasCheckBox.end(); ++it) {
        if ((*it)->isChecked())
            areas << (*it)->text().right(1);
    }
    emit changedFilterProperty("areas", QVariant(areas));
}

void SideBar::clickedDisplayObstraclesButton()
{
    if (ui->radiusGroupBox->isChecked())
        emit displayObstracles(QVariant(QPointF(Helper::convertCoordinateInDec(ui->latLineEdit->text()),
                                                Helper::convertCoordinateInDec(ui->lonLineEdit->text()))),
                               QVariant(ui->radiusSlider->value()));
    else
        emit displayObstracles(QVariant(QPointF()), QVariant(ui->radiusSlider->value()));
}

void SideBar::heightFilterChanged()
{
    changedFilterProperty("height", QVariant(QList<QVariant>() << ui->fromHeightLineEdit->text().toInt() << ui->toHeightLineEdit->text().toInt()));
}

void SideBar::enabledRemoveTagButton()
{
    bool isEnabled = false;
    QList<QCheckBox*> tagsCheckBox = ui->scrollAreaWidgetContents->findChildren<QCheckBox*>();
    for (QList<QCheckBox*>::iterator it = tagsCheckBox.begin(); it != tagsCheckBox.end(); ++it) {
        if ((*it)->isChecked()) {
            isEnabled = true;
            break;
        }
    }
    ui->removeTagButton->setEnabled(isEnabled);
}

void SideBar::removeTag()
{
    QMessageBox messageBox(QMessageBox::Question, tr("Confirmation"), tr("Are you sure you want to remove these tags?"));
    messageBox.addButton(tr("Yes"), QMessageBox::AcceptRole);
    QPushButton *noButton = messageBox.addButton(tr("No"), QMessageBox::RejectRole);
    messageBox.setDefaultButton(noButton);

    if (messageBox.exec() == QMessageBox::AcceptRole) {
        QList<QCheckBox*> tagsCheckBox = ui->scrollAreaWidgetContents->findChildren<QCheckBox*>();
        for (QList<QCheckBox*>::iterator it = tagsCheckBox.begin(); it != tagsCheckBox.end(); ++it) {
            if ((*it)->isChecked())
                if (DatabaseAccess::getInstance()->removeTag((*it)->text())) {
                    (*it)->hide();
                    delete (*it);
                }
        }
        ui->removeTagButton->setEnabled(false);
    }
}

void SideBar::reset()
{
    isShown = false;
    changeArrow();
}
