#ifndef STRATEGYOP_H
#define STRATEGYOP_H
#include "strategy.h"
/*
    Strategy derived class, Elevator-Centered
*/
class StrategyOP: public Strategy{
  public:
    StrategyOP();
    virtual ~StrategyOP()=default;
    virtual void allocateElevator(int floorNumber,int direction, ECS* ecs, Elevator** elevator) override;

};


#endif // STRATEGYOP_H
