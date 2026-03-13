#include "simulator.h"

void simulator(SchedulerState *state, SchedulingAlgorithm algorithm) {

    Event *queue = initialize_events(state);
}

//create 
Event* initialize_events(SchedulerState *state) {

    Event *queue = NULL;

    for (int i = 0; i < state->num_processes; i++) {

        Process *proc = state->processes[i];

        Event *event = malloc(sizeof(Event));
        event->time = proc->arrival_time;
        event->type = EVENT_ARRIVAL;
        event->process = proc;
        event->next = NULL;

        insert_event(&queue, event);
    }

    return queue;
}