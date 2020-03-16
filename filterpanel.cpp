#include "filterpanel.h"
#include <QFrame>
#include <QSlider>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QEvent>
#include <QMouseEvent>
#include <QRect>

FilterPanel::FilterPanel(QWidget *parent) : QWidget(parent)
{
//    filterPanel = new QFrame(this, Qt::Popup);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    installEventFilter(this);

    latLineEdit = new QLineEdit(this);
    latLineEdit->installEventFilter(this);
    lonLineEdit = new QLineEdit(this);
    lonLineEdit->installEventFilter(this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->installEventFilter(this);
    closeButton = new QPushButton(tr("Close"), this);
    closeButton->installEventFilter(this);
    filterButton = new QPushButton(tr("Filter"), this);
    filterButton->installEventFilter(this);

    bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(filterButton);
    bottomLayout->addWidget(closeButton);

    formLayout = new QFormLayout;
    formLayout->addRow(tr("&Latitude:"), latLineEdit);
    formLayout->addRow(tr("&Longitude:"), lonLineEdit);
    formLayout->addRow(tr("&Range:"), slider);
    formLayout->addRow(bottomLayout);

    setLayout(formLayout);
    setFocus();
}

bool FilterPanel::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn && this->windowOpacity() != 1.0) {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setStartValue(0.5);
        animation->setEndValue(1);
        animation->start();
    }
    else if (event->type() == QEvent::FocusOut) {
        if (!latLineEdit->hasFocus() && !lonLineEdit->hasFocus() && !slider->hasFocus() &&
                !closeButton->hasFocus() && !filterButton->hasFocus()) {
            QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
            animation->setStartValue(1);
            animation->setEndValue(0.5);
            animation->start();
        }
    }
    return QWidget::eventFilter(obj, event);
}
