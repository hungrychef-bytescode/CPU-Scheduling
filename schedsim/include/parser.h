#ifndef PARSER_H                //header guard
#define PARSER

#include <stdbool.h>
#include scheduler.h

#define MAX_FILENAME 256
#define MAX_PROCESS_STR 1000

//holds parsed command line input
typedef struct {
    SchedulingAlgorithm algorithm;
    char *processes_str[MAX_PROCESS_STR];
    char *input_file[MAX_FILENAME];
    int quantum;                //for RR
    
    //insert for MLFQ later
    //--mlfq-config=mlfq_config.txt

    bool compare_all;
} CLIOptions;

#endif