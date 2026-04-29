#ifndef BASIC_SCHEDULER_H
#define BASIC_SCHEDULER_H

#include "../coreJob/job.h"

// Scheduling algorithms
void fcfs(struct Job jobs[], int n);
void sjfP(struct Job jobs[], int n);
void rr(struct Job jobs[], int n, int quantum);

#endif