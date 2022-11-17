#include "elevatorState.h"


ElevatorState::ElevatorState(Elevator* elev, QString name):elev(elev), name(name)
{
    dirToStr.insert(1,"UP");
    dirToStr.insert(0, "IDLE");
    dirToStr.insert(-1, "DOWN");
}

ElevatorState::~ElevatorState(){
    // virtual dtor
}
const QString& ElevatorState::getName(){
    return name;
}
