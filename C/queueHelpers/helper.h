#ifndef QUEUE_H
#define QUEUE_H

#include "../coreJob/job.h"

struct Node {
    int data;
    struct Node *next;
};

void clearQueue(struct Node **head, struct Node **tail, struct Node **head2, struct Node **tail2, int *reset);

void enqueueReset(struct Job jobs[], struct Node **head, struct Node **tail, int len);

void enqueueFirst(struct Job jobs[], struct Node **head, struct Node **tail, int len, int current_time);

void degradeQueue(struct Job *j, struct Node **head, struct Node **tail, struct Node **head2, struct Node **tail2);

void createJson(struct Job jobs[], int len);

#endif