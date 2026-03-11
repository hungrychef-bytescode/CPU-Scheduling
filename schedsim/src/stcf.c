#include <stdio.h>
#include <string.h>
#include "../include/process.h"

// STUB FILE - to be implemented

Metrics schedule_stcf(Process processes[], int n) {
    (void)processes;
    (void)n;
    printf("STCF: Not yet implemented\n\n");
    Metrics m = {0};
    strncpy(m.algorithm, "STCF", MAX_ALG_LEN - 1);
    return m;
}