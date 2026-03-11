#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/*
 * sort_by_arrival -> sorts processes by arrival time (bubble sort)
 * Tie-break: alphabetical by PID
 */
void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int swap = 0;
            if (processes[j].arrival_time < processes[i].arrival_time) {
                swap = 1;
            } else if (processes[j].arrival_time == processes[i].arrival_time &&
                       strcmp(processes[j].pid, processes[i].pid) < 0) {
                swap = 1;
            }
            if (swap) {
                Process temp  = processes[i];
                processes[i]  = processes[j];
                processes[j]  = temp;
            }
        }
    }
}

/*
 * sort_by_burst -> sorts processes by burst time (bubble sort)
 * Tie-break: earlier arrival first
 */
void sort_by_burst(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int swap = 0;
            if (processes[j].burst_time < processes[i].burst_time) {
                swap = 1;
            } else if (processes[j].burst_time == processes[i].burst_time &&
                       processes[j].arrival_time < processes[i].arrival_time) {
                swap = 1;
            }
            if (swap) {
                Process temp  = processes[i];
                processes[i]  = processes[j];
                processes[j]  = temp;
            }
        }
    }
}

/*
 * find_min_remaining -> returns index of process with shortest remaining time
 * Only considers processes that have arrived by current_time
 * Returns -1 if no process is available
 */
int find_min_remaining(Process processes[], bool completed[], int n, int current_time) {
    int min_idx = -1;

    for (int i = 0; i < n; i++) {
        if (completed[i]) continue;
        if (processes[i].arrival_time > current_time) continue;

        if (min_idx == -1 ||
            processes[i].remaining_time < processes[min_idx].remaining_time ||
            (processes[i].remaining_time == processes[min_idx].remaining_time &&
             processes[i].arrival_time   <  processes[min_idx].arrival_time)) {
            min_idx = i;
        }
    }
    return min_idx;
}

/*
 * all_complete -> returns true if all processes are done
 */
bool all_complete(bool completed[], int n) {
    for (int i = 0; i < n; i++) {
        if (!completed[i]) return false;
    }
    return true;
}

/*
 * print_separator -> prints a divider line of given length
 */
void print_separator(int len) {
    for (int i = 0; i < len; i++) printf("-");
    printf("\n");
}