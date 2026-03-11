#ifndef PROCESS_H       //header guard
#define PROCESS_H

#define MAX_PID_LENGTH 16

//data struct for a process
typedef struct {
    char pid[MAX_PID_LENGTH];                   //process identifier
    int arrival_time;               
    int burst_time;                 //total time needed
    int remaining_time;             
    int start_time;                 //records first execution -> for Response Time calculation
    int finish_time;                //record completion time
    int waiting_time;               

    //for MLFQ
    int priority;
    int time_in_queue;
} Process;

#endif