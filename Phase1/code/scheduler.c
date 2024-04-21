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
char str[100];
void writeToFile(const char *filename, int num1, int num2);
void handler(int signum);
void SelectedAlgo(int x, struct PCB com);
void writeStringToFile(const char *filename, const char *str);
struct PCB current;
struct PCB dummy;
int p_num, Ori_p_num;
int shmidd;
int *shmaddrs;
priorityQueue Ready;
void HPF();
void RR();
int quantum;
int Final_time, TA, Total_running = 0;
float WTA, Avg_WTA = 0, Avg_waiting = 0, STD_WTA;
float *WTAArr;
int main(int argc, char *argv[])
{
    current.state = 0;
    int cur_algo = atoi(argv[1]);
    signal(SIGUSR1, handler);
    quantum = atoi(argv[2]);
    key_t keymem = ftok("RT", 'r');
    shmidd = shmget(keymem, 4, IPC_CREAT | 0644);
    if (shmidd == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    shmaddrs = shmat(shmidd, (void *)0, 0);
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
    Ori_p_num = p_num;
    WTAArr = (float*)malloc(p_num * sizeof(int));
    printf("%d\n", p_num);
    printf("%d\n", atoi(argv[1]));
    system("rm scheduler.log");
    system("rm scheduler.perf");
    // p_num++;
    // writeStringToFile("scheduler.log", "\U00002796\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U0001F7F0\U00002796\n");
    int clock = getClk();
    while (p_num)
    {
        sleep(.1); // to reduce lag
        while ((msgrcv(msqid, &messagebefore, sizeof(messagebefore.send), 0, IPC_NOWAIT) != -1) && messagebefore.send.pid != -1)
        {
            // p_num--;

            SelectedAlgo(cur_algo, messagebefore.send);
            writeToFile("output.txt", messagebefore.send.pid, getClk());
            // printQueue(&Ready);
            //  printf("hallo");
            //  printf("\nMessage received: %d\n", messagebefore.send.pid);
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
    Final_time = getClk();
    Finish();
    printf("complete\n");
    // writeStringToFile("scheduler.log", "\U00002705-=FINISHED=-\U00002705\n");
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

        printf("hallo\n");
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
            current.waitingtime=wait;
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
            snprintf(str, sizeof(str), "At time %d process %d resumed rr %d total %d remain %d wait %d \n", getClk(), current.pid, current.arrivaltime, current.runningtime, current.remainingtime, current.waitingtime);
            writeStringToFile("scheduler.log", str);
            current.state = 1;
            kill(current.acc_pid, SIGCONT);
        }
    }
    else if (current.state == 1 && current.remainingtime - quantum == *shmaddrs)
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
    WTAArr[Ori_p_num-p_num] = WTA;
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
    STD_WTA = sqrt(SD / (float) Ori_p_num);
    float CPU_UTI = (1 - ((Final_time - Total_running) / (float)Final_time)) * 100;
    snprintf(str, sizeof(str), "CPU utilization = %.2f\nAvg WTA = %.2f\nAvg Waiting = %.2f\nStd WTA = %.2f\n", CPU_UTI, Avg_WTA, Avg_waiting, current.runningtime, STD_WTA);
    writeStringToFile("scheduler.perf", str);
}