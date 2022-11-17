#include "strategyOP.h"

StrategyOP::StrategyOP():Strategy("OP"){
    // Opportunistic Strategy: no early decision, elevator itseslf doesn't know when to stop
}

// floor request - just need to decide if there is need to move an idle or wait for an moving elevator to pass by
void StrategyOP::allocateElevator(int floorNumber,int direction, ECS* ecs, Elevator** elevator){

    // Minimum Requirment for a passing-by elevator: the elevator is not out of service and currently has 100kg capacity
    QVector <Elevator*> elev_vec = ecs->getElevators();
    for (int i = 0; i < elev_vec.size(); i++){
        Elevator* elev = elev_vec.at(i);
        // check if Minimum Requirment is met
        if (elev->getCapacity()>=100 && elev->outOfService == false){
            // check if the floor request is on the way of this elev's moving direction, and floor req direction = moving direction
            if (elev->getDirection()==Elevator::UP && floorNumber >= elev->getCurrentFloor() && direction == 1){
                int carNum = elev->getCarNumber();
                ecs->addCarReq(carNum, floorNumber);
                *elevator = elev;
                return;
            }
            else if (elev->getDirection()==Elevator::DOWN && floorNumber <= elev->getCurrentFloor() && direction == -1){
                int carNum = elev->getCarNumber();
                ecs->addCarReq(carNum, floorNumber);
                *elevator = elev;
                return;
            }
        }
    }
    // no eleavtor can pass by, then move idle
    for (int i = 0; i < elev_vec.size(); i++){
        Elevator* elev = elev_vec.at(i);
        if (elev->getDirection()==Elevator::IDLE && elev->outOfService == false){
            if (elev->getCurrentFloor() < floorNumber){
                elev->setDirection(Elevator::UP);
            }
            else if (elev->getCurrentFloor() > floorNumber){
                elev->setDirection(Elevator::DOWN);
            }
            int carNum = elev->getCarNumber();
            ecs->addCarReq(carNum, floorNumber);
            *elevator = elev;
            return;
        }
    }
    // if no idle, no passing-by elevator, add this request to unassigned floor request list that ecs will try later
    ecs->getUnassignedFloorReqs()->insert(floorNumber, direction);
    qDebug() << "No available elevator, please wait (keep pressing next ten seconds button and ecs will try again!)";
}
