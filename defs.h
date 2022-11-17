
#ifndef DEFS_H
#define DEFS_H


/*
   This defs header file is essentially an <Admin UI>, where you can change:
       - the number of elevators
       - the number of floors
       - the max weight in kg
       - the allocation strategy : 1. "LB", 2. "OP" for elevator-centered

     Just rebuild the project when the desired changes are done
*/


#define NUM_FLOORS 12
#define NUM_ELEVATORS 3
#define MAX_WEIGHT 1000
#define STRATEGY_NAME "LB"

#endif // DEFS_H
