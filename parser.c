#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/* ─────────────────────────────────────────
   HELPER: map algorithm string → enum
   ───────────────────────────────────────── */
static Algorithm parse_algorithm_name(const char *name) {
    if (strcmp(name, "FCFS")    == 0) return ALGO_FCFS;
    if (strcmp(name, "SJF")     == 0) return ALGO_SJF;
    if (strcmp(name, "STCF")    == 0) return ALGO_STCF;
    if (strcmp(name, "RR")      == 0) return ALGO_RR;
    if (strcmp(name, "MLFQ")    == 0) return ALGO_MLFQ;
    return ALGO_NONE;
}

/* ─────────────────────────────────────────
   HELPER: initialize a Process to safe defaults
   ───────────────────────────────────────── */
static void init_process(Process *p) {
    memset(p->pid, 0, MAX_PID_LEN);
    p->arrival_time  = 0;
    p->burst_time    = 0;
    p->remaining_time = 0;
    p->start_time    = -1;   // -1 = not started yet
    p->finish_time   = -1;
    p->waiting_time  = 0;
    p->priority      = 0;
    p->time_in_queue = 0;
}

/* ─────────────────────────────────────────
   parse_args
   Reads argc/argv → fills Config struct
   ───────────────────────────────────────── */
int parse_args(int argc, char *argv[], Config *config) {
    if (!config) return -1;

    /* set defaults */
    memset(config, 0, sizeof(Config));
    config->algorithm = ALGO_NONE;
    config->quantum   = 30;   // default RR quantum per lab spec

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        /* --compare */
        if (strcmp(arg, "--compare") == 0) {
            config->compare_mode = 1;
            config->algorithm    = ALGO_COMPARE;
            continue;
        }

        /* flags with values: --key=value */
        char *eq = strchr(arg, '=');
        if (!eq) {
            fprintf(stderr, "Warning: unrecognized flag '%s'\n", arg);
            continue;
        }

        /* split into key and value */
        int   key_len = (int)(eq - arg);
        char  key[64] = {0};
        char *value   = eq + 1;

        if (key_len <= 0 || key_len >= (int)sizeof(key)) {
            fprintf(stderr, "Warning: malformed flag '%s'\n", arg);
            continue;
        }
        strncpy(key, arg, key_len);

        /* match key */
        if (strcmp(key, "--algorithm") == 0) {
            config->algorithm = parse_algorithm_name(value);
            if (config->algorithm == ALGO_NONE) {
                fprintf(stderr, "Error: unknown algorithm '%s'\n", value);
                return -1;
            }

        } else if (strcmp(key, "--input") == 0) {
            strncpy(config->input_file, value, sizeof(config->input_file) - 1);

        } else if (strcmp(key, "--quantum") == 0) {
            config->quantum = atoi(value);
            if (config->quantum <= 0) {
                fprintf(stderr, "Error: quantum must be > 0\n");
                return -1;
            }

        } else if (strcmp(key, "--mlfq-config") == 0) {
            strncpy(config->mlfq_config, value, sizeof(config->mlfq_config) - 1);

        } else if (strcmp(key, "--processes") == 0) {
            strncpy(config->inline_processes, value,
                    sizeof(config->inline_processes) - 1);

        } else {
            fprintf(stderr, "Warning: unrecognized flag '%s'\n", key);
        }
    }

    /* validation */
    if (config->algorithm == ALGO_NONE) {
        fprintf(stderr, "Error: --algorithm is required\n");
        return -1;
    }
    if (config->input_file[0] == '\0' && config->inline_processes[0] == '\0') {
        fprintf(stderr, "Error: provide --input=file or --processes=\"...\"\n");
        return -1;
    }

    return 0;
}

/* ─────────────────────────────────────────
   parse_workload_file
   Reads workload .txt → Process array
   Format: PID ArrivalTime BurstTime
   Lines starting with '#' are comments
   ───────────────────────────────────────── */
