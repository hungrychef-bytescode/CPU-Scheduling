#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

#define MAX_PID_LENGTH  16
#define MAX_GANTT       1100
#define MAX_PROCESSES   256
#define MAX_ALG_LEN     16

typedef struct {
    char pid[MAX_PID_LENGTH];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int response_time;

    // for MLFQ
    int priority;
    int time_in_queue;
} Process;

typedef struct {
    char algorithm[MAX_ALG_LEN];
    double avg_turnaround;
    double avg_waiting;
    double avg_response;
    int context_switches;
} Metrics;

// metrics.c
void    calculate_metrics(Process processes[], int n);
Metrics compute_averages(Process processes[], int n, const char *algorithm, int context_switches);
void    print_metrics_table(Process processes[], int n);
void    print_averages(Metrics *m);

// gantt.c
void gantt_record(char gantt[], int time, const char *pid);
void gantt_print(char gantt[], int total_time);

#endif