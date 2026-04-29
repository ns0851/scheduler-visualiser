#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../coreJob/job.h"
#include "../queueHelpers/helper.h"
#include "../scheduler/basic_scheduler.h"
#include "MLFQ.h"
#include "../queueHelpers/timeline.h"

// ######### Multi-level Feedback Queue ########### //

// faulty logic... breaks after first one

void MLFQ(struct Job jobs[], int len) {
    struct Node *head = NULL, *tail = NULL;
    struct Node *head2 = NULL, *tail2 = NULL;

    bool isQ1Empty;
    bool isQ2Empty;

    int current_time = 0;
    int reset_timer = 100;
    int remaining_jobs = len;

    while (remaining_jobs > 0) {
        // Enqueue all running jobs to highest priority Queue on timer reset
        if (reset_timer == 0) {
            printf("Resetting\n");
            // clearQueue(&head, &tail, &head2, &tail2);
            printList(head);
            enqueueReset(jobs, &head, &tail, len);
            reset_timer = 100;
        } else {
            enqueueFirst(jobs, &head, &tail, len, current_time);

            // Check alloted time for each in queues and degrade them if time's up
            degradeQueue(jobs, &head, &tail, &head2, &tail2);

            printf("First List: \n");
            printList(head);
            printf("Second List: \n");
            printList(head2);

            isQ1Empty = false;
            isQ2Empty = false;

            if (isQ1Empty && isQ2Empty) {
                current_time += 1;
                reset_timer -= 1;
                continue;
            }

            struct Node **activeHead, **activeTail;
            struct Node *curNode;
            struct Job *curJob;

            if (!isQ1Empty) {
                activeHead = &head;
                activeTail = &tail;
            } else {
                activeHead = &head2;
                activeTail = &tail2;
            }

            curNode = *activeHead;
            curJob = &jobs[curNode->data];

            // Move head forward
            *activeHead = curNode->next;
            curNode->next = NULL;

            if (curJob->response == -1) {
                curJob->start = current_time;
                curJob->response = curJob->start - curJob->arrival_time;
            }

            current_time++;
            curJob->remaining_time--;
            curJob->alloted_left--;
            reset_timer--;

            if (curJob->remaining_time <= 0) {
                // finished
                curJob->end = current_time;
                curJob->turnaround = curJob->end - curJob->arrival_time;
                curJob->inQueue = false;
                remaining_jobs--;
                free(curNode);
            } else if (curJob->alloted_left <= 0) {
                // degrade to next queue
                break;
            } else {
                // put back to end of same queue
                if (*activeTail == NULL) {
                    *activeHead = *activeTail = curNode;
                } else {
                    (*activeTail)->next = curNode;
                    *activeTail = curNode;
                }
            }
            printf("20 units over inside if and else block");
        }
    }
}

