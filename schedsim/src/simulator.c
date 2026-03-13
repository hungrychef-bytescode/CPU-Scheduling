#include "simulator.h"

void simulator(SchedulerState *state, SchedulingAlgorithm algorithm) {

    Event *queue = initialize_events(state);
}

//create the events - assign the event type (arrival), the process and the arrival time
Event* initialize_events(SchedulerState *state) {

    Event *queue = NULL;

    for (int i = 0; i < state->num_processes; i++) {

        Process *proc = state->processes[i];

        Event *new_event = malloc(sizeof(Event));
        new_event->time = proc->arrival_time;
        new_event->type = EVENT_ARRIVAL;
        new_event->process = proc;
        new_event->next = NULL;

        insert_event(&queue, new_event);
    }

    return queue;
}

//linsert a new event to the event queue -> linked list form
//LL is by order of arrival for the execution/simulation of the processes later
// head: pointer to the head of event list
// new_event: event node to insert in LL
void insert_event(Event **head, Event *new_event) {

    //insert to head if queue is empty or if new_event should've arrived first
    if (*head == NULL || new_event->time < (*head)->time) {
        new_event->next = *head;                                //point new event's next to the CURRENT head
        *head = new_event;                                      //update head so the new_event becomes the new head
        return;
    }

    //if node will be inserted within the LL:
    Event *current = *head;

    //traverse and find where to insert the new event
    //if it's not the end of LL
    // or new node arrival time is later than current's time -> loop
    while (*current != NULL && new_event->time >= current->next->time) {
        current = current->next;                                //continue to next node
    }

    //insert new_event after current node 
    //when current's next node arrival time is after the new node's 
    //or if reached the end of the LL:
    new_event->next = current->next;                           //point new event's next to the current's next
    current->next = new_event;                                 //set current's next node to be the new event

}