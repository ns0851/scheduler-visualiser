#ifndef JOB_H
#define JOB_H

#include <stdbool.h>

struct Job
{
    int arrival_time;
    int cpu_burst;
    int start;
    int end;
    int turnaround;
    int response;
    int remaining_time;
    bool inQueue;
    int serial;
    int alloted_time;
    int alloted_left;
};

void initStruct(struct Job *j, int s, int a, int c, int aa);

#endif