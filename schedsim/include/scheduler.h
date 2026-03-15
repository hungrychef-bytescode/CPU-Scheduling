#ifndef SCHEDULER_H             //guard start
#define SCHEDULER_H

#define MAX_PROCESSES        256
#define MAX_SIMULATION_TIME  1000
#define NUM_ALGORITHMS       5      // FCFS, SJF, STCF, RR, MLFQ

#include "../include/process.h"
#include "../include/parser.h"     

// SchedulerState is available for algos that need it, but not required
typedef struct {
    Process *processes;         // arr of all processes
    int num_processes;          // no. of processes
    int current_time;           // curr simulation time
    Process *current_process;   // track current running process
} SchedulerState;

Metrics schedule_fcfs(Process processes[], int n);
Metrics schedule_sjf(Process processes[], int n);
Metrics schedule_stcf(Process processes[], int n);
Metrics schedule_rr(Process processes[], int n, int quantum);
Metrics schedule_mlfq(Process processes[], int n, const char *config_file);

void run_all(const Process original[], int n, CLIOptions *option);

#endif //guard end