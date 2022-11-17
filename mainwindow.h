#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "defs.h"
#include "ecs.h"
#include "passenger.h"
#include "elevator.h"
#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QVector>
#include <QtGlobal>
/*
    mainwindow has:
        1. FLoor object is represented in this class as data strutures
            a. A vector of floor-button pairs (up and down) for each floor (Floor has QPushbuttons)
            b. A vector of passengers on floor (floor has passenger)
*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ECS* ecs;
    int selectedFl = 1;  // selected floor by user, default floor 1
    Elevator* selectedElev;  // selected elevator by user, default elevator 0
    Passenger* selectedPax;
    Passenger* selectedPaxElev;
    QVector<Passenger*> pax_onFloor_vec;  // On floor, vector <Pax>, suppose that passenger can be at any floor to make a floor call
    QMap<int, Passenger*> pax_inElev_map; // In elevator, map <carNum, Pax>
    void init();
    void setUpPaxDropdown();

private slots:
    void pressDestBtn();
    void setCurrentFloor();
    void setCurrentElevator();
    void setCurrentPax();
    void setCurrentPaxElev();
    void on_savePassenger_clicked();
    // floor request, no duplicate allowed
    void on_pushButton_up_released();
    void on_pushButton_down_released();
    void on_pushButton_2_released();
    void on_pushButton_nextTen_released();
    void on_pushButton_3_released();
    void on_pushButton_print_clicked();
    void on_pushButton_power_toggled(bool checked);
    void on_pushButton_fire_2_toggled(bool checked);
    void on_pushButton_obstacle_toggled(bool checked);
    void on_pushButton_overweight_toggled(bool checked);
    void on_pushButton_fire_toggled(bool checked);
};
#endif // MAINWINDOW_H
