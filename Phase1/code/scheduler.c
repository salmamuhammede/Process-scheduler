// #include "headers.h"
#include "PCB.h"
#include "PriorityQueue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
struct msgbuff
{
    long msgtype;
    struct PCB send;
};
char str[100];
void writeToFile(const char *filename, int num1, int num2);
void handler(int signum);
void SelectedAlgo(int x, struct PCB com);
void writeStringToFile(const char *filename, const char *str);
struct PCB current;
struct PCB dummy;
int p_num;
int cur_pid;
priorityQueue Ready;
void HPF();
int main(int argc, char *argv[])
{
    current.state = 0;
    int cur_algo = atoi(argv[1]);
    signal(SIGUSR1, handler);
    key_t key = ftok("keyfile", 's');
    if (key == -1)
    {
        perror("Error in ftok");
        exit(-1);
    }
    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (key == -1)
    {
        perror("Error in creating ids");
        exit(-1);
    }
    int rec_val;
    struct msgbuff messagebefore;
    initClk();

    // TODO implement the scheduler :)
    // upon termination release the clock resources.
    initQueue(&Ready);
    printf("\nreceived %d \n", msqid);
    p_num = atoi(argv[3]);
    printf("%d\n", p_num);
    printf("%d\n", atoi(argv[1]));
    // p_num++;
    int clock = getClk();
    while (p_num)
    {
        sleep(.1); // to reduce lag
        while ((msgrcv(msqid, &messagebefore, sizeof(messagebefore.send), 0, IPC_NOWAIT) != -1) && messagebefore.send.pid != -1)
        {
            // p_num--;

            SelectedAlgo(cur_algo, messagebefore.send);
            writeToFile("output.txt", messagebefore.send.pid, getClk());
            //printQueue(&Ready);
            // printf("hallo");
            // printf("\nMessage received: %d\n", messagebefore.send.pid);
        }
        if      (cur_algo == 2)
            HPF();
        else if (cur_algo == 3)
            SRTN();
    }

    printf("complete\n");
    destroyClk(true);
    return 0;
}
void SelectedAlgo(int x, struct PCB com)
{
    if (x == 1)
    {
        enqueue(&Ready, com, com.arrivaltime);
    }
    // RR

    if (x == 2)
    {
        enqueue(&Ready, com, com.priority);
    }
    // HPF

    if (x == 3)
    {
        enqueue(&Ready, com, com.remainingtime);
    }
    // SRTN
}
void HPF()
{
    if (current.state != 1 && !isEmpty(&Ready))
    {

        current = dequeu(&Ready);
        current.state = 1;
        int wait = getClk() - current.arrivaltime;
        snprintf(str, sizeof(str), "At time %d process %d started arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, wait);
        writeStringToFile("scheduler.log", str);
        int pid = fork();
        if (pid == -1)
        {
            perror("error in forking ");
            exit(-1);
        }

        else if (pid == 0)
        {
            char timeStr[20]; // assuming a reasonable size
            snprintf(timeStr, sizeof(timeStr), "%d", current.remainingtime);
            char *args[] = {"process.out", timeStr, NULL};
            execv("process.out", args);
        }
        // int stat;
        // waitpid(-1, &stat, 0); // Waiting for the child process to finish

        printf("hallo\n");
        return;
    }
}
int f_b =0;
int clock =-1;
void SRTN()
{    
    // if (!f_b)
    // {
    //     current=top(&Ready);
    //     f_b =1;
    // }
    if (current.state != 1 && !isEmpty(&Ready))
    {
        current = dequeu(&Ready);
        current.state = 1;
        int wait = getClk() - current.arrivaltime;
        if (current.runningtime == current.remainingtime)
        {
            snprintf(str, sizeof(str), "At time %d process %d started arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, wait);
            writeStringToFile("scheduler.log", str);
                    int pid = fork();
        if (pid == -1)
        {
            perror("error in forking ");
            exit(-1);
        }

        else if (pid == 0)
        {
            char timeStr[20]; // assuming a reasonable size
            snprintf(timeStr, sizeof(timeStr), "%d", current.remainingtime);
            char *args[] = {"process.out", timeStr, NULL};
            execv("process.out", args);
        }else
            cur_pid=pid;
        }else{
            snprintf(str, sizeof(str), "At time %d process %d resumed arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, wait);
            writeStringToFile("scheduler.log", str);
            kill(cur_pid,SIGUSR1);
        }
    
        // int stat;
        // waitpid(-1, &stat, 0); // Waiting for the child process to finish

        printf("hallo\n");
        return;
    }else if (current.state == 1 && !isEmpty(&Ready) && clock!= getClk())
    {
        printf("\n -- \n");
        dummy = top(&Ready);
        if (dummy.remainingtime<current.remainingtime)
        {
            kill(cur_pid,SIGUSR1);
            enqueue(&Ready,current,current.remainingtime);
            snprintf(str, sizeof(str), "\n At time %d process %d Stopped arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, wait);
            writeStringToFile("scheduler.log", str);
            current=dummy;
            clock = getClk();
            return;
        }
        current.remainingtime--;
        clock = getClk();
    }
    
}

void writeToFile(const char *filename, int num1, int num2)
{
    // Open the file in write mode. Use "a" mode to append if you don't want to overwrite existing data.
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Write the two integers to the file
    fprintf(file, "%d %d\n", num1, num2);

    // Close the file
    fclose(file);
}
void writeStringToFile(const char *filename, const char *str)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    fprintf(file, "%s", str);

    fclose(file);
}
void handler(int signum)
{
    char str[100];
    snprintf(str, sizeof(str), "At time %d process %d finished arr %d total %d remain %d wait 3 \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime);
    writeStringToFile("scheduler.log", str);
    p_num--;
    current.state = 2;
    printf("handler finished\n");
    // printQueue(&Ready);
}