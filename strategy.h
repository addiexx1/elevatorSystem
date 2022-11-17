#ifndef STRATEGY_H
#define STRATEGY_H
#include "ecs.h"
#include "elevator.h"
class ECS;

/*
    Strategy base class
*/
class Strategy{
  public:
    Strategy(const QString& name);
    virtual ~Strategy()=default;
    virtual void allocateElevator(int floorNumber,int direction, ECS* ecs, Elevator** elevator)=0;
    //virtual void serviceDecision(ECS* ecs, int carNumber)=0;
    const QString& getName();

  protected:
    QString name;

};

#endif // STRATEGY_H
