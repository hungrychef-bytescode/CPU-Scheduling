#include <stdio.h>
#include <string.h>
#include "../include/process.h"

// STUB FILE - to be implemented

Metrics schedule_mlfq(Process processes[], int n, const char *config_file) {
    (void)processes;
    (void)n;
    (void)config_file;
    printf("MLFQ: Not yet implemented\n\n");
    Metrics m = {0};
    strncpy(m.algorithm, "MLFQ", MAX_ALG_LEN - 1);
    return m;
}