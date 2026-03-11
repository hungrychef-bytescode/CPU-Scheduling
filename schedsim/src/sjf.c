#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/*
 * schedule_sjf -> Non-preemptive Shortest Job First scheduling
 * From all arrived processes, picks the one with the shortest burst time.
 * Once selected, runs to completion.
 */
Metrics schedule_sjf(Process processes[], int n) {
    printf("Running SJF Scheduler...\n\n");

    // ── 1. Track which processes have been completed ───────────────────────────
    bool completed[MAX_PROCESSES] = {false};

    char gantt[MAX_GANTT];
    memset(gantt, '.', sizeof(gantt));
    gantt[MAX_GANTT - 1] = '\0';

    int current_time  = 0;
    int done          = 0;

    // ── 2. Simulate SJF ───────────────────────────────────────────────────────
    while (done < n) {
        // Find shortest available job that has arrived
        int shortest = -1;

        for (int i = 0; i < n; i++) {
            if (completed[i]) continue;
            if (processes[i].arrival_time > current_time) continue;

            if (shortest == -1 ||
                processes[i].burst_time < processes[shortest].burst_time ||
                // Tie-break: earlier arrival first
                (processes[i].burst_time == processes[shortest].burst_time &&
                 processes[i].arrival_time < processes[shortest].arrival_time)) {
                shortest = i;
            }
        }

        // No process available yet — advance time to next arrival
        if (shortest == -1) {
            current_time++;
            continue;
        }

        // Record start time
        processes[shortest].start_time = current_time;

        // Run to completion
        for (int t = 0; t < processes[shortest].burst_time; t++) {
            gantt_record(gantt, current_time + t, processes[shortest].pid);
        }

        current_time += processes[shortest].burst_time;
        processes[shortest].finish_time = current_time;
        completed[shortest] = true;
        done++;
    }

    // ── 3. Calculate metrics ──────────────────────────────────────────────────
    calculate_metrics(processes, n);

    // ── 4. Print Gantt chart and metrics ─────────────────────────────────────
    gantt_print(gantt, current_time);
    print_metrics_table(processes, n);

    // ── 5. Return averages ────────────────────────────────────────────────────
    Metrics m = compute_averages(processes, n, "SJF", 0);
    print_averages(&m);
    return m;
}