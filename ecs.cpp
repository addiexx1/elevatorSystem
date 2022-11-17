#include "ecs.h"
#include "strategyLB.h"
#include "strategyOP.h"
#include "stateStop.h"
#include "stateReadyToMove.h"

// ** ECS ctor **
ECS::ECS(){
    qDebug() << "Initializing ECS ...";
    for (int i = 0; i < NUM_ELEVATORS; i++){
        elev_vec.push_back(new Elevator());
    }
    qDebug() << "- Elev directions: IDLE = 0, UP = 1, DOWN = -1";
    qDebug() << "- Floor Req directions: BOTH = 0, UP = 1, DOWN = -1";
    dirToStr.insert(1,"UP");
    dirToStr.insert(0, "IDLE");
    dirToStr.insert(-1, "DOWN");
    flDirToStr.insert(1,"UP");
    flDirToStr.insert(0, "UP&DOWN");
    flDirToStr.insert(-1, "DOWN");
    qDebug() << "# of Floors: "<< NUM_FLOORS;
    qDebug() << "# of Elevators: "<< NUM_ELEVATORS;
    qDebug() << "Max capacity: "<< MAX_WEIGHT << "KG";
    // set up Strategy
    if (strcmp(STRATEGY_NAME, "LB") == 0){
        currentStrat = new StrategyLB();
    }
    else if (strcmp(STRATEGY_NAME, "OP") == 0){
        currentStrat = new StrategyOP();
    }
    else{
        currentStrat = nullptr;
    }
    qDebug() << "Current Strategy: "<< currentStrat->getName() <<"\n\n";
    floor_requests = new QMap<int, int>();
    car_requests = new QMap<int, QSet<int>*>();
    unassigned_flReqs = new QMap<int, int>();
}
// ** ECS dtor **
ECS::~ECS(){
    for (int i = 0; i < NUM_ELEVATORS; i++){
        delete elev_vec[i];
    }
    elev_vec.clear();
    if(currentStrat) {
        delete currentStrat;
    }
    delete floor_requests;

    QMap<int, QSet<int>*>::iterator i;
    for (i = car_requests->begin(); i != car_requests->end(); ++i){
        delete i.value();
    }
    delete car_requests;
    delete unassigned_flReqs;
}

// getters
QVector<Elevator*> ECS::getElevators() const{
    return elev_vec;
}
QMap<int, QSet<int>*>* ECS::getCarRequests() const{
    return car_requests; // <car#, set of floors>
}
QMap<int, int>* ECS::getFloorReqs() const{
    return floor_requests;
}
QMap<int, int>* ECS::getUnassignedFloorReqs() const{
    return unassigned_flReqs;
}

// setters
void ECS::setStrategy(Strategy* strategy){
    currentStrat = strategy;
}

// QMap<int, QSet<int>*>* car_requests;
void ECS::addCarReq(int carNumber, int floorNumber){
    if (car_requests->keys().contains(carNumber)){
        car_requests->value(carNumber)->insert(floorNumber);
    }
    else{
        QSet<int>* floors = new QSet<int>();
        floors->insert(floorNumber);
        car_requests->insert(carNumber, floors);
    }
}
bool ECS::checkCarReq(int carNumber, int floorNumber){
    return (car_requests->contains(carNumber) && car_requests->value(carNumber)->contains(floorNumber));
}
// check and remove
bool ECS::removeCarReq(int carNumber, int floorNumber){
    if (checkCarReq(carNumber, floorNumber)){
        if (car_requests->value(carNumber)->size() == 1){
            car_requests->remove(carNumber);
            return true;
        }
        else{
            car_requests->value(carNumber)->remove(floorNumber);
            return true;
        }
    }
    return false;
}

