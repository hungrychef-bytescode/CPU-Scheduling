#ifndef PARSER_H                //header guard
#define PARSER

#include <stdbool.h>
#include "scheduler.h"

#define MAX_FILENAME 256
#define MAX_PROCESS_STR 1000

//holds parsed command line input
typedef struct {
    SchedulingAlgorithm algorithm;
    char processes_str[MAX_PROCESS_STR];
    char input_file[MAX_FILENAME];
    int quantum;                                    //for RR
    char mlfq_config_file[MAX_FILENAME];            //for MLFQ later
    bool compare_all;
} CLIOptions;

CLIOptions parse_args(int argc, char *argv[]);
int parse_processes(const char *str, Process processes[], int max_processes);
int parse_workload_file(const char *filename, Process processes[], int max_processes);

#endif