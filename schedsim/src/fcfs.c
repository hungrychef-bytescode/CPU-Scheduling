#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/*
 * schedule_fcfs -> Non-preemptive First-Come First-Served scheduling
 * Processes execute in order of arrival time.
 * Once a process starts, it runs to completion.
 */
Metrics schedule_fcfs(Process processes[], int n) {
    printf("Running FCFS Scheduler...\n\n");

    // ── 1. Sort processes by arrival time ─────────────────────────────────────
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[j].arrival_time < processes[i].arrival_time) {
                Process temp  = processes[i];
                processes[i]  = processes[j];
                processes[j]  = temp;
            }
        }
    }

    // ── 2. Simulate FCFS ──────────────────────────────────────────────────────
    char gantt[MAX_GANTT];
    memset(gantt, '.', sizeof(gantt));
    gantt[MAX_GANTT - 1] = '\0';

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        // If CPU is idle, jump to next arrival
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        // Record start time (for response time)
        processes[i].start_time = current_time;

        // Run process to completion
        for (int t = 0; t < processes[i].burst_time; t++) {
            gantt_record(gantt, current_time + t, processes[i].pid);
        }

        current_time += processes[i].burst_time;

        // Record finish time
        processes[i].finish_time = current_time;
    }

    // ── 3. Calculate metrics ──────────────────────────────────────────────────
    calculate_metrics(processes, n);

    // ── 4. Print Gantt chart ──────────────────────────────────────────────────
    gantt_print(gantt, current_time);

    // ── 5. Print metrics table ────────────────────────────────────────────────
    print_metrics_table(processes, n);

    // ── 6. Detect and warn about convoy effect ────────────────────────────────
    for (int i = 1; i < n; i++) {
        if (processes[i].waiting_time > processes[i].burst_time * 2) {
            printf("Convoy effect detected: Process %s waited %d time units\n",
                   processes[i].pid, processes[i].waiting_time);
        }
    }

    // ── 7. Return averages ────────────────────────────────────────────────────
    Metrics m = compute_averages(processes, n, "FCFS", 0);
    print_averages(&m);
    return m;
}