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
    struct PCB send;
};
void writeToFile(const char *filename, int num1, int num2);
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
   // initClk();
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    
    printf("\nreceived %d \n",msqid);
    int p_num = atoi(argv[3]);
    printf("%d\n",p_num);
    for (int i = 0; i < p_num; i++)
    {
         //writeToFile("output.txt", 1, getClk());
            rec_val = msgrcv(msqid, &messagebefore, sizeof(messagebefore.send), 0, !IPC_NOWAIT);
        if (rec_val == -1)
            perror("error in recieve");
        else
        {
            writeToFile("output.txt",messagebefore.send.pid, getClk());
           // printf("\nMessage received: %d\n", messagebefore.send.pid);
            
        }
    }
   // destroyClk(true);
    return 0;
}

void writeToFile(const char *filename, int num1, int num2) {
    // Open the file in write mode. Use "a" mode to append if you don't want to overwrite existing data.
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write the two integers to the file
    fprintf(file, "%d %d\n", num1, num2);

    // Close the file
    fclose(file);

   
}