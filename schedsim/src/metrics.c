#include <stdio.h>
#include <string.h>
#include "../include/process.h"

// STUB FILE - Myra will replace this with the real implementation

void calculate_metrics(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];
        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time    = p->turnaround_time - p->burst_time;
        p->response_time   = p->start_time - p->arrival_time;
    }
}

Metrics compute_averages(Process processes[], int n, const char *algorithm, int context_switches) {
    Metrics m = {0};
    strncpy(m.algorithm, algorithm, MAX_ALG_LEN - 1);
    m.context_switches = context_switches;
    for (int i = 0; i < n; i++) {
        m.avg_turnaround += processes[i].turnaround_time;
        m.avg_waiting    += processes[i].waiting_time;
        m.avg_response   += processes[i].response_time;
    }
    m.avg_turnaround /= n;
    m.avg_waiting    /= n;
    m.avg_response   /= n;
    return m;
}

void print_metrics_table(Process processes[], int n) {
    printf("=== Metrics ===\n");
    printf("%-8s | %-4s | %-4s | %-4s | %-4s | %-4s | %-4s\n",
           "Process", "AT", "BT", "FT", "TT", "WT", "RT");
    printf("---------|------|------|------|------|------|------\n");
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];
        printf("%-8s | %4d | %4d | %4d | %4d | %4d | %4d\n",
               p->pid, p->arrival_time, p->burst_time,
               p->finish_time, p->turnaround_time,
               p->waiting_time, p->response_time);
    }
    printf("\n");
}

void print_averages(Metrics *m) {
    printf("=== Averages ===\n");
    printf("Algorithm : %s\n", m->algorithm);
    printf("Avg TT    : %.2f\n", m->avg_turnaround);
    printf("Avg WT    : %.2f\n", m->avg_waiting);
    printf("Avg RT    : %.2f\n", m->avg_response);
    printf("\n");
}