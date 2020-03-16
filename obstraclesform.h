#ifndef OBSTRACLESFORM_H
#define OBSTRACLESFORM_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
    class ObstraclesForm;
}

class QStandardItemModel;
class QToolBar;
class QToolButton;
class SearchModel;
class QFrame;
class SortSearchFilterObstracleModel;
class SideBar;
class MapView;
class QLabel;
class ObstraclesForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ObstraclesForm(QWidget *parent = nullptr);
        ~ObstraclesForm();

        QVariantList getCheckedObstralcles();

    protected:
        void resizeEvent(QResizeEvent *event);

    private:
        Ui::ObstraclesForm *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *importButton;
        QToolButton *removeAirfieldButton;
        QToolButton *displayOnMapButton;
        QStandardItemModel *airfieldsModel;
        QStandardItemModel *obstraclesModel;
        SearchModel *searchAirfieldsModel;
        SortSearchFilterObstracleModel *sortSearchFilterObstracleModel;
        QFrame *filterPanel;
        SideBar *sideBar;
        MapView *mapView;
        QLabel *totalObstraclesLabel;
        QLabel *selectedObstraclesLabel;
        QLabel *dateUpdatedLabel;

        void readSettings();
        void writeSettings();
        void updateModelAirfields();

    private slots:
        void updateModelObstracles(const QModelIndex &index = QModelIndex());
        void enabledToolButton();
//        void showFilterPanel();
        void exportToFile();
        void importData();
        void setCheckedAllRowTable(bool checked = false);
        void setChecked(bool, QString);
        void setFilterRadius();
        void showObstracles(QVariant c = QVariant(), QVariant radius = QVariant(0));
        void updateStatusSelectedObstracles();
        void removeAirfield();
};

#endif // OBSTRACLESFORM_H
