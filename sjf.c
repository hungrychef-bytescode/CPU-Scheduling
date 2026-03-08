#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"

/*
 * SHORTEST JOB FIRST (SJF)
 * ────────────────────────
 * Non-preemptive. Among all arrived processes, pick the one
 * with the shortest burst time. Once started, runs to completion.
 * Tie-breaking: shortest burst → earliest arrival → alphabetical PID.
 *
 * Note: SJF requires knowing burst times in advance (unlike MLFQ).
 *       It is optimal for average turnaround time (non-preemptive case).
 *
 * Steps:
 *  1. At current time, collect all arrived but not-yet-done processes
 *  2. Pick the one with the smallest burst_time
 *  3. Run it to completion
 *  4. Advance time and repeat
 */

/* ─────────────────────────────────────────
   HELPER: pick index of shortest job in ready queue
   Returns -1 if no process is ready
   ───────────────────────────────────────── */
static int pick_shortest(Process *procs, int n, int *done, int current_time) {
    int best = -1;

    for (int i = 0; i < n; i++) {
        if (done[i]) continue;
        if (procs[i].arrival_time > current_time) continue;

        if (best == -1) {
            best = i;
            continue;
        }

        /* compare burst time */
        if (procs[i].burst_time < procs[best].burst_time) {
            best = i;
        }
        /* tie-break 1: earlier arrival */
        else if (procs[i].burst_time == procs[best].burst_time &&
                 procs[i].arrival_time < procs[best].arrival_time) {
            best = i;
        }
        /* tie-break 2: alphabetical PID */
        else if (procs[i].burst_time    == procs[best].burst_time &&
                 procs[i].arrival_time  == procs[best].arrival_time &&
                 strcmp(procs[i].pid, procs[best].pid) < 0) {
            best = i;
        }
    }

    return best;
}

/* ─────────────────────────────────────────
   schedule_sjf
   ───────────────────────────────────────── */
int schedule_sjf(SchedulerState *state) {
    if (!state || state->num_processes <= 0) return -1;

    int      n     = state->num_processes;
    Process *procs = state->processes;
    int     *done  = calloc(n, sizeof(int));

    if (!done) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return -1;
    }

    state->current_time = 0;
    int completed = 0;

    while (completed < n) {

        int chosen = pick_shortest(procs, n, done, state->current_time);

        /* no process ready — CPU idle, jump to next arrival */
        if (chosen == -1) {
            int next_arrival = -1;
            for (int i = 0; i < n; i++) {
                if (!done[i]) {
                    if (next_arrival == -1 ||
                        procs[i].arrival_time < next_arrival)
                        next_arrival = procs[i].arrival_time;
                }
            }
            gantt_add_slot(&state->gantt, "IDLE",
                           state->current_time, next_arrival);
            state->current_time = next_arrival;
            continue;
        }

        Process *p = &procs[chosen];

        /* record start time for response time */
        p->start_time = state->current_time;

        /* run to completion — non-preemptive */
        int end_time = state->current_time + p->burst_time;

        gantt_add_slot(&state->gantt, p->pid,
                       state->current_time, end_time);

        p->finish_time      = end_time;
        state->current_time = end_time;
        done[chosen]        = 1;
        completed++;
    }

    free(done);
    return 0;
}
