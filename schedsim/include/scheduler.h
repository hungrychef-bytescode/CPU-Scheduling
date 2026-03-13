#ifndef SCHEDULER_H             //guard start
#define SCHEDULER_H

#define MAX_PROCESSES 256
#define MAX_SIMULATION_TIME 1000

#include "process.h"

//enumerate scheduling algorithms
//used to determine which algo to execute.
typedef enum {
    FCFS,
    SJF,
    STCF,
    RR,
    MLFQ,
    COMPARE
} SchedulingAlgorithm;

typedef struct {
    Process *processes;         //arr of all processes
    int num_processes;          //no. of processes
    int current_time;           //curr simulation time
    Process *current_process;   //track current running process

    //add later depending on what's neededF
} SchedulerState; 

int schedule_fcfs(SchedulerState *state);
int schedule_sjf(SchedulerState *state);
int schedule_stcf(SchedulerState *state);
int schedule_rr(SchedulerState *state);
int schedule_mlfq(SchedulerState *state);

#endif //guard end