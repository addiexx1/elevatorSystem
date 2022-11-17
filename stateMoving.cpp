#include "stateMoving.h"
#include "stateStop.h"

StateMoving::StateMoving(Elevator* elev):ElevatorState(elev, "moving")
{}

StateMoving::~StateMoving()
{
    delete elev;
}
void StateMoving::openDoor(){
    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"is moving: You can't open the door!";
}
void StateMoving::closeDoor(){
    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"is moving: Door is closed already!";
}
void StateMoving::nextTen(){
    elev->moveNextFloor();
    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"moves to next floor# ="<< elev->getCurrentFloor()<<", direction ="<< dirToStr[elev->getDirection()]<<"\n";
}

// door open close not working
