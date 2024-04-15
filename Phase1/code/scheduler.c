//#include "headers.h"
#include "PCB.h"
#include "PriorityQueue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
 struct msgbuff
{
    long msgtype;
    //struct PCB send;'char
    char text[256];
};
int main(int argc, char * argv[])
{
    key_t key=ftok("keyfile",'s');
    if (key == -1)
    {
        perror("Error in ftok");
        exit(-1);
    }
    int msqid=msgget(key,0666|IPC_CREAT);
    if (key == -1)
    {
        perror("Error in creating ids");
        exit(-1);
    } 
    int rec_val;
    struct msgbuff messagebefore;
    //initClk();
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    printf("\nreceived %d \n",msqid);
    for (int i = 0; i < 7; i++)
    {
            rec_val = msgrcv(key, &messagebefore, 256, 0, !IPC_NOWAIT);
        if (rec_val == -1)
            perror("Error in receive");
        else
            printf("\nMessage received: %s\n", messagebefore.text);
    }
    //destroyClk(true);
    return 0;
}
