#include <stdio.h>
#include <string.h>
#include "../include/process.h"

// STUB FILE - Myra will replace this with the real implementation

Metrics schedule_rr(Process processes[], int n, int quantum) {
    (void)processes;
    (void)n;
    (void)quantum;
    printf("RR: Not yet implemented (Myra's task)\n\n");
    Metrics m = {0};
    strncpy(m.algorithm, "RR", MAX_ALG_LEN - 1);
    return m;
}