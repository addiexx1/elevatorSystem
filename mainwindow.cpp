#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize a default passenger: name = Default, weight = 80 kg
    selectedPax = new Passenger("Default", 80);
    pax_onFloor_vec.push_back(selectedPax);
    ecs = new ECS();
    init();
    selectedElev = ecs->getElevators().at(0);

}

MainWindow::~MainWindow()
{
    delete ui;
//    for (int i = 0; i < floor_btns.size(); i++){
//        delete floor_btns[i].first;
//        delete floor_btns[i].second;
//    }
//    floor_btns.clear();
    for (int i = 0; i < pax_onFloor_vec.size(); i++){
        delete pax_onFloor_vec[i];
    }
    foreach (Passenger* pax, pax_inElev_map){
        delete pax;
    }
    pax_onFloor_vec.clear();
    delete ecs;
    //delete pax_inElev_map;
}

/*
    Init() initilizes:
        1. MainWindow,
        2. Things based on numbers in defs.h
            a. Floor btns,
            b. Destination buttons,
            c. dropdowns to select,
            d. Weight Limit,
            e. ECS strategy,
*/

void MainWindow::init(){
    // setup Destination buttons based on NUM_FLOORS
    // in a [3 * r] grid layout
    QGridLayout* layout = ui->gridLayout_destinations;
    for (int i = 0; i < NUM_FLOORS; i++){
        QPushButton *btn = new QPushButton(QString::number(i+1));
        int row = int(i)/3;
        int col = i % 3;
        layout->addWidget(btn,row,col);
        connect(btn, SIGNAL(released()), this, SLOT(pressDestBtn()));
    }

    // Store all floor btns pairs in floor_btns vector, QPair<QPushButton*, QPushButton*>
    // add them in the layout, show/hide dynamically based on the floor (initial floor = 1)
//    QVBoxLayout* vlay = ui->verticalLayout_upAndDown;
//    for (int i = 0; i < NUM_FLOORS; i++){

//        QPushButton* up = new QPushButton("UP");
//        up->setObjectName("btn_up_"+QString::number(i+1)); // btn_up_floor#

//        QPushButton* down = new QPushButton("DOWN");
//        down->setObjectName("btn_down_"+QString::number(i+1)); // btn_down_floor#

//        floor_btns.push_back(qMakePair(up, down));
//        vlay->addWidget(up);
//        vlay->addWidget(down);
//        if (i == 0){
//            continue;
//        }
//        up->hide();
//        down->hide();
//    }

    // setup comboBox_floors (dropdown to select floor)
    QComboBox* dropdown_floors = ui->comboBox_floor;
    for (int i = 0; i < NUM_FLOORS; i++){
        dropdown_floors->addItem(QString::number(i+1));
    }
    connect(dropdown_floors, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentFloor()));

    // setup comboBox_elevators (dropdown to select elevator)
    QComboBox* dropdown_elevators = ui->comboBox_elevators;
    for (int i = 0; i < NUM_ELEVATORS; i++){
        dropdown_elevators->addItem(QString::number(i));
    }
    connect(dropdown_elevators, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentElevator()));

    // setup comboBox_passenger_floor(dropdown to select passenger)
    setUpPaxDropdown();
    connect(ui->comboBox_passenger_floor, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentPax()));
    connect(ui->comboBox_passenger_elev, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentPaxElev()));
}
void MainWindow::setUpPaxDropdown(){
    ui->comboBox_passenger_floor->blockSignals(true);
    ui->comboBox_passenger_elev->blockSignals(true);

    QComboBox* dropdown_pax_onFloor = ui->comboBox_passenger_floor;
    for (int i = 0; i < pax_onFloor_vec.size(); i++){
        Passenger* pax = pax_onFloor_vec.at(i);
        dropdown_pax_onFloor->addItem(pax->getName()+" "+ QString::number(pax->getWeight())+" kg");
    }
    QComboBox* dropdown_pax_elev = ui->comboBox_passenger_elev;
    foreach (int key, pax_inElev_map.keys()){
        Passenger* pax = pax_inElev_map.value(key);
        dropdown_pax_elev->addItem(pax->getName()+" "+ QString::number(pax->getWeight())+" kg");
    }
    ui->comboBox_passenger_floor->blockSignals(false);
    ui->comboBox_passenger_elev->blockSignals(false);
}

/*  POST INITIALIZATION */

/*
    4 SLOTS handle SELECTION of floor, elevator and passenger in UI
*/

 // 1. set selected floor
