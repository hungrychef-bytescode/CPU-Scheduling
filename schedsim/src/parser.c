#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

//parse cmd-line arguments to a CLIOptions struct
//handles --algorithm, --input, --quantum, --processes, --compare, --mlfq-config
CLIOptions parse_args(int count, char *cmdarg[]) {

    CLIOptions option = {0};
    //option.quantum = ; //default time quantum

    //to do: handle edge cases
    //test lowercase uppercase

    if (count < 2) {
        fprintf(stderr, "Usage: ./schedsim --algorithm=<ALG> --input=<file>\n");
        fprintf(stderr, "       ./schedsim --compare --input=<file>\n");
        fprintf(stderr, "       ./schedsim --algorithm=RR --quantum=<q> --input=<file>\n");
        exit(1);
    }

    for (int i = 1; i < count; i++) {

        if (strncmp(cmdarg[i], "--algorithm=", 12) == 0) {

            char *algo = cmdarg[i] + 12;

            if (strcmp(algo, "FCFS") == 0) option.algorithm = FCFS;
            else if (strcmp(algo, "SJF") == 0) option.algorithm = SJF;
            else if (strcmp(algo, "STCF") == 0) option.algorithm = STCF;
            else if (strcmp(algo, "RR") == 0) option.algorithm = RR;
            else if (strcmp(algo, "MLFQ") == 0) option.algorithm = MLFQ;
            else {
                fprintf(stderr, "unknown algo: %s\n", algo)
                //exit smthn //handle
            }

        } else if (strncmp(cmdarg[i], "--compare") == 0 ) {
            option.compare_all = true;

        } else if (strncmp(cmdarg[i], "--input=", 8) == 0) {
            snprintf(option.input_file, MAX_FILENAME, "%s", cmdarg[i] + 8);

        } else if (strncmp(cmdarg[i], "--processes=", 12) == 0) {
            snprintf(option.processes_str, MAX_PROCESS_STR, "%s", cmdarg[i] + 12);

        } else if (strncmp(cmdarg[i], "--quantum=", 10) == 0) {
            char *end;
            long quantum = strtol(cmdarg[i] + 10, &end, 10);

            if (*end != '\0' || quantum <= 0) {
                fprintf(stderror, "invalid number");
                //exit
            } 

            option.quantum = (int) quantum;
        } else if (strncmp(cmdarg[i], "--mlfq-config=", 14) == 0) {
            snprintf(option.mlfq_config_file, MAX_FILENAME, "%s", cmdarg[i] + 14);
        } else {
            fprintf(stderr, "Unknown argument %s\n", cmdarg[i]);
            exit(1);
        }
    }

    return option;
}


/*
 * parse_processes_string func -> parses inline process string from CLI
 * Format: "A:0:240,B:10:180,C:20:150"
 * Returns number of processes parsed
 */
int parse_processes_string(const char *str, Process processes[], int max_processes) {
    char buf[MAX_PROC_STR];
    strncpy(buf, str, MAX_PROC_STR - 1);

    int count = 0;
    char *token = strtok(buf, ",");

    while (token != NULL && count < max_processes) {
        Process p = {0};
        p.start_time = -1;

        if (sscanf(token, "%15[^:]:%d:%d", p.pid, &p.arrival_time, &p.burst_time) != 3) {
            fprintf(stderr, "Warning: skipping malformed process entry: %s\n", token);
            token = strtok(NULL, ",");
            continue;
        }

        p.remaining_time = p.burst_time;
        processes[count++] = p;
        token = strtok(NULL, ",");
    }

    if (count == 0) {
        fprintf(stderr, "Error: no valid processes found in --processes string\n");
        exit(1);
    }

    return count;
}


/*
 * parse_workload_file func -> reads process workload from a file
 * Format: PID ArrivalTime BurstTime (lines starting with # are comments)
 * Returns number of processes read
 */
int parse_workload_file(const char *filename, Process processes[], int max_processes) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        exit(1);
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        // Skip comments and blank lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        if (count >= max_processes) {
            fprintf(stderr, "Warning: max process limit (%d) reached\n", max_processes);
            break;
        }

        Process p = {0};
        p.start_time = -1;  // not yet started

        if (sscanf(line, "%15s %d %d", p.pid, &p.arrival_time, &p.burst_time) != 3) {
            fprintf(stderr, "Warning: skipping malformed line: %s", line);
            continue;
        }

        p.remaining_time = p.burst_time;
        processes[count++] = p;
    }

    fclose(fp);

    if (count == 0) {
        fprintf(stderr, "Error: no valid processes found in '%s'\n", filename);
        exit(1);
    }

    return count;
}
