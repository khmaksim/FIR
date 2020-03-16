#ifndef FILTERPANEL_H
#define FILTERPANEL_H

#include <QObject>
#include <QWidget>

class QLineEdit;
class QSlider;
class QPushButton;
class QHBoxLayout;
class QFormLayout;
class FilterPanel : public QWidget
{
        Q_OBJECT
    public:
        explicit FilterPanel(QWidget *parent = nullptr);

    protected:
        bool eventFilter(QObject *obj, QEvent *event);

    private:
        QLineEdit *latLineEdit;
        QLineEdit *lonLineEdit;
        QSlider *slider;
        QPushButton *closeButton;
        QPushButton *filterButton;
        QHBoxLayout *bottomLayout;
        QFormLayout *formLayout;
};

#endif // FILTERPANEL_H