void MainWindow::setCurrentFloor(){
    QComboBox* floorDropdown = qobject_cast<QComboBox*>(sender());
    selectedFl = floorDropdown->currentText().toInt();
    qDebug() << "Selected Floor:" << selectedFl<<"\n";
    // show/hide UP-DOWN Btn Pair according to the floor selection
    // hide the old btns
//    QVBoxLayout* vlay = ui->verticalLayout_upAndDown;
//    for (int i = 0; i < vlay->count(); i++){
//        vlay->itemAt(i)->widget()->hide();
//    }
    // show the pair for the current floor
//    vlay->itemAt(2*currFloor - 2)->widget()->show();
//    vlay->itemAt(2*currFloor - 1)->widget()->show();
//    qDebug() <<vlay->itemAt(2*currFloor - 2)->widget();
//    qDebug() <<vlay->itemAt(2*currFloor - 1)->widget();
}

// 2. current elev
void MainWindow::setCurrentElevator(){
    QComboBox* elevatorDropdown = qobject_cast<QComboBox*>(sender());
    int currElevator = elevatorDropdown->currentText().toInt();
    if (pax_inElev_map.contains(currElevator)){
        selectedPaxElev = pax_inElev_map.value(currElevator);
    }
    qDebug() << "Selected Elevator:" << currElevator<<"\n";
}
// 3. set current pax on floor
void MainWindow::setCurrentPax(){
    QComboBox* paxDropdown = qobject_cast<QComboBox*>(sender());
    int i = paxDropdown->currentIndex();
    selectedPax = pax_onFloor_vec.at(i);
    qDebug() << "Selected Pax on floor:" << selectedPax->getName()<<selectedPax->getWeight()<<"\n";
}

// 4. set current pax in elevator
void MainWindow::setCurrentPaxElev(){
    QComboBox* paxDropdown = qobject_cast<QComboBox*>(sender());
    int carNum = ui->comboBox_elevators->currentText().toInt();
    if (pax_inElev_map.contains(carNum)){
        selectedPaxElev = pax_inElev_map.value(carNum);
        qDebug() << "Selected Pax in elevator:" << selectedPaxElev->getName()<<selectedPaxElev->getWeight()<<"\n";
    }
}

/*
 * FOLLOWING SLOTS ARE FOR BUTTONS
 */

/* SLOT: Save Passenger on Click */
void MainWindow::on_savePassenger_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    int weight = ui->lineEdit_weight->text().toInt();
    if (name.isEmpty() || weight==0){
        qDebug() << "Error: inputs can not be empty and weight has to be integer! ";
        return;
    }
    // name should be unique
    for (int i=0; i<pax_onFloor_vec.size();i++){
        if (name.toLower() == pax_onFloor_vec.at(i)->getName().toLower()){
            qDebug() << "Error: name already exists! ";
            return;
        }
    }
    Passenger* p = new Passenger(name, weight);
    pax_onFloor_vec.push_back(p);
    // update widget
    ui->comboBox_passenger_floor->addItem(p->getName()+" "+ QString::number(p->getWeight())+" kg");
}


/*
 * SLOT: Car Call - Press Destination Button
 */
void MainWindow::pressDestBtn(){
    QPushButton* destBtn = qobject_cast<QPushButton*>(sender());
    if (destBtn){
        QString buttonText = destBtn->text();
        int destFloor = buttonText.toInt();
        int carNum = ui->comboBox_elevators->currentText().toInt();
        qDebug() << "+ Car call: destination Floor " << destFloor << ", from Elevator#"<<carNum<<"\n";
        ecs->addCarReq(carNum, destFloor);
        ecs->printCarReq();
    }
}

/*
 * SLOT: Floor Call - pushButton_up or pushButton_down on released
    In ECS, floor_requests is type of QMap<int, int>* , where key is floor, value is direction
        - No duplicate floor
        - UP = 1  DOWN = -1  BOTH = 0
 */

// UP = 1
void MainWindow::on_pushButton_up_released()
{
    if (selectedFl == NUM_FLOORS){
        qDebug() << "You can't go UP from the top floor!\n";
        return;
    }

    if (ecs->floorReq(selectedFl, 1)){
        ecs->printFloorReq();
    }
}
// DOWN = -1
void MainWindow::on_pushButton_down_released()
{
    if (selectedFl == 1){
        qDebug() << "You can't go DOWN from the bottom floor!\n";
        return;
    }
    if(ecs->floorReq(selectedFl, -1)){
        ecs->printFloorReq();
    }
}


