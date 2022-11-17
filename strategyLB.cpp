#include "strategyLB.h"

StrategyLB::StrategyLB():Strategy("LB"){

}

/*
    LB Strategy: load-balancing (smarter version):
        // Minimum Requirment: the elevator is not out of service and currently has 100kg capacity
        // Select the IDLE with the shortest distance first
        // If there is no IDLE, assign to the elevator with the least jobs
*/
void StrategyLB::allocateElevator(int floorNumber,int direction, ECS* ecs, Elevator** elevator){
    QVector <Elevator*> elev_vec = ecs->getElevators();
    int min_distance = NUM_FLOORS;
    for (int i = 0; i < elev_vec.size(); i++){
        Elevator* elev = elev_vec.at(i);
        // check for IDLE, and select with the min distance to requesting floor
        if (elev->getDirection() == Elevator::IDLE && abs(elev->getCurrentFloor() - floorNumber) < min_distance){
            min_distance = abs(elev->getCurrentFloor() - floorNumber);
            *elevator = elev;
        }
    }
    if (*elevator){
        ecs->addCarReq((*elevator)->getCarNumber(), floorNumber);
        return;
    }
    // If there is no IDLE, assign to the elevator with the least jobs(amount of floors need to visit)
    int numJobs = 10; // job limit
    for (int i = 0; i < elev_vec.size(); i++){
        Elevator* elev = elev_vec.at(i);
        // check if Minimum Requirment is met
        if (elev->getCapacity()>=100 && elev->outOfService == false){
            int carNum = elev->getCarNumber();
            if (ecs->getCarRequests()->contains(carNum) && (ecs->getCarRequests()->value(carNum)->size() < numJobs)){
                *elevator = elev;
            }
        }
    }
    if (*elevator){
        ecs->addCarReq((*elevator)->getCarNumber(), floorNumber);
        return;
    }

    // if no available elevator, add this request to unassigned floor request
    ecs->getUnassignedFloorReqs()->insert(floorNumber, direction);
    qDebug() << "No available elevator, please wait (keep pressing next ten seconds button and ecs will try again!)";
}

