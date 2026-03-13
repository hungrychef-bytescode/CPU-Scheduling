#include <stdio.h>              //printf
#include <stdlib.h>             //exit
#include "parser.h"
#include "scheduler.h"

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
        fprintf(stderr, "no workload specified\n");
        exit(1);
    }

    //designated initializer for scheduler state
    SchedulerState state = {
        .processes = original,
        .num_processes = count,
        .current_time = 0,
        .current_process = NULL
    };
   

    switch(option.algorithm) {
        case FCFS:
            printf("running FCFS");
            schedule_fcfs(&state);
            break;
        case SJF:
            printf("running sjf");
            // schedule_sjf(&state);
            break;
        case STCF:
            printf("running sctf");
            // schedule_sctf(&state);
            break;
        case RR:
            printf("running rr");
            // schedule_rr(&state);
            break;
        case MLFQ:
            printf("running mlfq");
            // schedule_mlfq(&state);
            break;
        case COMPARE:
            printf("running compare");
            // schedule_compare(&state);
            break;
        default:
            fprintf(stderr, "unknown algorithm");   
    }


    printf("%d", count );
}