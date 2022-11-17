#ifndef FLOOR_H
#define FLOOR_H
#include <QString>
#include <QDebug>

class Floor{

public:
    Passenger(const QString& name, const int weight);
    ~Passenger();

    QString getName();
    int getWeight(); // kg

private:
    int floorNumber; // kg
};

#endif // FLOOR_H
