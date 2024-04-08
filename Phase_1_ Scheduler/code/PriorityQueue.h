#ifndef PRIORITYQUEUE.H
#define PRIORITYQUEUE.H
#include "PCB.h"
#include "headers.h"
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


bool enqueue(priorityQueue *comming,PCB process,int chosen)
{
    node * currentProcess=(node *)malloc(sizeof(node));
    currentProcess->P=process;
    currentProcess->alg=chosen;    
    currentProcess->next=nullptr;
    if(comming->length==0&&comming->tail==nullptr&&comming->tail==nullptr)
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
bool dequeu(priorityQueue *comming)
{
    if(comming->length==0)
    {
        printf("Empty Queue\n");
        return false;
    } 
    else {
        node* temp=comming->head;
        comming->head=comming->head->next;
        free(temp);
        comming->length--;
    }
    return true;
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
bool isEmpty(priorityQueue * comming)
{
    return (comming->length==0);
}
#endif