#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "headers.h"
#include "PCB.h"

typedef struct node{
struct PCB P;
int alg;
struct node * next;
}node;
typedef struct priorityQueue{
    node *head;
    node *tail;
    int length;
}priorityQueue;

void initQueue(priorityQueue * comming)
{  
    comming->tail = NULL;
    comming->head = NULL;
    comming->length=0;
  
}


bool enqueue(priorityQueue *comming,struct PCB process,int chosen)
{
    node * currentProcess=(node *)malloc(sizeof(node));
    currentProcess->P=process;
    currentProcess->alg=chosen;    
    currentProcess->next=NULL;
    if(comming->length==0&&comming->tail==NULL&&comming->tail==NULL)
    {
        comming->head=comming->tail=currentProcess;

    }
    else if(comming->head->alg>currentProcess->alg)
    {
        currentProcess->next=comming->head;
        comming->head=currentProcess;
    }
    else{
        node *pointer=comming->head;
        while(pointer!=NULL && pointer->next !=NULL && currentProcess->alg > pointer->next->alg)
        {
            pointer=pointer->next;
        }
        currentProcess->next=pointer->next;
        pointer->next=currentProcess;

    }
    comming->length++;
    return true;
}
struct PCB dequeu(priorityQueue *comming)
{
    struct PCB j;
    if(comming->length==0)
    {
        printf("Empty Queue\n");
        j.pid=5;
        return j;
    } 
    else {
        node* temp=comming->head;
        j=temp->P; 
        comming->head=comming->head->next;
        free(temp);
        comming->length--;
    }
    return j;
}
struct PCB top(priorityQueue *comming)
{
    if(isEmpty(comming))
    {
        printf("The quueu is empty\n");
        struct PCB L;
        L.pid=-1;
        return L;
    }
    else
    {
        return comming->head->P;
    }
}
int isEmpty(priorityQueue * comming)
{
    return (comming->length==0);
}
#endif