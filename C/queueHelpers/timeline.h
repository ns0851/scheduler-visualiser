#ifndef TIMELINE_H
#define TIMELINE_H

struct Interval {
    int pid;
    int start;
    int end;
};

extern struct Interval timeline[100];
extern int tcount;

#endif