// Floor request, returns true if success
bool ECS::floorReq(int floorNumber, int direction){

    // check duplicates and insert:
    if (floor_requests->contains(floorNumber)){
        // same floor and same direction,  duplicate request
        if (floor_requests->value(floorNumber) == direction || floor_requests->value(floorNumber) == 0){
            qDebug()<<"Duplicated request...\n";
            return false;
        }
        // same floor and diff direction, set value = 0 = BOTH
        else{
            (*floor_requests)[floorNumber] = 0;
        }
    }
    else{
        floor_requests->insert(floorNumber, direction);
    }
    // Stored the request sucessfully
    qDebug() <<"+ Floor request: floor"<< floorNumber<<"button" << dirToStr[direction] << "lights on ...\n";

    // first, try to see if there is an elavator on the same floor
    for (int i = 0; i < elev_vec.size();i++){
        Elevator* elev = elev_vec.at(i);
        if ((elev->getDirection()==Elevator::Direction::IDLE|| elev->getDirection() == direction)&& elev->outOfService == false && elev->getCurrentFloor() == floorNumber){
            elev->setDirection(static_cast<Elevator::Direction>(direction));
            makeDecision(floorNumber, i); //  ECS MAKE DECISION(), stop means servicing
            return true;
        }
    }
    // then, use allocation strategy
    Elevator* elev;
    currentStrat->allocateElevator(floorNumber,direction, this, &elev);
    // if elev is assigned, move it
    if (!elev){
        // unassigned floor requests, next 10 seconds will trigger trying to handle them again
        return false;
    }

    return true;

}
//   QMap<int, int>* floor_requests;
bool ECS::checkFlReq(int floorNumber, int direction){
    if (floor_requests->contains(floorNumber)){
        int dir = floor_requests->value(floorNumber);
        // direction == 0 means BOTH OF UP AND DOWN
        if (dir == direction || direction == 0){
            return true;
        }
    }
    return false;
}
// check and remove, removeFlReq() checks existence first
int ECS::removeFlReq(int floorNumber, int direction){

    if (checkFlReq(floorNumber,direction)){
        // if dir == 0, then left request's direction = (0 - direction) because UP = 1, DOWN = 0, BOTH = 0
        int dir = floor_requests->value(floorNumber);
        if (dir == 0){
            (*floor_requests)[floorNumber] = 0 - direction;
            return direction;
        }
        else{
            floor_requests->remove(floorNumber);
            return direction;
        }
    }
    return 0;
}

// *** MAKE DECISION for car to stop or not, update floor_requests and car_requests
void ECS::makeDecision(int floorNumber, int carNumber){
    int dir = elev_vec.at(carNumber)->getDirection();
    int isFlReq = removeFlReq(floorNumber, dir); // returns the removed direction
    bool isCarReq = removeCarReq(carNumber,floorNumber); // check and remove
    if (isFlReq != 0 || isCarReq){
        elev_vec.at(carNumber)->stop();
        if(isFlReq != 0){
            qDebug() << "- Elevator#"<<carNumber<<"servicing the floor"<<floorNumber<<": button"<<flDirToStr[isFlReq]<<"lights off.\n";
        }
        // update ui
    }
}

// *** each elevator calls nextTen() will update state and current floor, then ecs calls makeDecision() on the newFloor for each elev ***
void ECS::nextTen(){
    for (int i = 0 ; i < elev_vec.size(); i++){
         Elevator* elev = elev_vec.at(i);
        // First, decide if there is a need to update direction for this elevator
        // update to IDLE if there is no floor to visit for elevator
        // update to DOWN if no floor is higher than current floor
        // update to UP if no floor is lower than current floor
         int carNum = elev_vec.at(i)->getCarNumber();
         int currFloor = elev_vec.at(i)->getCurrentFloor();
         if (!car_requests->contains(carNum)){
             elev->setDirection(Elevator::IDLE);
         }
         else {
            int highestFl = 1;
            int lowestFl = NUM_FLOORS;
            foreach (int fl, *(car_requests->value(carNum))){
                highestFl = std::max(highestFl, fl);
                lowestFl = std::min(lowestFl, fl);
            }

            if (highestFl < currFloor || currFloor >= NUM_FLOORS){
                elev->setDirection(Elevator::DOWN);
            }
            else if (highestFl > currFloor || currFloor <= 1){
                elev->setDirection(Elevator::UP);
            }
            else if (lowestFl < currFloor){
                 elev->setDirection(Elevator::UP);
            }
            else{
                elev->setDirection(Elevator::DOWN);
            }

         }
        // Then, elev's direction set correctly, call nextTen();
        elev->nextTen();
        currFloor = elev_vec.at(i)->getCurrentFloor();
        carNum = elev_vec.at(i)->getCarNumber();
        makeDecision(currFloor, carNum);
    }
}


//Printing Fl Req DUE or Car Req DUE
void ECS::printFloorReq() const{
    qDebug()<< "***** PRINTING FLOOR REQUESTs DUE: *****";
    foreach (int f, floor_requests->keys()){
        qDebug() <<"[ floor"<<f << ", direction"<<flDirToStr[floor_requests->value(f)]<<"]";
    }
    qDebug()<<"\n";
}

