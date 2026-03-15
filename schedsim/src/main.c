#include <stdio.h>              //printf
#include "parser.h"
#include "scheduler.h"
#include "../include/process.h"

int main(int argc, char *argv[]) {              //gets cli count, arr of cli cmd
    
    CLIOptions option = parse_args(argc, argv);

    Process original[MAX_PROCESSES];
    // Process copy[MAX_PROCESSES];

    int count;

    if (option.processes_str[0]) {
        count = parse_processes(option.processes_str, original, MAX_PROCESSES);
    } else if (option.input_file[0]) {
        count = parse_workload_file(option.input_file, original, MAX_PROCESSES);
    } else {
        //error
    }

    printf("%d", count );
}