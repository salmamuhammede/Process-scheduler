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
void handler();

struct PCB current;
priorityQueue Ready;

int main(int argc, char * argv[])
{
    signal(SIGUSR1,handler);
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
    initClk();
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    initQueue(&Ready);
    printf("\nreceived %d \n",msqid);
    int p_num = atoi(argv[3]);
    printf("%d\n",p_num);
    while(p_num)
    {
            if ((msgrcv(msqid, &messagebefore, sizeof(messagebefore.send), 0, IPC_NOWAIT) != -1) && messagebefore.send.pid != -1)
        
        // if (rec_val == -1)
        //     perror("error in recieve");
        {
            p_num--;
            writeToFile("output.txt",messagebefore.send.pid, getClk());
            SelectedAlgo(atoi(argv[1]),messagebefore.send);
            // printf("hallo");
           // printf("\nMessage received: %d\n", messagebefore.send.pid);
            
        }else{
            
        }
    }
   // destroyClk(true);
    return 0;
}
void SelectedAlgo(int x , struct PCB com){
    if (x == 1){
        enqueue(&Ready,com,com.arrivaltime);
    }
        // RR

    if (x == 2){
        enqueue(&Ready,com,com.priority);
    }
        //HPF

    if (x == 3){
        enqueue(&Ready,com,com.remainingtime);
    }
        //SRTN

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
void writeStringToFile(const char* filename, const char* str)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    fprintf(file, "%s", str);

    fclose(file);
}
void handler(){
    char str[100];
     snprintf("At time %d process %d finished arr %d total %d remain %d wait 3 \n",getClk(),current.pid,current.arrivaltime,current.runningtime,current.remainingtime);
     writeStringToFile("scheduler.log",str);
} 