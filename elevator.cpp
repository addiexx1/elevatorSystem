#include "elevator.h"
#include "elevatorState.h"
#include "stateReadyToMove.h"
#include "stateStop.h"
#include "stateMoving.h"

int Elevator::nextId = 0;
//ctor: init current state to 'ready to move'
Elevator::Elevator()
    :carNumber(nextId++),currentState(new StateReadyToMove(this))
{
    fsDue = new QMap<int, int>();
    dirToStr.insert(1,"UP");
    dirToStr.insert(0, "IDLE");
    dirToStr.insert(-1, "DOWN");
    qDebug() << "Elev #"<<carNumber<<"at floor:"<<currentFloor<<", current direction:"<<dirToStr[direction];
}

//dtor
Elevator::~Elevator(){
    delete currentState;
    delete fsDue;
}

// Current STATE object calls nextTen(), behaves differently based on the state
void Elevator::nextTen(){
    currentState->nextTen();
}

// getters
int Elevator::getCurrentFloor()const{
    return currentFloor;
}
int Elevator::getDirection()const{
    return direction;
}
int Elevator::getCarNumber()const{
    return carNumber;
}
int Elevator::getCapacity() const{
    return capacity;
}
QMap <int, int>* Elevator::getFsDue() const{
    return fsDue;
}
QString Elevator::getStateName() const{
    return currentState->getName();
}

// setters
void Elevator::setCurrentFloor(int floor){
    currentFloor = floor;
}
bool Elevator::updateCapacity(int weight){
    qDebug() << "Elevator#"<<carNumber<<"updating capacity....";
    if (capacity < weight){
        qDebug() << "Overweight: remaining capacity:"<<capacity<<"kg\n";
        return false;
    }
    capacity -= weight;
    qDebug() << "Updated: remaining capacity:"<<capacity<<"kg\n";
    return true;
}
void Elevator::setDirection(Direction dir){
    direction = dir;
}


void Elevator::moveNextFloor(){
    int nextFloor = currentFloor + direction;
    if (nextFloor>=1 && nextFloor<=NUM_FLOORS){
          setCurrentFloor(nextFloor);
    }
    // update display
    // decide whether to stop
}

void Elevator::stop(){
    changeState<StateStop>();
    qDebug() << " Elevator#" <<carNumber<<":"<<"Stops at floor"<<getCurrentFloor();
    qDebug() << " Elevator#" <<carNumber<<":"<<"Opens door";
    qDebug() << " Elevator#" <<carNumber<<":"<< "Rings bell";
    qDebug() << "Passenger can [embark]/[disembark] to update the capacity ...\n";
}
void Elevator::openDoor(){
    currentState->openDoor();
}
void Elevator::closeDoor(){
    currentState->closeDoor();
}
