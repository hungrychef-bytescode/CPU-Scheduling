#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "scheduler.h"

typedef enum {
    EVENT_ARRIVAL,
    EVENT_COMPLETION,
    EVENT_QUANTUM_EXPIRE,
    EVENT_PRIORITY_BOOST
} EventType;

typedef struct Event {
    int time;                           //for arrival time
    EventType type;                     //type-> if what kind of event
    Process *process;                   //entire process
    struct Event *next                  //points to the next event
} Event;

void simulator(SchedulerState *state, SchedulingAlgorithm algorithm);
Event* initialize_events(SchedulerState *state);

#endif