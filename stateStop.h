#ifndef STOPSTATE_H
#define STOPSTATE_H
#include "elevatorState.h"

/*  Derived class from ElevatorState (stop state = not ready to move)
    - Elevator's Stop State include: stop for servicing, stuck for obstacle, overweight...
        - Open/close door btns work
        - Door Obstacle trigger btn works
        - Overweight trigger btn works
        - passenger cam disembark
*/
class StateStop: public ElevatorState
{

public:
    StateStop(Elevator* elev);
    virtual ~StateStop();
    virtual void openDoor() override;
    virtual void closeDoor() override;
    virtual void nextTen() override;

private:
    //Elevator* elev;
};

#endif // STOPSTATE_H
