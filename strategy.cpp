#include "strategy.h"

Strategy::Strategy(const QString& name):name(name){}


const QString& Strategy::getName(){
    return name;
}
