#include <string.h>
#include <stdio.h>
#include "scheduler/basic_scheduler.h"
#include "scheduler/MLFQ.h"
#include "coreJob/job.h"
#include <stdlib.h>
#include "cJSON.h"

int main() {

    struct Job *jobs = malloc(100 * sizeof(struct Job));

    char buffer[10000];
    int len = fread(buffer, 1, sizeof(buffer) - 1, stdin);
    buffer[len] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        printf("{\"error\":\"Invalidd JSON\"}");
        return 1;
    }

    cJSON *algo = cJSON_GetObjectItem(json, "algorithm");
    cJSON *quantum = cJSON_GetObjectItem(json, "quantum");
    cJSON *reset = cJSON_GetObjectItem(json, "reset");
    cJSON *processes = cJSON_GetObjectItem(json, "processes");

    int count = cJSON_GetArraySize(processes);

    for (int i = 0; i < count; i++) {
        cJSON *proc = cJSON_GetArrayItem(processes, i);

        int id = cJSON_GetObjectItem(proc, "id")->valueint;
        int arrival = cJSON_GetObjectItem(proc, "arrival")->valueint;
        int burst = cJSON_GetObjectItem(proc, "burst")->valueint;
        int alloted = cJSON_GetObjectItem(proc, "allotment")->valueint;

        initStruct(&jobs[i], id, arrival, burst, alloted);
    }

    if (strcmp(algo->valuestring, "fcfs") == 0) {
        fcfs(jobs, count);
    } else if (strcmp(algo->valuestring, "sjfp") == 0) {
        sjfP(jobs, count);
    } else if (strcmp(algo->valuestring, "sjf") == 0) {
        sjfNP_pointer_final(jobs, count);
    } else if (strcmp(algo->valuestring, "rr") == 0) {
        rr(jobs, count, quantum->valueint);
    } else if (strcmp(algo->valuestring, "mlfq") == 0) {
        MLFQ3(jobs, count, quantum->valueint, reset->valueint);
    }

    cJSON_Delete(json);
    free(jobs);

    return 0;
}