int parse_workload_file(const char *filename, Process processes[], int max) {
    if (!filename || !processes) return -1;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        return -1;
    }

    int   count = 0;
    char  line[256];

    while (fgets(line, sizeof(line), fp)) {
        /* skip blank lines and comments */
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;

        if (count >= max) {
            fprintf(stderr, "Warning: max process limit (%d) reached\n", max);
            break;
        }

        Process p;
        init_process(&p);

        /* parse:  PID  ArrivalTime  BurstTime */
        int matched = sscanf(line, "%15s %d %d",
                             p.pid,
                             &p.arrival_time,
                             &p.burst_time);

        if (matched != 3) {
            fprintf(stderr, "Warning: skipping malformed line: %s", line);
            continue;
        }
        if (p.arrival_time < 0 || p.burst_time <= 0) {
            fprintf(stderr, "Warning: invalid values for process %s, skipping\n",
                    p.pid);
            continue;
        }

        p.remaining_time = p.burst_time;  // initialize for preemptive algos
        processes[count++] = p;
    }

    fclose(fp);
    return count;
}

/* ─────────────────────────────────────────
   parse_inline_processes
   Parses --processes="A:0:240,B:10:180,C:20:150"
   ───────────────────────────────────────── */
int parse_inline_processes(const char *raw, Process processes[], int max) {
    if (!raw || !processes) return -1;

    /* work on a mutable copy */
    char buf[1024];
    strncpy(buf, raw, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    int   count = 0;
    char *saveptr_outer = NULL;
    char *token = strtok_r(buf, ",", &saveptr_outer);  // split by comma

    while (token && count < max) {
        Process p;
        init_process(&p);

        /* each token is  PID:ArrivalTime:BurstTime */
        char *saveptr_inner = NULL;
        char *part = strtok_r(token, ":", &saveptr_inner);

        /* PID */
        if (!part) { fprintf(stderr, "Warning: missing PID in '%s'\n", token); goto next; }
        strncpy(p.pid, part, MAX_PID_LEN - 1);

        /* ArrivalTime */
        part = strtok_r(NULL, ":", &saveptr_inner);
        if (!part) { fprintf(stderr, "Warning: missing ArrivalTime for %s\n", p.pid); goto next; }
        p.arrival_time = atoi(part);

        /* BurstTime */
        part = strtok_r(NULL, ":", &saveptr_inner);
        if (!part) { fprintf(stderr, "Warning: missing BurstTime for %s\n", p.pid); goto next; }
        p.burst_time = atoi(part);

        if (p.burst_time <= 0) {
            fprintf(stderr, "Warning: BurstTime must be > 0 for %s\n", p.pid);
            goto next;
        }

        p.remaining_time = p.burst_time;
        processes[count++] = p;

    next:
        token = strtok_r(NULL, ",", &saveptr_outer);
    }

    return count;
}

/* ─────────────────────────────────────────
   DEBUG HELPERS
   ───────────────────────────────────────── */
void print_config(const Config *config) {
    const char *algo_names[] = {
        "NONE", "FCFS", "SJF", "STCF", "RR", "MLFQ", "COMPARE"
    };
    printf("=== Parsed Config ===\n");
    printf("Algorithm : %s\n", algo_names[config->algorithm]);
    printf("Quantum   : %d\n", config->quantum);
    printf("Input file: %s\n", config->input_file[0] ? config->input_file : "(none)");
    printf("MLFQ cfg  : %s\n", config->mlfq_config[0] ? config->mlfq_config : "(none)");
    printf("Inline    : %s\n", config->inline_processes[0] ? config->inline_processes : "(none)");
    printf("Compare   : %s\n", config->compare_mode ? "yes" : "no");
}

void print_processes(const Process processes[], int n) {
    printf("=== Processes Loaded (%d) ===\n", n);
    printf("%-6s %12s %10s\n", "PID", "ArrivalTime", "BurstTime");
    printf("%-6s %12s %10s\n", "------", "-----------", "---------");
    for (int i = 0; i < n; i++) {
        printf("%-6s %11d %10d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time);
    }
}
