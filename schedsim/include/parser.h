#ifndef PARSER_H                //header guard
#define PARSER_H

#include <stdbool.h>
#include "process.h"

#define MAX_FILENAME        256
#define MAX_PROCESS_STR     1000
#define MAX_MLFQ_QUEUES     8
#define MLFQ_DEFAULT_QUEUES 3
#define MLFQ_DEFAULT_BASE_QUANTUM 8

// scheduling algorithm enum lives here so both parser and scheduler can use it
// without either including the other
typedef enum {
    NONE,
    FCFS,
    SJF,
    STCF,
    RR,
    MLFQ,
    COMPARE
} SchedulingAlgorithm;

/*
 * MLFQConfig -> parsed from --mlfq-config=<file>
 * quantums[i] = time quantum for queue level i (0 = FCFS at that level)
 * boost_interval = 0 means no priority boost
 */
typedef struct {
    int num_queues;
    int quantums[MAX_MLFQ_QUEUES];
    int boost_interval;
} MLFQConfig;

// holds parsed command line input
typedef struct {
    SchedulingAlgorithm algorithm;
    char processes_str[MAX_PROCESS_STR];
    char input_file[MAX_FILENAME];
    int quantum;                            // for RR
    char mlfq_config_file[MAX_FILENAME];    // for MLFQ
    MLFQConfig mlfq_cfg;                    // populated by parse_mlfq_config()
    bool compare_all;
} CLIOptions;

CLIOptions  parse_args(int argc, char *argv[]);
MLFQConfig  parse_mlfq_config(const char *filename); // pass NULL for defaults
int         parse_processes(const char *str, Process processes[], int max_processes);
int         parse_workload_file(const char *filename, Process processes[], int max_processes);

#endif