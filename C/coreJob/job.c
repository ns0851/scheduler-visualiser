#include "job.h"

void initStruct(struct Job *j, int s, int a, int c, int aa) {
    j->arrival_time = a;
    j->cpu_burst = c;

    j->serial = s;
    j->remaining_time = c;
    j->alloted_time = aa;
    j->alloted_left = aa;

    j->start = -1;
    j->end = -1;
    j->response = -1;
    j->turnaround = 0;
    j->inQueue = false;
}