#include "passenger.h"

Passenger::Passenger(const QString& name, const int weight)
    :name(name), weight(weight){
    qDebug() << "Passenger ctor: "<< name << weight << "kg\n";
}
Passenger::~Passenger(){}

const QString& Passenger::getName(){ return name; }
int Passenger::getWeight(){ return weight; }
