#include "stateStop.h"
#include "stateReadyToMove.h"


StateStop::StateStop(Elevator* elev):ElevatorState(elev, "stop")
{}

StateStop::~StateStop(){
    delete elev;
}

void StateStop::openDoor(){
    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"stopped: Door is already open!";
}
void StateStop::closeDoor(){
    // change to State ReadyToMove, door will close
    elev->changeState<StateReadyToMove>();
    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"ready to move: Door is closed!";
}
void StateStop::nextTen(){
    // if everything ok, no obstacle , no overweight...
    if (elev->outOfService == false){
        elev->changeState<StateReadyToMove>();
        qDebug() << "-> Elevator#"<< elev->getCarNumber()<< "door closed, changed state from Stop to ReadyToMove"
                 << ", direction ="<< dirToStr[elev->getDirection()]<<"\n";
    }
}
