#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

/*
 * gantt_record -> records which process ran at a given time unit
 * gantt[] is an array where index = time unit, value = first char of PID
 */
void gantt_record(char gantt[], int time, const char *pid) {
    if (time >= 0 && time < MAX_GANTT - 1) {
        gantt[time] = pid[0];
    }
}

/*
 * gantt_print -> prints ASCII Gantt chart from the recorded gantt array
 * Automatically scales if total_time is large (1 char = 10 units)
 */
void gantt_print(char gantt[], int total_time) {
    printf("=== Gantt Chart ===\n");

    // Decide scale: if total_time > 100, scale down
    int scale = 1;
    if (total_time > 100) {
        scale = 10;
        printf("(Each character = %d time units)\n", scale);
    }

    // ── Print process row ─────────────────────────────────────────────────────
    printf("[");
    char prev = gantt[0];
    int  seg_start = 0;

    for (int t = 1; t <= total_time; t++) {
        char curr = (t < total_time) ? gantt[t] : '\0';

        if (curr != prev || t == total_time) {
            // Only print at scale intervals
            if (scale == 1) {
                // Print full segment
                for (int s = seg_start; s < t; s++) {
                    printf("%c", prev == '.' ? '-' : prev);
                }
            } else {
                // Print scaled segment
                int len = (t - seg_start + scale - 1) / scale;
                for (int s = 0; s < len; s++) {
                    printf("%c", prev == '.' ? '-' : prev);
                }
            }

            if (t < total_time) printf("][");
            seg_start = t;
            prev = curr;
        }
    }
    printf("]\n");

    // ── Print time markers ────────────────────────────────────────────────────
    int prev_len = 0;
    int printed  = 0;

    // Print time=0
    printf("%-4d", 0);
    prev_len = 4;
    printed  = 0;

    // Track segment boundaries to place time markers
    char prev_c = gantt[0];
    for (int t = 1; t <= total_time; t++) {
        char curr_c = (t < total_time) ? gantt[t] : '\0';

        if (curr_c != prev_c || t == total_time) {
            int seg_len = t - printed;
            int chars   = (scale == 1) ? seg_len : (seg_len + scale - 1) / scale;

            // Pad to end of segment
            for (int s = prev_len; s < chars; s++) printf(" ");
            printf("%-4d", t);
            prev_len = 4;
            printed  = t;
            prev_c   = curr_c;
        }
    }
    printf("\n\n");
}