// Embark
void MainWindow::on_pushButton_2_released()
{
    bool valid = false;
    QVector<Elevator*> elev_vec = ecs->getElevators();
    for (int i = 0; i < elev_vec.size(); i++){
       Elevator* elev = elev_vec.at(i);
       // check if passenger can embark or not
       if (elev->getStateName() == "ready" && elev->getCurrentFloor() == selectedFl){
           qDebug() << "Door is closed! ";
           return;
       }
       if (elev->getStateName() == "stop" && elev->getCurrentFloor() == selectedFl){
            valid = true;
            if (elev->updateCapacity(selectedPax->getWeight())){
                int index = pax_onFloor_vec.indexOf(selectedPax);
                qDebug()<<selectedPax->getName()<<index;
                if (index >= 0){
                    pax_onFloor_vec.remove(index);
                    int carNum = elev->getCarNumber();
                    pax_inElev_map.insert(carNum,selectedPax);
                    ui->comboBox_passenger_floor->blockSignals(true);
                    ui->comboBox_passenger_floor->clear();
                    ui->comboBox_passenger_floor->blockSignals(false);
                    ui->comboBox_passenger_elev->blockSignals(true);
                    ui->comboBox_passenger_elev->clear();
                    ui->comboBox_passenger_elev->blockSignals(false);
                    setUpPaxDropdown();
                }else{
                    qDebug()<<"Nobody is on the floor...\n";
                }
                return;
            }
       }
    }
    if (!valid){
        qDebug()<<"No elevator is stopped for service at the selected floor, you can't embark!";
    }
}

// Disembark
//     Passenger* selectedPax;
// QVector<Passenger*> pax_onFloor_vec;
// QMap<int, Passenger*> pax_inElev_map;
void MainWindow::on_pushButton_3_released()
{
    bool valid = false;
    QVector<Elevator*> elev_vec = ecs->getElevators();
    int currElevator = ui->comboBox_elevators->currentText().toInt();
    if (!pax_inElev_map.contains(currElevator)){
        qDebug()<<"No one is in the selected elevator, you can't disembark!";
        return;
    }
    Elevator* elev = elev_vec.at(currElevator);
    if (elev->getStateName() == "stop" && pax_inElev_map.value(currElevator) == selectedPaxElev){
        valid = true;
        if (elev->updateCapacity(-(selectedPaxElev->getWeight()))){
            pax_inElev_map.remove(currElevator);
            pax_onFloor_vec.append(selectedPaxElev);
            ui->comboBox_passenger_floor->blockSignals(true);
            ui->comboBox_passenger_floor->clear();
            ui->comboBox_passenger_floor->blockSignals(false);
            ui->comboBox_passenger_elev->blockSignals(true);
            ui->comboBox_passenger_elev->clear();
            ui->comboBox_passenger_elev->blockSignals(false);
            setUpPaxDropdown();
            return;
        }
    }

    if (!valid){
        qDebug()<<"Selected elevator is not stopped or door is closed, or no one is in the selected elevator, you can't disembark!";
    }

}


void MainWindow::on_pushButton_nextTen_released()
{
    ecs->nextTen();

}


void MainWindow::on_pushButton_print_clicked()
{
    ecs->printFloorReq();
    ecs->printCarReq();
    ecs->printCarDir();
}


void MainWindow::on_pushButton_power_toggled(bool checked)
{
    if(checked){
        qDeleteAll(pax_inElev_map);
        ecs->powerOut();
        qDebug()<<"All elevator stopped and emptied!\n";
    }
    else{
        ecs->isPowerOut = false;
        if (ecs->isFire == false){
            ecs->backToNormal();
        }
    }
}

void MainWindow::on_pushButton_fire_2_toggled(bool checked)
{
    if(checked){
        qDeleteAll(pax_inElev_map);
        ecs->buildingFire();
        qDebug()<<"All elevator stopped and emptied!\n";
    }
    else{
        ecs->isFire = false;
        if (ecs->isPowerOut == false){
            ecs->backToNormal();
        }
    }
}

void MainWindow::on_pushButton_obstacle_toggled(bool checked)
{
    int currElevator = ui->comboBox_elevators->currentText().toInt();
    if(checked){
        ecs->obstacle(currElevator);
    }
    else{
       ecs->getElevators().at(currElevator)->obstacle=false;
       ecs->backToNormalElev(currElevator);
    }
}

void MainWindow::on_pushButton_overweight_toggled(bool checked)
{
    int currElevator = ui->comboBox_elevators->currentText().toInt();
    if(checked){
        ecs->overload(currElevator);
    }
    else{
       ecs->getElevators().at(currElevator)->overload=false;
       ecs->backToNormalElev(currElevator);
    }
}

void MainWindow::on_pushButton_fire_toggled(bool checked)
{
    int currElevator = ui->comboBox_elevators->currentText().toInt();
    if(checked){
        ecs->elevFire(currElevator);
    }
    else{
       ecs->getElevators().at(currElevator)->fire=false;
       ecs->backToNormalElev(currElevator);
    }
}
