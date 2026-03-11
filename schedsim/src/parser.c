#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/*
 * parse_arguments func -> parses command-line arguments into a Config struct
 * Handles: --algorithm, --input, --quantum, --processes, --compare, --mlfq-config
 */
Config parse_arguments(int argc, char *argv[]) {
    Config config = {0};
    config.quantum = 30;        // default time quantum for RR

    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim --algorithm=<ALG> --input=<file>\n");
        fprintf(stderr, "       ./schedsim --compare --input=<file>\n");
        fprintf(stderr, "       ./schedsim --algorithm=RR --quantum=<q> --input=<file>\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        // --algorithm=FCFS / SJF / STCF / RR / MLFQ
        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            strncpy(config.algorithm, argv[i] + 12, MAX_ALG_LEN - 1);

        // --input=workload.txt
        } else if (strncmp(argv[i], "--input=", 8) == 0) {
            strncpy(config.input_file, argv[i] + 8, MAX_PATH_LEN - 1);

        // --quantum=50
        } else if (strncmp(argv[i], "--quantum=", 10) == 0) {
            config.quantum = atoi(argv[i] + 10);
            if (config.quantum <= 0) {
                fprintf(stderr, "Error: quantum must be a positive integer\n");
                exit(1);
            }

        // --processes="A:0:240,B:10:180"
        } else if (strncmp(argv[i], "--processes=", 12) == 0) {
            strncpy(config.processes_str, argv[i] + 12, MAX_PROC_STR - 1);

        // --mlfq-config=mlfq_config.txt
        } else if (strncmp(argv[i], "--mlfq-config=", 14) == 0) {
            strncpy(config.mlfq_config_file, argv[i] + 14, MAX_PATH_LEN - 1);

        // --compare (run all algorithms)
        } else if (strcmp(argv[i], "--compare") == 0) {
            config.compare_mode = true;

        } else {
            fprintf(stderr, "Error: unknown argument '%s'\n", argv[i]);
            exit(1);
        }
    }

    // Validate: must have algorithm or compare mode
    if (!config.compare_mode && config.algorithm[0] == '\0') {
        fprintf(stderr, "Error: --algorithm is required (or use --compare)\n");
        exit(1);
    }

    // Validate: must have input file or inline processes
    if (config.input_file[0] == '\0' && config.processes_str[0] == '\0') {
        fprintf(stderr, "Error: --input or --processes is required\n");
        exit(1);
    }

    return config;
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