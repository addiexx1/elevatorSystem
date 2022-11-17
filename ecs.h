#ifndef ECS_H
#define ECS_H
#include "strategy.h"
#include "elevator.h"
#include "defs.h"
#include <QString>
#include <QDebug>
#include <QPair>
#include <QMap>
class Strategy;
class MainWindow;

/*  Elevator Control System has:
 *      currentStrat to allocate elevator based on the strategy
           - note: strategy can be changed in defs.h, and rebuild after the change
 *      elev_vec is a vector of all the elevator pointers
           - note:
 *      floor_requests map keeps those requests are not yet serviced,  <key=floor#, value=direction>, 1 being 'UP', -1 being 'DOWN', 0 being 'BOTH'
           - note: the map.keys() are sorted in c++
        unassigned_flReqs is a unassigned/unhandled floor requests, next 10 seconds will trigger the assignment again
        car_requests is a map to store car requests and the floor is assigned to visited (need this info to decide elevator's direction)
*/

class ECS{

public:
    ECS();
    ~ECS();
    bool isPowerOut = false;
    bool isFire = false;
    QVector<Elevator*> getElevators()const;
    QMap<int, int>* getFloorReqs() const;
    QMap<int, int>* getUnassignedFloorReqs() const;
    QMap<int, QSet<int>*>* getCarRequests() const;
    void setStrategy(Strategy* strategy);
    void addCarReq(int carNumber, int floorNumber);
    bool checkCarReq(int carNumber, int floorNumber);
    bool removeCarReq(int carNumber, int floorNumber);
    bool floorReq(int floorNumber, int direction);
    bool checkFlReq(int floorNumber, int direction);
    int removeFlReq(int floorNumber, int direction);
    void printFloorReq() const;
    void printCarReq() const;
    void printCarDir() const;
    void makeDecision(int floorNumber, int carNumber); // new floor decision
    void nextTen(); // what happens next 10 sec
    void backToNormal();
    void backToNormalElev(int carNum);
    void powerOut();
    void buildingFire();
    void elevFire(int carNum);
    void help(int carNum);
    void overload(int carNum);
    void obstacle(int carNum);
    void openDoor(int carNum); // door can be controlled only when elev is 'stopped' or 'ready to move'
    void closeDoor(int carNum); // door can be controlled only when elev is 'stopped' or 'ready to move'

private:
    Strategy* currentStrat;
    QVector<Elevator*> elev_vec;
    QMap<int, int>* floor_requests;// <key=floor#, value=direction>, 1 being 'UP', -1 being 'DOWN', 0 being 'BOTH'
    QMap<int, QSet<int>*>* car_requests; // <key=car#, value=a set of floors of car calls>, also stores floor to visit for each car, it determines the direction of elevator
    QMap<int, int>* unassigned_flReqs;  // no available elevator, next 10 seconds will trigger assignment again
    QMap<int, QString> dirToStr;
    QMap<int, QString> flDirToStr;


};

#endif // ECS_H
