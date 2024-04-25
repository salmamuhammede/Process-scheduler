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
#include <math.h>

struct msgbuff
{
    long msgtype;
    struct PCB send;
};
void writeToFile(const char *filename, int num1, int num2);
void handler(int signum);
void SelectedAlgo(int x, struct PCB com);
void writeStringToFile(const char *filename, const char *str);
void clearResources();
void HPF();
void RR();
void SRTN();
////////////////////////////////
struct PCB current;
struct PCB rec;
struct PCB dummy;
char str[100];
int p_num, Ori_p_num;
int shmidd;
int *shmaddrs;
priorityQueue Ready;

int quantum;
int Final_time, TA, Total_running = 0;
float WTA, Avg_WTA = 0, Avg_waiting = 0, STD_WTA;
float *WTAArr;
int cur_algo;
struct msgbuff messagebefore;
int msqid;
int main(int argc, char *argv[])
{
    current.state = 0;
    cur_algo = atoi(argv[1]);    
    quantum = atoi(argv[2]);
    ///////////////////////////
    signal(SIGUSR1, handler);
    /////////////////////////////////////////////
    key_t keymem = ftok("RT", 'r');
    shmidd = shmget(keymem, 4, IPC_CREAT | 0644);
    if (shmidd == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    shmaddrs = shmat(shmidd, (void *)0, 0);
    ////////////////////////////////////msqqueue
    key_t key = ftok("keyfile", 's');
    if (key == -1)
    {
        perror("Error in ftok");
        exit(-1);
    }
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (key == -1)
    {
        perror("Error in creating ids");
        exit(-1);
    }
    int rec_val;
     p_num = atoi(argv[3]);
    Ori_p_num = p_num;
    WTAArr = (float *)malloc(p_num * sizeof(int));
    printf("%d\n", p_num);
    printf("%d\n", atoi(argv[1]));

    initClk();
    // TODO implement the scheduler :)
    // upon termination release the clock resources.
    initQueue(&Ready);
    printf("\nreceived %d \n", msqid);
   
    system("rm scheduler.log");
    system("rm scheduler.perf");
    
    int clock = getClk();
    int time = -1;
    int flag = 0;

    /*while (p_num)
    {
        // sleep(.1); // to reduce lag
        int num = 0;
        while (1)
        {
            msgrcv(msqid, &messagebefore, sizeof(messagebefore.send), 0, !IPC_NOWAIT);

            if (messagebefore.send.pid != -1 && messagebefore.send.pid != -2)
            // p_num--;

            {
                SelectedAlgo(cur_algo, messagebefore.send);
                writeToFile("output.txt", messagebefore.send.pid, getClk());
            }
            if (messagebefore.send.pid == -2)
            {
                flag = 1;
                break;
            }
            if (cur_algo == 2)
                HPF();
            else if (cur_algo == 3)
                SRTN();
            else if (cur_algo == 1)
            {
                RR();
            }
            num++;
            if (messagebefore.send.pid == -1)
            {
                if (num == 1)
                {
                    printf("%d", num);
                    break;
                }
            }
            // printQueue(&Ready);
            //  printf("hallo");
            //  printf("\nMessage received: %d\n", messagebefore.send.pid);
        }
        if (flag == 1)
        {
            if (cur_algo == 2)
                HPF();
            else if (cur_algo == 3)
                SRTN();
            else if (cur_algo == 1)
            {
                RR();
            }
        }
    }*/
    ////////////////////////////////////////////////->>>>>>>>>>>>>>>>>>>>>>>>>
    while(p_num)
    {
        if(time!=getClk)
        {
            time=getClk();
            while(msgrcv(msqid, &messagebefore, sizeof(messagebefore.send), 0, IPC_NOWAIT)!=-1)
            {
                rec=messagebefore.send;
                if(rec.pid!=-1){
                SelectedAlgo(cur_algo, rec);
                writeToFile("output.txt", rec.pid, getClk());
                printf("********************************\n");
                printQueue(&Ready);
                printf("********************************\n");
                }
            }
            if (cur_algo == 2)
                HPF();
            else if (cur_algo == 3)
                SRTN();
            else if (cur_algo == 1)
            {
                RR();
            }
        }
    }
/////////////////////////////////////////////////////////////////////
    Final_time = getClk();
    Finish();
    clearResources();
    kill(getpgrp,SIGINT);
    printf("complete\n");    
    destroyClk(true);
    return 0;
}
void SelectedAlgo(int x, struct PCB com)
{
    if (x == 1)
    {
        enqueue(&Ready, com, quantum);
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
    if (current.state != 1 )
    {
        if(isEmpty(&Ready)==1)
        return;
        
        current = dequeu(&Ready);
        current.state = 1;
        int wait = getClk() - current.arrivaltime;
        current.waitingtime = wait;
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
            char *args[] = {"process.out", timeStr, 0, NULL};
            execv("process.out", args);
        }
        // int stat;
        // waitpid(-1, &stat, 0); // Waiting for the child process to finish
        else
        {
            current.acc_pid=pid;
        }

        //printf("hallo\n");
        return;
    }
}
int f_b = 0;
int clock = -1;
void SRTN()
{
    // if (!f_b)
    // {
    //     current=top(&Ready);
    //     f_b =1;
    // }
    // if (current.state == 1 && current.remainingtime != *shmaddrs)
    // {
    //     printf("\nRT for %d : %d \n", current.acc_pid, *shmaddrs);
    //     current.remainingtime = *shmaddrs;
    // }
    if ((current.state != 1) && !isEmpty(&Ready))
    {
        current = dequeu(&Ready);
        printf("hello : %d\n", current.state);
        if (current.runningtime == current.remainingtime)
        {
            current.state = 1;
            int wait = getClk() - current.arrivaltime;
            current.waitingtime = wait;
            snprintf(str, sizeof(str), "At time %d process %d started arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, wait);
            writeStringToFile("scheduler.log", str);
            *shmaddrs = current.remainingtime;
            int pid = fork();
            if (pid == -1)
            {
                perror("error in forking ");
                exit(-1);
            }

            else if (pid == 0)
            {
                char flag[10], timeStr[20]; // assuming a reasonable size
                snprintf(timeStr, sizeof(timeStr), "%d", current.remainingtime);
                snprintf(flag, sizeof(flag), "%d", 1);
                char *args[] = {"process.out", timeStr, flag, NULL};
                printf("forked");
                printf(args);
                execv("process.out", args);
            }
            else
                current.acc_pid = pid;
        }
        else if (current.state == 3)
        {
            current.waitingtime += getClk() - current.last;
            snprintf(str, sizeof(str), "At time %d process %d resumed arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, current.waitingtime);
            writeStringToFile("scheduler.log", str);
            current.state = 1;
            *shmaddrs = current.remainingtime;
            kill(current.acc_pid, SIGCONT);
        }
        return;
    }
    else if (current.state == 1 && !isEmpty(&Ready) && current.remainingtime != *shmaddrs)
    {
        dummy = top(&Ready);
        current.remainingtime = *shmaddrs;
        // printf("\n- %d -\n",current.remainingtime);
        if (dummy.remainingtime < current.remainingtime)
        {
            kill(current.acc_pid, SIGSTOP);
            current.state = 3;
            current.last = getClk();
            enqueue(&Ready, current, current.remainingtime);
            snprintf(str, sizeof(str), "At time %d process %d Stopped arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, current.waitingtime);
            writeStringToFile("scheduler.log", str);
            current = dummy;
            printQueue(&Ready);
            return;
        }
    }
}
void RR()
{
    if ((current.state != 1) && !isEmpty(&Ready))
    {
        current = dequeu(&Ready);
        printf("hello : %d\n", current.state);
        if (current.runningtime == current.remainingtime) // means at the beggining only
        {
            current.state = 1; // running

            int wait = getClk() - current.arrivaltime; // wait at begining only
            current.waitingtime += wait;
            snprintf(str, sizeof(str), "At time %d process %d started arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, wait);
            writeStringToFile("scheduler.log", str);
            *shmaddrs = current.remainingtime;
            int pid = fork();
            if (pid == -1)
            {
                perror("error in forking ");
                exit(-1);
            }

            else if (pid == 0)
            {
                char flag[10], timeStr[20]; // assuming a reasonable size
                snprintf(timeStr, sizeof(timeStr), "%d", current.remainingtime);
                snprintf(flag, sizeof(flag), "%d", 1);
                char *args[] = {"process.out", timeStr, flag, NULL};
                printf("forked");
                printf(args);
                execv("process.out", args);
            }
            else
                current.acc_pid = pid;
        }
        else if (current.state == 3) // waitinig
        {
            current.waitingtime += getClk() - current.last;
            snprintf(str, sizeof(str), "At time %d process %d resumed arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, current.waitingtime);
            writeStringToFile("scheduler.log", str);
            current.state = 1;
            kill(current.acc_pid, SIGCONT);
        }
    }
    else if (current.state == 1 && current.remainingtime - quantum == *shmaddrs && !isEmpty(&Ready))
    {
        kill(current.acc_pid, SIGSTOP);
        current.remainingtime = *shmaddrs;
        current.last = getClk();
        current.state = 3;
        enqueuelast(&Ready, current);
        snprintf(str, sizeof(str), "At time %d process %d Stopped arr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, current.waitingtime);
        writeStringToFile("scheduler.log", str);
        current = top(&Ready);
        // printQueue(&Ready);
    }
    return;
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
    int FT = getClk();
    current.remainingtime = *shmaddrs;
    printQueue(&Ready);
    TA = FT - current.arrivaltime;
    WTA = TA / (float)current.runningtime;
    WTAArr[Ori_p_num - p_num] = WTA;
    snprintf(str, sizeof(str), "At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n", FT, current.pid, current.arrivaltime, current.runningtime, current.remainingtime, current.waitingtime, TA, WTA);
    writeStringToFile("scheduler.log", str);
    Avg_WTA += (WTA / Ori_p_num);
    Avg_waiting += (current.waitingtime / (float)Ori_p_num);
    Total_running += current.runningtime;
    p_num--;
    current.state = 2;

    printf("handler finished\n");

    // printQueue(&Ready);
}
void Finish()
{
    float sum = 0.0, mean, SD = 0.0;
    int i;
    for (i = 0; i < Ori_p_num; ++i)
    {
        sum += WTAArr[i];
    }
    mean = sum / Ori_p_num;
    for (i = 0; i < Ori_p_num; ++i)
    {
        SD += pow((WTAArr[i] - mean), 2);
    }
    STD_WTA = sqrt(SD / (float)Ori_p_num);
    float CPU_UTI = (1 - ((Final_time - Total_running) / (float)Final_time)) * 100;
    snprintf(str, sizeof(str), "CPU utilization = %.2f\nAvg WTA = %.2f\nAvg Waiting = %.2f\nStd WTA = %.2f\n", CPU_UTI, Avg_WTA, Avg_waiting, current.runningtime, STD_WTA);
    writeStringToFile("scheduler.perf", str);
}
void clearResources()
{
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)0);
    msgctl(shmidd, IPC_RMID, (struct msqid_ds *)0);
}