#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"
#include "../include/parser.h"

//parse cmd-line arguments to a CLIOptions struct
//handles --algorithm, --input, --quantum, --processes, --compare, --mlfq-config
CLIOptions parse_args(int count, char *cmdarg[]) {

    CLIOptions option = {0};

    if (count < 2) {
        fprintf(stderr, "Usage: ./schedsim --algorithm=<ALG> --input=<file>\n");
        fprintf(stderr, "       ./schedsim --compare --input=<file>\n");
        fprintf(stderr, "       ./schedsim --algorithm=RR --quantum=<q> --input=<file>\n");
        fprintf(stderr, "       ./schedsim --algorithm=MLFQ --mlfq-config=<file> --input=<file>\n");
        exit(1);
    }

    for (int i = 1; i < count; i++) {

        if (strncmp(cmdarg[i], "--algorithm=", 12) == 0) {

            char *algo = cmdarg[i] + 12;

            if      (strcmp(algo, "FCFS") == 0) option.algorithm = FCFS;
            else if (strcmp(algo, "SJF")  == 0) option.algorithm = SJF;
            else if (strcmp(algo, "STCF") == 0) option.algorithm = STCF;
            else if (strcmp(algo, "RR")   == 0) option.algorithm = RR;
            else if (strcmp(algo, "MLFQ") == 0) option.algorithm = MLFQ;
            else {
                fprintf(stderr, "unknown algo: %s\n", algo);
                fprintf(stderr, "algo options: FCFS, SJF, STCF, RR, MLFQ\n");
                exit(1);
            }

        } else if (strcmp(cmdarg[i], "--compare") == 0) {
            option.compare_all = true;

        } else if (strncmp(cmdarg[i], "--input=", 8) == 0) {
            snprintf(option.input_file, MAX_FILENAME, "%s", cmdarg[i] + 8);

        } else if (strncmp(cmdarg[i], "--processes=", 12) == 0) {
            snprintf(option.processes_str, MAX_PROCESS_STR, "%s", cmdarg[i] + 12);

        } else if (strncmp(cmdarg[i], "--quantum=", 10) == 0) {
            char *end;
            long quantum = strtol(cmdarg[i] + 10, &end, 10);
            if (*end != '\0' || quantum <= 0) {
                fprintf(stderr, "Error: invalid --quantum value\n");
                exit(1);
            }
            option.quantum = (int) quantum;

        } else if (strncmp(cmdarg[i], "--mlfq-config=", 14) == 0) {
            snprintf(option.mlfq_config_file, MAX_FILENAME, "%s", cmdarg[i] + 14);

        } else {
            fprintf(stderr, "Unknown argument: %s\n", cmdarg[i]);
            exit(1);
        }
    }

    if (!option.compare_all && option.algorithm == NONE) {
        fprintf(stderr, "Error: --algorithm is required (or use --compare)\n");
        exit(1);
    }

    if (option.input_file[0] == '\0' && option.processes_str[0] == '\0') {
        fprintf(stderr, "Error: --input or --processes is required\n");
        exit(1);
    }

    if (option.algorithm == RR && option.quantum <= 0) {
        fprintf(stderr, "Error: --algorithm=RR requires --quantum=<q>\n");
        exit(1);
    }

    // if MLFQ selected, parse config now (NULL = use defaults)
    if (option.algorithm == MLFQ || option.compare_all) {
        option.mlfq_cfg = parse_mlfq_config(
            option.mlfq_config_file[0] ? option.mlfq_config_file : NULL
        );
    }

    printf("\n Algorithm: %d\n", option.algorithm);
    printf("Processes string: %s\n", option.processes_str);
    printf("Input file: %s\n", option.input_file);
    printf("Quantum: %d\n", option.quantum);
    printf("Compare all: %s\n", option.compare_all ? "true" : "false");
    return option;
}

/*
 * parse_mlfq_config -> parses an MLFQ config file into an MLFQConfig struct
 *
 * File format:
 *   queues=<N>
 *   quantum=<q0>,<q1>,...,<qN-1>    (0 = FCFS at that level)
 *   boost=<interval>                 (optional, 0 = disabled)
 *
 * Example:
 *   queues=3
 *   quantum=8,16,0
 *   boost=100
 *
 * Pass NULL to use defaults: 3 queues, quantums 8/16/0, no boost
 */
