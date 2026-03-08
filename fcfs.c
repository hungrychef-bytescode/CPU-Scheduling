#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"

/*
 * FIRST-COME FIRST-SERVE (FCFS)
 * ─────────────────────────────
 * Non-preemptive. Processes run in order of arrival time.
 * Once a process starts, it runs until completion.
 * Tie-breaking: alphabetical by PID (for same arrival time).
 *
 * Steps:
 *  1. Sort processes by arrival time
 *  2. Pick the first arrived process in the ready queue
 *  3. Run it to completion
 *  4. Repeat until all done
 */

/* ── helper: sort by arrival time, then PID alphabetically ── */
static void sort_by_arrival(Process *processes, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int swap = 0;
            if (processes[i].arrival_time > processes[j].arrival_time) {
                swap = 1;
            } else if (processes[i].arrival_time == processes[j].arrival_time &&
                       strcmp(processes[i].pid, processes[j].pid) > 0) {
                swap = 1;  // tie-break alphabetically
            }
            if (swap) {
                Process tmp  = processes[i];
                processes[i] = processes[j];
                processes[j] = tmp;
            }
        }
    }
}

/* ─────────────────────────────────────────
   schedule_fcfs
   ───────────────────────────────────────── */
int schedule_fcfs(SchedulerState *state) {
    if (!state || state->num_processes <= 0) return -1;

    int       n         = state->num_processes;
    Process  *procs     = state->processes;
    int      *done      = calloc(n, sizeof(int));  // 0 = not done

    if (!done) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return -1;
    }

    sort_by_arrival(procs, n);

    state->current_time = 0;
    int completed = 0;

    while (completed < n) {

        /* find next process: arrived and not done yet */
        int chosen = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && procs[i].arrival_time <= state->current_time) {
                chosen = i;
                break;   // already sorted by arrival, first match wins
            }
        }

        /* no process ready yet — CPU is idle, jump to next arrival */
        if (chosen == -1) {
            int next_arrival = -1;
            for (int i = 0; i < n; i++) {
                if (!done[i]) {
                    if (next_arrival == -1 || procs[i].arrival_time < next_arrival)
                        next_arrival = procs[i].arrival_time;
                }
            }
            /* record idle slot in Gantt chart */
            gantt_add_slot(&state->gantt, "IDLE",
                           state->current_time, next_arrival);
            state->current_time = next_arrival;
            continue;
        }

        Process *p = &procs[chosen];

        /* record start time (for response time) */
        p->start_time = state->current_time;

        /* run to completion — non-preemptive */
        int end_time = state->current_time + p->burst_time;

        gantt_add_slot(&state->gantt, p->pid,
                       state->current_time, end_time);

        p->finish_time  = end_time;
        state->current_time = end_time;
        done[chosen] = 1;
        completed++;
    }

    free(done);
    return 0;
}
