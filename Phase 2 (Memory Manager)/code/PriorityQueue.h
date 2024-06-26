#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "headers.h"
#include "PCB.h"
#include "Tree.h"
typedef struct node
{
    struct PCB P;
    int alg;
    struct node *next;
} node;
typedef struct priorityQueue
{
    node *head;
    node *tail;
    int length;
} priorityQueue;

void initQueue(priorityQueue *comming)
{
    comming->tail = NULL;
    comming->head = NULL;
    comming->length = 0;
}

int findQuick(priorityQueue *comming, struct PCB process)
{
    if (isEmpty(comming) == 1)
        return 0;
    node *pointer = comming->head;
    while (pointer != NULL && pointer->next != NULL && process.pid != pointer->P.pid && process.arrivaltime != pointer->P.arrivaltime)
    {
        /// printf("headpid %d\n",pointer->P.pid);
        pointer = pointer->next;
    }
    if (pointer == NULL)
        return 0;
    else
    {
        printf("headpid %d\n", pointer->P.pid);
        return 1;
    }
}
int enqueue(priorityQueue *comming, struct PCB process, int chosen)
{
    node *currentProcess = (node *)malloc(sizeof(node));
    currentProcess->P = process;
    currentProcess->alg = chosen;
    currentProcess->next = NULL;
    if (comming->length == 0 && comming->tail == NULL && comming->tail == NULL)
    {
        comming->head = comming->tail = currentProcess;
    }
    else if (comming->head->alg > currentProcess->alg)
    {
        currentProcess->next = comming->head;
        comming->head = currentProcess;
    }
    else
    {
        node *pointer = comming->head;
        while (pointer != NULL && pointer->next != NULL && currentProcess->alg >= pointer->next->alg)
        {
            pointer = pointer->next;
        }
        currentProcess->next = pointer->next;
        pointer->next = currentProcess;
    }
    comming->length++;
    return 1;
}
// int eneque_Ready(priorityQueue *waiting, priorityQueue *ready, int chosen, Tree *t)
// {
//     if (waiting->head == NULL)
//     {
//         printf("zzzzzzzzzzyead\n");
//         return 0;
//     }
//     else
//     {

//         node *currentProcess = (node *)malloc(sizeof(node));
//         currentProcess = waiting->head;
//         node *prev = (node *)malloc(sizeof(node));
//         prev = NULL;
//         while (1)
//         {
//             printf("kareem\n");
//             if (currentProcess != NULL)
//             {
//                 printf("zeyad\n");
//                 if (insertElement(currentProcess->P, t, t->root) == 1)
//                 {
//                     if (currentProcess->P.pid == waiting->head->P.pid)
//                     {
//                         enqueue(ready, currentProcess->P, chosen);
//                         waiting->head = currentProcess->next;
//                     }
//                     else
//                     {
//                         enqueue(ready, currentProcess->P, chosen);
//                         prev->next = currentProcess->next;
//                         currentProcess = NULL;
//                     }
//                     printf("rrrrrrrrrrrrr");

//                     return 1;
//                 }
//                 else
//                 {
//                     prev = currentProcess;
//                     currentProcess = currentProcess->next;
//                 }
//                 printf("eneque ready");
//             }
//             else
//             {
//                 return 0;
//             }
//         }
//     }
// }
int enqueue_priority(priorityQueue *comming, struct PCB process)
{
    node *currentProcess = (node *)malloc(sizeof(node));
    if (currentProcess == NULL)
    {
        perror("Failed to allocate memory for new node");
        return -1; // return -1 to indicate error in memory allocation
    }

    currentProcess->P = process;
    currentProcess->next = NULL;

    if (comming->head == NULL)
    {
        // Queue is empty
        comming->head = comming->tail = currentProcess;
    }
    else
    {
        // Find the correct insertion point considering priority and arrival time
        node *pointer = comming->head;
        node *previous = NULL;

        while (pointer != NULL && (currentProcess->P.priority < pointer->P.priority ||
                                   (currentProcess->P.priority == pointer->P.priority && currentProcess->P.arrivaltime > pointer->P.arrivaltime)))
        {
            previous = pointer;
            pointer = pointer->next;
        }

        if (previous == NULL)
        {
            // Insert at head
            currentProcess->next = comming->head;
            comming->head = currentProcess;
        }
        else
        {
            // Insert in middle or at tail
            previous->next = currentProcess;
            currentProcess->next = pointer;
        }
    }

    comming->length++;
    return 1;
}

void printQueue(priorityQueue *queue)
{
    const node *current = queue->head; // Start with the head of the queue

    while (current != NULL)
    {
        printf("%d    %d\n", current->P.pid, current->P.begin);
        current = current->next; // Move to the next node
    }
}
struct PCB dequeu(priorityQueue *comming)
{
    struct PCB j;
    if (comming->length == 0)
    {
        printf("Empty Queue\n");
        j.pid = -1;
        return j;
    }
    else
    {
        node *temp = comming->head;
        j = temp->P;
        comming->head = comming->head->next;
        comming->length--;
        free(temp);

        // Update tail if head becomes NULL
        if (comming->head == NULL)
        {
            comming->tail = NULL;
        }

        return j;
    }
}

struct PCB top(priorityQueue *comming)
{
    if (isEmpty(comming) == 1)
    {
        printf("The quueu is empty\n");
        struct PCB L;
        L.pid = -1;
        return L;
    }
    else
    {
        return comming->head->P;
    }
}
int isEmpty(priorityQueue *comming)
{
    return (comming->length == 0);
}
int enqueuelast(priorityQueue *comming, struct PCB process)
{
    node *currentProcess = (node *)malloc(sizeof(node));
    currentProcess->P = process;
    currentProcess->alg = 0;
    currentProcess->next = NULL;
    if (comming->length == 0 && comming->head == NULL && comming->tail == NULL)
    {
        comming->head = comming->tail = currentProcess;
    }

    else
    {
        node *pointer = comming->head;
        while (pointer != NULL && pointer->next != NULL)
        {
            pointer = pointer->next;
        }
        currentProcess->next = pointer->next;
        pointer->next = currentProcess;
    }
    comming->length++;
    return 1;
}

#endif