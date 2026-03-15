#ifndef METRICS_H
#define METRICS_H

#define MAX_ALG_LEN 16

typedef struct {
    char algorithm[MAX_ALG_LEN];
    double avg_turnaround;
    double avg_waiting;
    double avg_response;
    int context_switches;
} Metrics;

void    calculate_metrics(Process processes[], int n);
Metrics compute_averages(Process processes[], int n, const char *algorithm, int context_switches);
void    print_metrics_table(Process processes[], int n);
void    print_averages(Metrics *m);

#endif