MLFQConfig parse_mlfq_config(const char *filename) {

    MLFQConfig cfg = {0};

    // --- apply defaults first ---
    cfg.num_queues     = MLFQ_DEFAULT_QUEUES;
    cfg.boost_interval = 0;
    for (int i = 0; i < MLFQ_DEFAULT_QUEUES; i++) {
        cfg.quantums[i] = MLFQ_DEFAULT_BASE_QUANTUM * (1 << i); // 8, 16, 32
    }
    cfg.quantums[MLFQ_DEFAULT_QUEUES - 1] = 0; // last queue is FCFS

    if (filename == NULL || filename[0] == '\0') {
        fprintf(stderr, "Note: no --mlfq-config provided, using defaults "
                        "(%d queues, quantums", cfg.num_queues);
        for (int i = 0; i < cfg.num_queues; i++)
            fprintf(stderr, " %d", cfg.quantums[i]);
        fprintf(stderr, ", no boost)\n");
        return cfg;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open mlfq config file '%s'\n", filename);
        exit(1);
    }

    int queues_set  = 0;
    int quantum_set = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {

        // skip comments and blank lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        // strip trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        if (strncmp(line, "queues=", 7) == 0) {
            char *end;
            long n = strtol(line + 7, &end, 10);
            if (*end != '\0' || n <= 0 || n > MAX_MLFQ_QUEUES) {
                fprintf(stderr, "Error: invalid 'queues' value (must be 1-%d)\n",
                        MAX_MLFQ_QUEUES);
                fclose(fp);
                exit(1);
            }
            cfg.num_queues = (int) n;
            queues_set = 1;

        } else if (strncmp(line, "quantum=", 8) == 0) {
            char *ptr = line + 8;
            int idx = 0;
            while (*ptr != '\0' && idx < MAX_MLFQ_QUEUES) {
                char *end;
                long q = strtol(ptr, &end, 10);
                if (end == ptr) {
                    fprintf(stderr, "Error: malformed 'quantum' list in mlfq config\n");
                    fclose(fp);
                    exit(1);
                }
                if (q < 0) {
                    fprintf(stderr, "Error: quantum values must be >= 0\n");
                    fclose(fp);
                    exit(1);
                }
                cfg.quantums[idx++] = (int) q;
                if (*end == ',') ptr = end + 1;
                else             ptr = end;
            }
            quantum_set = idx;

        } else if (strncmp(line, "boost=", 6) == 0) {
            char *end;
            long b = strtol(line + 6, &end, 10);
            if (*end != '\0' || b < 0) {
                fprintf(stderr, "Error: invalid 'boost' value (must be >= 0)\n");
                fclose(fp);
                exit(1);
            }
            cfg.boost_interval = (int) b;

        } else {
            fprintf(stderr, "Warning: unknown key in mlfq config, skipping: %s\n", line);
        }
    }

    fclose(fp);

    // --- validate ---
    if (!queues_set) {
        fprintf(stderr, "Error: mlfq config missing required field 'queues'\n");
        exit(1);
    }
    if (!quantum_set) {
        fprintf(stderr, "Error: mlfq config missing required field 'quantum'\n");
        exit(1);
    }
    if (quantum_set != cfg.num_queues) {
        fprintf(stderr, "Error: mlfq config 'quantum' has %d entries but queues=%d\n",
                quantum_set, cfg.num_queues);
        exit(1);
    }

    printf("MLFQ config loaded: %d queues, quantums:", cfg.num_queues);
    for (int i = 0; i < cfg.num_queues; i++)
        printf(cfg.quantums[i] == 0 ? " FCFS" : " %d", cfg.quantums[i]);
    printf(", boost: %s\n", cfg.boost_interval ? "enabled" : "disabled");

    return cfg;
}

/*
 * parse_processes -> parses inline process string from CLI
 * Format: "A:0:240,B:10:180,C:20:150"
 * Returns number of processes parsed
 */
int parse_processes(const char *str, Process processes[], int max_processes) {
    char buf[MAX_PROCESS_STR];
    snprintf(buf, sizeof(buf), "%s", str);

    int count = 0;
    char *token = strtok(buf, ",");

    while (token != NULL && count < max_processes) {

        while (*token == ' ') token++;

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
 * parse_workload_file -> reads process workload from a file
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
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        if (count >= max_processes) {
            fprintf(stderr, "Warning: max process limit (%d) reached\n", max_processes);
            break;
        }

        Process p = {0};
        p.start_time = -1;

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