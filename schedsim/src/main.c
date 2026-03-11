#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/*
 * reset_processes -> copies original process data into a fresh array
 * needed before each algorithm run so state is not carried over
 */
static void reset_processes(Process original[], Process copy[], int n) {
    for (int i = 0; i < n; i++) {
        copy[i] = original[i];
        copy[i].remaining_time = original[i].burst_time;
        copy[i].start_time     = -1;
        copy[i].finish_time    = 0;
        copy[i].waiting_time   = 0;
        copy[i].turnaround_time = 0;
        copy[i].response_time  = 0;
        copy[i].priority       = 0;
        copy[i].time_in_queue  = 0;
    }
}

int main(int argc, char *argv[]) {

    // ── 1. Parse command-line arguments ──────────────────────────────────────
    Config config = parse_arguments(argc, argv);

    // ── 2. Load processes from file or inline string ──────────────────────────
    Process original[MAX_PROCESSES];
    int n = 0;

    if (config.input_file[0] != '\0') {
        n = parse_workload_file(config.input_file, original, MAX_PROCESSES);
    } else {
        n = parse_processes_string(config.processes_str, original, MAX_PROCESSES);
    }

    printf("Loaded %d process(es).\n\n", n);

    // Working copy so original is preserved for multiple runs
    Process processes[MAX_PROCESSES];

    // ── 3. Dispatch to algorithm(s) ───────────────────────────────────────────
    if (config.compare_mode) {
        // Run all algorithms and collect metrics for comparison
        Metrics results[5];
        int     count = 0;

        reset_processes(original, processes, n);
        results[count++] = schedule_fcfs(processes, n);

        reset_processes(original, processes, n);
        results[count++] = schedule_sjf(processes, n);

        reset_processes(original, processes, n);
        results[count++] = schedule_stcf(processes, n);

        reset_processes(original, processes, n);
        results[count++] = schedule_rr(processes, n, config.quantum);

        reset_processes(original, processes, n);
        results[count++] = schedule_mlfq(processes, n, config.mlfq_config_file);

        // Print comparison table
        printf("\n=== Algorithm Comparison ===\n\n");
        printf("%-10s | %8s | %8s | %8s | %s\n",
               "Algorithm", "Avg TT", "Avg WT", "Avg RT", "Context Switches");
        printf("-----------|----------|----------|----------|------------------\n");
        for (int i = 0; i < count; i++) {
            printf("%-10s | %8.1f | %8.1f | %8.1f | %d\n",
                   results[i].algorithm,
                   results[i].avg_turnaround,
                   results[i].avg_waiting,
                   results[i].avg_response,
                   results[i].context_switches);
        }
        printf("\n");

    } else {
        // Run single algorithm
        reset_processes(original, processes, n);

        if (strcmp(config.algorithm, "FCFS") == 0) {
            schedule_fcfs(processes, n);

        } else if (strcmp(config.algorithm, "SJF") == 0) {
            schedule_sjf(processes, n);

        } else if (strcmp(config.algorithm, "STCF") == 0) {
            schedule_stcf(processes, n);

        } else if (strcmp(config.algorithm, "RR") == 0) {
            schedule_rr(processes, n, config.quantum);

        } else if (strcmp(config.algorithm, "MLFQ") == 0) {
            schedule_mlfq(processes, n, config.mlfq_config_file);

        } else {
            fprintf(stderr, "Error: unknown algorithm '%s'\n", config.algorithm);
            fprintf(stderr, "Valid options: FCFS, SJF, STCF, RR, MLFQ\n");
            return 1;
        }
    }

    return 0;
}