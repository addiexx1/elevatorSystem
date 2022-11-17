#ifndef MOVINGSTATE_H
#define MOVINGSTATE_H
#include "elevatorState.h"

/*  Derived class from ElevatorState
    - Elevator's Moving State
        - Open/close door NOT work
        - Door Obstacle trigger btn NOT works
        - Overweight trigger btn NOT works
*/
class StateMoving: public ElevatorState
{

public:
    StateMoving(Elevator* elev);
    virtual ~StateMoving();
    virtual void openDoor() override;
    virtual void closeDoor() override;
    virtual void nextTen() override;

private:

};

#endif // MOVINGSTATE_H
