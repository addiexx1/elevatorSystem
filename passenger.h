#ifndef PASSENGER_H
#define PASSENGER_H
#include <QString>
#include <QDebug>

class Passenger{

public:
    Passenger(const QString& name, const int weight);
    ~Passenger();

    const QString& getName();
    int getWeight(); // kg

private:
    QString name;
    int weight; // kg
};

#endif // PASSENGER_H
