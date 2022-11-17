#ifndef ELEVATOR_H
#define ELEVATOR_H
#include <iostream>
#include <QString>
#include <QDebug>
#include "elevatorState.h"
#include "defs.h"

class ElevatorState;

class Elevator{

  public:
        //constructor
        Elevator();
        //destructor
        ~Elevator();

        enum Direction {
            UP = 1, // next job going up
            DOWN = -1,  // next job going down
            IDLE = 0    // no job
        };
        bool obstacle = false;
        bool overload = false;
        bool fire = false;
        bool outOfService = false; //  building emergency, stuck for long time will trigger, out of service behaves as stateStop, but cannot accept new service

    // getters and setters
    int getDirection()const;
    int getCarNumber()const;
    int getCurrentFloor()const;
    int getCapacity() const;
    QString getStateName() const;
    QMap <int, int>* getFsDue() const;
    void setDirection(Direction dir);
    void setCurrentFloor(int floor);
    bool updateCapacity(int weight);
    void moveNextFloor(); // depends on the current direction,current floor += Direction
    void stop();
    void openDoor();
    void closeDoor();

    // *** IMPORTANT FUNCTION: Polymorphism based on the state, nextTen = what happens after ten seconds
    void nextTen();

    // others
//    void moveToSafe();
//    void newFloor(int floorNumber);
//    void move(int floorNumber);
//    void stop();
//    void help();
//    void obstacleWarning();
//    void fireAlram();
//    void powerOut();
//    void overload();

    // change the current state
    template <class T>
    void changeState();


  private:
    static int nextId;
    int carNumber; // = nextId, starts from 0
    int currentFloor = 1;
    Direction direction = IDLE;
    ElevatorState* currentState; // init StateReadyToMove
    int capacity = MAX_WEIGHT;
    QMap <int, int>* fsDue; // TODO update, no need floor services due, key=floor, value = direction = 1 being 'UP', -1 being 'DOWN', 0 being 'BOTH'
    QVector <int> ccDue; // car calls due, needed for diff strategy and turn on/off the dest button
    QMap<int, QString> dirToStr;

    //ECS* ecs;   //pointer to its ecs
    //Floor** serviceList; // floors to be serviced
//    // door
//    Door door;
//    // sensors
//    PositionSensor ps;
//    WeightSensor ws;
//    // button controls
//    HelpButton hb;
//    DestButton** dbtns;
//    FireButton fb;
//    DoorButton db2[2];
//    // audio and display
//    Audio audio;
//    Display display;

};

template <class T>
void Elevator::changeState(){

    currentState = new T(this);
    //qDebug() << "Change state to"<< currentState->getName();
}

#endif // ELEVATOR_H