// Failed Attempt to better algorithm... too complex dueto multiple loops
void MLFQ2(struct Job jobs[], int len) {
    struct Node *head = NULL, *tail = NULL;
    struct Node *head2 = NULL, *tail2 = NULL;

    int current_time = 0;
    int quantum_time = 20;
    int reset_timer = 100;
    int remaining_jobs = len;
    int *reset;
    bool isBreak = false;

    while (remaining_jobs > 0) {
        if (reset_timer <= 0) {
            reset = &reset_timer;
            printf("Resetting\n");
            clearQueue(&head, &tail, &head2, &tail2, reset);
            enqueueReset(jobs, &head, &tail, len);
        } else {
            enqueueFirst(jobs, &head, &tail, len, current_time);

            if (head == NULL && head2 == NULL) {
                current_time++;
                reset_timer--;
            }

            struct Node *current_node;
            struct Job *current_job;
            bool isBreak = false;

            while (head != NULL) {
                printf("\n[DEBUG] --- Starting top of outer while (head queue) ---\n");
                printList(head);

                enqueueFirst(jobs, &head, &tail, len, current_time);

                if (head != NULL && head->next != NULL && jobs[tail->data].response == -1) {
                    struct Node *temp;
                    temp = head;
                    head = head->next;
                    temp->next = NULL;
                    tail->next = temp;
                    tail = temp;
                }

                current_node = head;
                current_job = &jobs[current_node->data];

                if (current_job->response == -1) {
                    current_job->start = current_time;
                    current_job->response = current_job->start - current_job->arrival_time;
                }

                quantum_time = 20;

                printf("[DEBUG] Entering inner while for Job ID %d with quantum_time = %d\n",
                       current_node->data, quantum_time);

                while (quantum_time > 0) {
                    current_time++;
                    reset_timer--;
                    quantum_time--;
                    current_job->alloted_left--;
                    current_job->remaining_time--;

                    if (current_job->alloted_left == 0) {
                        isBreak = true;
                        printf("\n[DEBUG] Job %d used up its allotted time slice. Moving to lower queue.\n",
                               current_node->data);
                        printList(head);
                        printf("[DEBUG] Current lower queue (before move):\n");
                        printList(head2);
                        printf("\n");

                        current_job->alloted_left = current_job->alloted_time;

                        if (head->next != NULL)
                            head = head->next;
                        else
                            head = NULL, tail = NULL;

                        if (tail2 != NULL) {
                            tail2->next = current_node;
                            tail2 = current_node;
                        } else {
                            head2 = tail2 = current_node;
                        }

                        current_node->next = NULL;

                        printf("[DEBUG] Job %d moved successfully to lower queue.\n", current_node->data);
                        printf("[DEBUG] Updated high-priority queue:\n");
                        printList(head);
                        printf("[DEBUG] Updated lower-priority queue:\n");
                        printList(head2);
                        printf("\n");
                        break;
                    } else if (current_job->remaining_time == 0) {
                        isBreak = true;
                        if (head->next != NULL)
                            head = head->next;
                        else
                            head = NULL, tail = NULL;
                        current_job->end = current_time;
                        current_job->turnaround = current_job->end - current_job->arrival_time;
                        current_job->inQueue = false;
                        remaining_jobs--;
                        current_node->next = NULL;
                        free(current_node);
                        break;
                    } else {
                        printf("[DEBUG] Job %d still running... quantum_time left: %d\n",
                               current_node->data, quantum_time);
                    }
                }
                if (isBreak)
                    break;
                if (current_node == head || head == NULL)
                    break;

                printf("[DEBUG] End of inner while for Job ID %d\n", current_node->data);
                printList(head);

                current_node->next = NULL;
                tail->next = current_node;
                tail = current_node;

                printf("[DEBUG] Job %d moved to end of same queue (Round Robin rotation)\n",
                       current_node->data);
                printf("[DEBUG] Updated queue after rotation:\n");
                printList(head);
            }

            printf("[DEBUG] End of head queue processing.\n");

            while (head == NULL && head2 != NULL) {
                printf("\n[DEBUG] --- Starting top of outer while (head2 queue) ---\n");
                printList(head2);

                enqueueFirst(jobs, &head, &tail, len, current_time);

                if (head2 != NULL && head2->next != NULL && jobs[tail2->data].response == -1) {
                    struct Node *temp;
                    temp = head2;
                    head2 = head2->next;
                    temp->next = NULL;
                    tail2->next = temp;
                    tail2 = temp;
                }
                current_node = head2;
                current_job = &jobs[current_node->data];

                if (current_job->response == -1) {
                    current_job->start = current_time;
                    current_job->response = current_job->start - current_job->arrival_time;
                }

                quantum_time = 20;

                printf("[DEBUG] Entering inner while for Job ID %d with quantum_time = %d in lower priority queue\n",
                       current_node->data, quantum_time);

                while (quantum_time > 0) {
                    current_time++;
                    reset_timer--;
                    quantum_time--;
                    current_job->alloted_left--;
                    current_job->remaining_time--;

                    if (reset_timer <= 0) {
                        isBreak = true;
                        reset = &reset_timer;
                        printf("Resetting\n");
                        clearQueue(&head, &tail, &head2, &tail2, reset);
                        enqueueReset(jobs, &head, &tail, len);
                        break;
                    }

                    if (current_job->alloted_left <= 0) {
                        isBreak = true;
                        printf("\n[DEBUG] Job %d used up its allotted time slice. Moving to lower queue.\n",
                               current_node->data);
                        printList(head);
                        printf("[DEBUG] Current lower queue (before move):\n");
                        printList(head2);
                        printf("\n");

                        current_job->alloted_left = current_job->alloted_time;

                        if (head2->next != NULL)
                            head2 = head2->next;
                        else
                            head2 = NULL, tail2 = NULL;

                        if (tail2 != NULL) {
                            tail2->next = current_node;
                            tail2 = current_node;
                        } else {
                            head2 = tail2 = current_node;
                        }

                        current_node->next = NULL;

                        printf("[DEBUG] Job %d moved successfully to lower queue.\n", current_node->data);
                        printf("[DEBUG] Updated high-priority queue:\n");
                        printList(head);
                        printf("[DEBUG] Updated lower-priority queue:\n");
                        printList(head2);
                        printf("\n");
                        break;
                    } else if (current_job->remaining_time <= 0) {
                        isBreak = true;
                        if (head2->next != NULL)
                            head2 = head2->next;
                        else
                            head2 = NULL, tail2 = NULL;
                        current_job->end = current_time;
                        current_job->turnaround = current_job->end - current_job->arrival_time;
                        current_job->inQueue = false;
                        remaining_jobs--;
                        current_node->next = NULL;
                        free(current_node);
                        break;
                    } else {
                        printf("[DEBUG] Job %d still running... quantum_time left: %d with %d time left and %d reset\n",
                               current_node->data, quantum_time, current_job->remaining_time, reset_timer);
                        printf("end");
                    }
                }
                printList(head2);
                if (isBreak)
                    break;
                if (current_node == head2 || head2 == NULL)
                    break;

                printf("[DEBUG] End of inner while for Job ID %d\n", current_node->data);
                printList(head2);

                current_node->next = NULL;
                tail2->next = current_node;
                tail2 = current_node;

                printf("[DEBUG] Job %d moved to end of same queue (Round Robin rotation)\n",
                       current_node->data);
                printf("[DEBUG] Updated queue after rotation:\n");
                printList(head2);
            }
        }
    }
    printf("completed run");
}
// Attemp-03 with better approach and complexity
void MLFQ3(struct Job jobs[], int len, int quantum, int resett) {
    struct Node *head = NULL, *tail = NULL;
    struct Node *head2 = NULL, *tail2 = NULL;
    struct Job *prev = NULL; 

    int current_time = 0;
    int quantum_time = quantum;
    int reset_timer = resett;
    int remaining_jobs = len;
    int *reset;
    bool isBreak = false;
    enqueueFirst(jobs, &head, &tail, len, current_time);

    // printf("=== MLFQ3 START ===\n");

    while (remaining_jobs > 0) {
        struct Node *currentNode;
        struct Job *job;
        struct Node *temp;
        struct Node **activeHead, **activeTail;
        quantum_time = quantum;

        if (head == NULL && head2 == NULL) {
            // printf("[TIME %d] Both queues empty — advancing time.\n", current_time);
            current_time++;
            enqueueFirst(jobs, &head, &tail, len, current_time);
            continue;
        }

        if (head != NULL) {
            currentNode = head;
            activeHead = &head;
            activeTail = &tail;
            job = &jobs[currentNode->data];
            // printf("[TIME %d] Running job %d from Q1\n", current_time, currentNode->data);
        } else if (head2 != NULL) {
            currentNode = head2;
            activeHead = &head2;
            activeTail = &tail2;
            job = &jobs[currentNode->data];
            // printf("[TIME %d] Running job %d from Q2\n", current_time, currentNode->data);
        }

        if (prev != job) {
        if (prev != NULL) {
            timeline[tcount - 1].end = current_time;
        }
        timeline[tcount++] = (struct Interval){
            job->serial,
            current_time,
            -1
        };
    }
    prev = job;

        if (job->response == -1) {
            job->start = current_time;
            job->response = job->start - job->arrival_time;
            // printf("Job %d first response: start=%d response=%d\n",currentNode->data, job->start, job->response);
        }

        while (quantum_time > 0) {
            current_time++;
            reset_timer--;
            quantum_time--;
            job->alloted_left--;
            job->remaining_time--;
            isBreak = false;

            reset = &reset_timer;

            enqueueFirst(jobs, &head, &tail, len, current_time);

            if (reset_timer == 0) {
                // printf("\n=== RESETTING at TIME %d ===\n", current_time);
                // printf("Rest time Now: %d\n", reset_timer);
                clearQueue(&head, &tail, &head2, &tail2, reset);
                // printf("Rest time Now: %d\n", reset_timer);
                // printList(head);
                enqueueReset(jobs, &head, &tail, len);
                reset_timer = resett;
                // printf("=== RESET DONE ===\n\n");
            }

            if (job->remaining_time <= 0) {
                isBreak = true;
                temp = currentNode;
                job->end = current_time;
                job->turnaround = job->end - job->arrival_time;
                job->inQueue = false;

                // printf("[TIME %d] Job %d completed | turnaround=%d end=%d\n", current_time, currentNode->data, job->turnaround, job->end);

                if (currentNode->next == NULL)
                    (*activeHead) = (*activeTail) = NULL;
                else {
                    (*activeHead) = (*activeHead)->next;
                    temp->next = NULL;
                }
                remaining_jobs--;
                timeline[tcount - 1].end = current_time;
                free(currentNode);
                // printf("Remaining jobs: %d\n", remaining_jobs);
            } else if (job->alloted_left <= 0) {
                // printf("[TIME %d] Job %d exhausted its quantum in Q1 — demoting to Q2\n",
                    //    current_time, currentNode->data);
                if (currentNode == head) {
                    temp = head;
                    head = head->next;
                    temp->next = NULL;
                    if (tail2 == NULL)
                        head2 = tail2 = currentNode;
                    else {
                        tail2->next = temp;
                        tail2 = temp;
                        temp = NULL;
                    }
                } else {
                    if ((*activeHead)->next == NULL) {
                        // printf("ok");
                    } else {
                        (*activeHead) = (*activeHead)->next;
                        currentNode->next = NULL;
                        (*activeTail)->next = currentNode;
                        (*activeTail) = currentNode;
                    }
                }
                job->alloted_left = job->alloted_time;
                timeline[tcount - 1].end = current_time;
                isBreak = true;
            }

            if (isBreak)
                break;
        }

        // printf("\nPrinting List(head): ");
        // printList(head);
        // printf("\n");
        // printf("\nPrinting List(head2): ");
        // printList(head2);
        // printf("\n");

        if (isBreak) {
            // printf("[TIME %d] Breaking inner loop for job %d\n\n", current_time, job->serial);
            continue;
        }

        if ((*activeHead)->next == NULL) {
            // printf("[TIME %d] Only one node in active queue — skipping rotation.\n\n", current_time);
            continue;
        }
        timeline[tcount - 1].end = current_time;
        // printf("[TIME %d] Rotating job %d to end of active queue\n", current_time, currentNode->data);
        (*activeHead) = currentNode->next;
        currentNode->next = NULL;
        (*activeTail)->next = currentNode;
        (*activeTail) = currentNode;
        // printf("Rotation done for job %d\n\n", currentNode->data);
    }

    // printf("=== MLFQ3 END ===\n");
    if (tcount > 0) {
        timeline[tcount - 1].end = current_time;
    }
    createJson(jobs, len);
}
