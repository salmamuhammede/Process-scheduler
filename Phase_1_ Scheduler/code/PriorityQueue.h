#include<PCB.h>
#include<headers.h>
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

void initQueue(priorityQueue *comming)
{  
    comming->tail = nullptr;
    comming->head = nullptr;
    comming->length=0;
  
}_

/*bool enqueue(priorityQueue *comming,PCB process,int chosen)
{
    node * currentProcess=(node *)malloc(sizeof(node));
    currentProcess->PCB=process;
    currentProcess->alg=chosen;    
    currentProcess->next=nullptr;
    if(comming->length==0&&comming->tail==nullptr&&comming->tail==nullptr)
    {
        comming->head=comming->tail=currentProcess;

    }
    else if(comming->head->alg>currentProcess->alg)
    {
        currentProcess->next=comming->head;
        comming->head=curentProcess;
    }
    else{
        node *pointer=comming->head;
        while(pointer&&pointer->next&&currentProcess->alg>pointer>next->alg)
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

    }
}*/