#include "stateReadyToMove.h"
#include "stateMoving.h"
#include "stateStop.h"

StateReadyToMove::StateReadyToMove(Elevator* elev):ElevatorState(elev, "ready")
{}

StateReadyToMove::~StateReadyToMove()
{
    delete elev;
}

void StateReadyToMove::openDoor(){
    // change to state stop, door will open
    elev->changeState<StateStop>();
    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"stopped: Door is open now!";
}
void StateReadyToMove::closeDoor(){

    qDebug() << "-> Elevator#"<<elev->getCarNumber()<<"ready to move: Door is already closed!";
}
void StateReadyToMove::nextTen(){
    if (elev->getDirection() != Elevator::Direction::IDLE){
        elev->changeState<StateMoving>();
        elev->moveNextFloor();
        qDebug() << "-> Elevator#"<< elev->getCarNumber()<< "changed from ReadyToMove to Moving, moves to next floor# ="<< elev->getCurrentFloor()
                 << ", direction ="<< dirToStr[elev->getDirection()]<<"\n";
    }
}

// door open/close still works
