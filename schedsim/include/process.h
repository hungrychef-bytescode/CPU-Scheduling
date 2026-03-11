#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

// ─── Constants ───────────────────────────────────────────────────────────────
#define MAX_PROCESSES   100
#define MAX_ALG_LEN     16
#define MAX_PATH_LEN    256
#define MAX_PROC_STR    1024
#define MAX_GANTT       10000

// ─── Process Struct ──────────────────────────────────────────────────────────
/*
 * Represents a single process in the simulation.
 * Used by all scheduling algorithms.
 */
typedef struct {
    char pid[16];           // Process identifier (e.g. "A", "B", "P1")
    int  arrival_time;      // When process enters the ready queue
    int  burst_time;        // Total CPU time required
    int  remaining_time;    // Remaining CPU time (for preemptive algorithms)
    int  start_time;        // When process first got the CPU (for response time)
    int  finish_time;       // When process completed (for turnaround time)
    int  waiting_time;      // Total time spent waiting in ready queue
    int  turnaround_time;   // finish_time - arrival_time
    int  response_time;     // start_time - arrival_time

    // MLFQ fields
    int  priority;          // Current queue level (0 = highest)
    int  time_in_queue;     // Time accumulated in current queue level
} Process;

// ─── Config Struct ───────────────────────────────────────────────────────────
/*
 * Holds all parsed command-line arguments for the simulator.
 * Built by parse_arguments() in parser.c
 */
typedef struct {
    char algorithm[MAX_ALG_LEN];        // "FCFS", "SJF", "STCF", "RR", "MLFQ"
    char input_file[MAX_PATH_LEN];      // Path to workload input file
    char processes_str[MAX_PROC_STR];   // Inline processes "A:0:240,B:10:180"
    char mlfq_config_file[MAX_PATH_LEN];// Path to MLFQ config file
    int  quantum;                       // Time quantum for RR (default: 30)
    bool compare_mode;                  // true if --compare flag is set
} Config;

// ─── Metrics Struct ──────────────────────────────────────────────────────────
/*
 * Holds average metrics after simulation completes.
 * Used for comparison across algorithms.
 */
typedef struct {
    char algorithm[MAX_ALG_LEN];
    double avg_turnaround;
    double avg_waiting;
    double avg_response;
    int    context_switches;
} Metrics;

// ─── Function Prototypes: parser.c ───────────────────────────────────────────
Config  parse_arguments(int argc, char *argv[]);
int     parse_workload_file(const char *filename, Process processes[], int max_processes);
int     parse_processes_string(const char *str, Process processes[], int max_processes);

// ─── Function Prototypes: metrics.c (Myra) ───────────────────────────────────
void    calculate_metrics(Process processes[], int n);
Metrics compute_averages(Process processes[], int n, const char *algorithm, int context_switches);
void    print_metrics_table(Process processes[], int n);
void    print_averages(Metrics *m);

// ─── Function Prototypes: gantt.c ────────────────────────────────────────────
void    gantt_record(char gantt[], int time, const char *pid);
void    gantt_print(char gantt[], int total_time);

// ─── Function Prototypes: fcfs.c ─────────────────────────────────────────────
Metrics schedule_fcfs(Process processes[], int n);

// ─── Function Prototypes: sjf.c ──────────────────────────────────────────────
Metrics schedule_sjf(Process processes[], int n);

// ─── Function Prototypes: stcf.c ──────────────────────────────────────
Metrics schedule_stcf(Process processes[], int n);

// ─── Function Prototypes: rr.c ────────────────────────────────────────
Metrics schedule_rr(Process processes[], int n, int quantum);

// ─── Function Prototypes: mlfq.c ─────────────────────────────────────────────
Metrics schedule_mlfq(Process processes[], int n, const char *config_file);

#endif // PROCESS_H