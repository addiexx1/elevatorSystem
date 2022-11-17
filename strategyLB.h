#ifndef STRATEGYLB_H
#define STRATEGYLB_H
#include "strategy.h"
/*
    Strategy derived class, LB = LOAD BALANCING
*/
class StrategyLB: public Strategy{
  public:
    StrategyLB();
    virtual ~StrategyLB()=default;
    virtual void allocateElevator(int floorNumber,int direction, ECS* ecs, Elevator** elevator) override;
    int timeElev(Elevator* elev, int floorNumber, int direction); // time each elevator travel and stop time to get to the desired floor
};

#endif // STRATEGYLB_H
