#ifndef ELEVATORSTATE_H
#define ELEVATORSTATE_H

#include "elevator.h"


/*
    Elevator State Base class (Abstract class for polymorphism) has 3 states:
        1. state Moving (Normal moving operation)
        2. state Ready To Move (Not moving yet, door is closed, and ready to move when requires)
        3. state Stop (AKA not ready to move, stopped for servicing, obstacle, overweight, etc.)
*/
class Elevator;

class ElevatorState
{

public:
    ElevatorState(Elevator* elev, QString name);
    virtual ~ElevatorState();
    virtual void openDoor() = 0;
    virtual void closeDoor() = 0;
    virtual void nextTen() = 0; // handles what happen after 10 seconds based on the elevator state
    const QString& getName();

protected:
    Elevator* elev;
    QString name;
    QMap<int, QString> dirToStr;
};

#endif // ELEVATORSTATE_H
