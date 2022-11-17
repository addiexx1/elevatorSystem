#ifndef READYTOMOVESTATE_H
#define READYTOMOVESTATE_H

#include "elevatorState.h"

/*  Derived class from ElevatorState
    - Elevator's ReadyToMove State:
        - Open/close door still works at the beginning of this state
        - Door Obstacle trigger btn NOT work
        - Overweight trigger btn NOT works

*/
class StateReadyToMove: public ElevatorState
{

public:
    StateReadyToMove(Elevator* elev);
    virtual ~StateReadyToMove();
    virtual void openDoor() override;
    virtual void closeDoor() override;
    virtual void nextTen() override;

private:
    //Elevator* elev;
};



#endif // READYTOMOVESTATE_H