void ECS::printCarReq() const{
    qDebug()<< "***** PRINTING FLOOR(s) NEED TO BE VISITED FOR EACH CAR: *****";
    foreach (int c, car_requests->keys()){
        QDebug dbg(QtDebugMsg);
        dbg <<"Elevator#"<< c <<": Floor [";
        foreach (int f, *(car_requests->value(c))){
            dbg << f;
        }
        dbg << "]";
    }
    qDebug()<<"\n";
}

void ECS::printCarDir() const{
    qDebug()<< "***** PRINTING ELEVATOR(s) DIRECTION: *****";
    for(int i=0; i<elev_vec.size();i++){
        qDebug()<< "Elevator#"<<elev_vec.at(i)->getCarNumber()<<"at floor"<<elev_vec.at(i)->getCurrentFloor()
                <<" direction"<<dirToStr[elev_vec.at(i)->getDirection()];
    }
    qDebug()<<"\n";
}

void ECS::backToNormal(){
    if(isPowerOut == false && isFire == false){
        for (int i = 0; i < elev_vec.size(); i++){
            elev_vec.at(i)->changeState<StateReadyToMove>();
            elev_vec.at(i)->outOfService = false;
        }
        qDebug()<<"Back to normal operation ...\n";
    }
}
void ECS::backToNormalElev(int carNum){
    Elevator* elev = elev_vec.at(carNum);
    if(elev->obstacle == false && elev->fire == false && elev->overload == false){
        elev->changeState<StateReadyToMove>();
        elev->outOfService = false;
        qDebug()<<"Elevator"<<carNum<<"back to normal operation ...\n";
    }
}
void ECS::powerOut(){
    isPowerOut = true;
    for (int i = 0; i < elev_vec.size(); i++){
        elev_vec.at(i)->changeState<StateStop>();
        elev_vec.at(i)->updateCapacity(MAX_WEIGHT);
        elev_vec.at(i)->outOfService = true;
    }
    qDebug()<<"ALRAM: Power Out!!!(Audio & Text) All elevator is moving to a save floor... ";
    qDebug()<<"ALRAM: Power Out!!!(Audio & Text) Disembark immediately ... ";
}
void ECS::buildingFire(){
    isPowerOut = true;
    for (int i = 0; i < elev_vec.size(); i++){
        elev_vec.at(i)->changeState<StateStop>();
        elev_vec.at(i)->updateCapacity(MAX_WEIGHT);
        elev_vec.at(i)->outOfService = true;
    }
    qDebug()<<"ALRAM: Building Fire!!!(Audio & Text) All elevator is moving to a save floor... ";
    qDebug()<<"ALRAM: Building Fire!!!(Audio & Text) Disembark immediately ... ";
}
void ECS::elevFire(int carNum){
    elev_vec.at(carNum)->changeState<StateStop>();
    elev_vec.at(carNum)->updateCapacity(MAX_WEIGHT);
    elev_vec.at(carNum)->fire = true;
    elev_vec.at(carNum)->outOfService = true;
    qDebug()<<"ALRAM: Elevator"<<carNum<<"Fire!!!(Audio & Text) Elevator is moving to a save floor... ";
    qDebug()<<"ALRAM: Elevator"<<carNum<< "Fire!!!(Audio & Text) Disembark immediately ... ";
}
void ECS::overload(int carNum){
    elev_vec.at(carNum)->changeState<StateStop>();
    elev_vec.at(carNum)->overload = true;
    elev_vec.at(carNum)->outOfService = true;
    qDebug()<<"ALRAM: Elevator"<<carNum<<"Overloaded!!!(Audio & Text) Please reduce the cargo load... ";
}
void ECS::obstacle(int carNum){
    elev_vec.at(carNum)->changeState<StateStop>();
    elev_vec.at(carNum)->obstacle = true;
    elev_vec.at(carNum)->outOfService = true;
    qDebug()<<"ALRAM: Elevator"<<carNum<<"Door Obstacle!!!(Audio & Text) Please clear the doorway ... ";
}
void ECS::help(int carNum){
    elev_vec.at(carNum)->changeState<StateStop>();
    qDebug()<<"ALRAM: Elevator"<<carNum<<"Help!!!(Audio & Text) Please respond to the building safety... ";
    qDebug()<<"ALRAM: Elevator"<<carNum<<"Help!!!(Audio & Text) No response, calling 911 ... ";
}
void ECS::openDoor(int carNum){
    elev_vec.at(carNum)->openDoor();
}
void ECS::closeDoor(int carNum){
    elev_vec.at(carNum)->closeDoor();
}


