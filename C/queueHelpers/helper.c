#include <stdlib.h>
#include "../coreJob/job.h"
#include "helper.h"
#include "timeline.h"
#include <stdio.h>


void createJson(struct Job jobs[], int len) {
    printf("{");
    printf("\"metrics\": {");

    for (int i = 0; i < len; i++) {
        printf("\"P%d\": {\"completion\": %d, \"turnaround\": %d, \"response\": %d}",
            i,
            jobs[i].end,
            jobs[i].turnaround,
            jobs[i].response);

        if (i != len - 1) printf(",");
    }

    printf("},");
    printf("\"timeline\": [");

    for (int i = 0; i < tcount; i++) {
        printf("{\"process\": \"P%d\", \"start\": %d, \"end\": %d}",
            timeline[i].pid,
            timeline[i].start,
            timeline[i].end);

        if (i != tcount - 1) printf(",");
    }

    printf("]");
    printf("}");
}

void clearQueue(struct Node **head, struct Node **tail, struct Node **head2, struct Node **tail2, int *reset) {
    *reset = 100;
    if (*head2 == NULL) return;

    if (*head == NULL) {
        *head = *head2;
        *tail = *tail2;
    } else {
        (*tail)->next = *head2;
        *tail = *tail2;
    }

    *head2 = *tail2 = NULL;
}

void enqueueReset(struct Job jobs[], struct Node **head, struct Node **tail, int len) {
    for(int i=0; i < len; i++) {
        if (jobs[i].inQueue && jobs[i].remaining_time > 0) {
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            newNode->data = i;
            newNode->next = NULL;

            if(!(*head)) {
                *head = *tail = newNode;
            } else {
                (*tail)->next = newNode;
                *tail = newNode;
            }
        }
    }
}

void enqueueFirst(struct Job jobs[], struct Node **head, struct Node **tail, int len, int current_time) {
    for (int i = 0; i < len; i++) {
        if (!jobs[i].inQueue && jobs[i].remaining_time > 0 && jobs[i].arrival_time <= current_time && jobs[i].response == -1) {
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            newNode->data = i;
            newNode->next = NULL;

            if (!(*head)) {
                *head = *tail = newNode;
            } else {
                (*tail)->next = newNode;
                *tail = newNode;
            }

            jobs[i].inQueue = true;
        }
    }
}


void degradeQueue(struct Job *j, struct Node **head, struct Node **tail, struct Node **head2, struct Node **tail2) {
    struct Node *temp = *head;
    struct Node *prev = NULL;

    while (temp != NULL) {
        struct Node *nextNode = temp->next;
        struct Job *currentJob = &j[temp->data];

        if (currentJob->alloted_left == 0) {
            if (prev == NULL)
                *head = nextNode;
            else
                prev->next = nextNode;

            if (temp == *tail)
                *tail = prev;

            temp->next = NULL;
            if (*head2 == NULL) {
                *head2 = *tail2 = temp;
            } else {
                (*tail2)->next = temp;
                *tail2 = temp;
            }

            currentJob->alloted_left = currentJob->alloted_time;
        } else {
            prev = temp; 
        }

        temp = nextNode;
    }
}