#include "headers.h"
#define NUMOFPROC 100
#include "PCB.h"
#include "PriorityQueue.h"
void clearResources(int);
int readFile(char*l);
int algo;
int quantuam;
struct PCB currentprocess;
struct priorityQueue ready;
int msqid;
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
 struct msgbuff
{
    long msgtype;
    struct PCB send;
};
void handler(int signum)
{
    printf("client ha terminated\n");
    exit(0);
} 
int temp ;  
int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    
    // 1. Read the input files.
    int p_num = readFile(argv[1]);
    algo =atoi(argv[2]);
    if(argc==4)
    {
        quantuam=atoi(argv[3]);
    }
    char algo_str[10], quantum_str[10], p_num_str[10];
    snprintf(algo_str, sizeof(algo_str), "%d", algo);
    snprintf(quantum_str, sizeof(quantum_str), "%d", quantuam);
    snprintf(p_num_str, sizeof(p_num_str), "%d", p_num);

    // 3. Initiate and create the scheduler and clock processes.
    int pid=fork();
    if(pid==-1)
    {
        printf("error forking schudler");
        exit(-1);
    }
    else if(pid ==0)
    {
            printf("\nI am the schudler, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());
           execl("./scheduler.out", "scheduler.out", algo_str, quantum_str, p_num_str, (char *) NULL);
            //exit(0);
    }else{
        temp =pid;
            pid=fork(); 
            // 4. Use this function after creating the clock process to initialize clock
            if(pid==-1)
            {
                printf("error forking clk");
                exit(-1);
            }
            else if(pid ==0)
            {
                printf("\nI am the clk, my pid = %d and my parent's pid = %d\n\n", getpid(), getppid());
                execl("clk.out","",NULL);
                //exit(0);
            }else{
                    // To get time use this
                    //sleep(2);
                    initClk();
                    int x = getClk();
                    printf("current time is %d\n", x);
                    int send_val;
                    key_t key=ftok("keyfile",'s');
                    if (key == -1)
                    {
                        perror("Error in ftok");
                        exit(-1);
                    }
                    msqid=msgget(key,0666|IPC_CREAT);
                    if (key == -1)
                    {
                        perror("Error in creating ids");
                        exit(-1);
                    }
                    struct msgbuff messagebefore;
                    struct PCB sent; 
                    messagebefore.msgtype = 1; 
                    int y;
                    sent=top(&ready);
                   
                    while((isEmpty(&ready)!= 1))
                    {
                         
                  //  printf("\nin first while x = %d\n",getClk());
                    y=getClk();
                    while((sent.arrivaltime==x)&&(isEmpty(&ready)!= 1))
                        {
                            sent = dequeu(&ready);
                            //printf("\nsent %d\n",msqid);
                            //writeToFile("output.txt",sent.pid,1);
                            messagebefore.send=sent;
                            send_val = msgsnd(msqid, &messagebefore, sizeof(messagebefore.send), !IPC_NOWAIT);
                            if (send_val == -1)
                            perror("Error in send");
                            
                            sent=top(&ready);
                        }
                            if (y!=x)           
                                x = y;
                    }

                    // TODO Generation Main Loop
                    // 5. Create a data structure for processes and provide it with its parameters.
                    // 6. Send the information to the scheduler at the appropriate time.
                    // 7. Clear clock resources
                    printf("%d  %d",temp,getpid());
                   int stat;
waitpid(temp, &stat, 0); // Waiting for the child process to finish

destroyClk(true); // Destroying clock resource after the child process finishes

                    return 0;
            }
        }
}

int readFile(char * filename)
{
    int i=-1;
    FILE *file;
    file = fopen(filename, "r");
    int id;
    int arrival;
    int runtime;
    int priority;
    char data[100];
    if(file) 
    {
        i++;
        while(fgets(data,100,file))
        { 
            if(data[0]=='#')
                continue;
            sscanf(data,"%d %d %d %d",&id,&arrival,&runtime,&priority);
            currentprocess.arrivaltime=arrival;
            currentprocess.runningtime=runtime;
            currentprocess.priority=priority;
            currentprocess.pid=id;
            setPCB(& currentprocess,id,arrival,runtime,priority);
            enqueue(&ready,currentprocess,currentprocess.arrivaltime);
            printf("%d    %d     %d     %d\n",id,arrival,runtime,priority);
            i++;
        }
setPCB(&currentprocess,-1,arrival+1,0,0);
enqueue(&ready,currentprocess,currentprocess.arrivaltime);
 

    } 
    else 
    {
        printf("Not able to open the file.");
    }
    fclose(file); 
    return i;
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    //printf("Choose a scheduling algorithm\n1)RR\n2)HPF\n3)SRTN\n");
    /*int algo,quantum;
    scanf("%d",&algo);
    if(algo==1)
    {
        printf("Enter Quantum for RR\n");
        scanf("%d",&quantum);
    }*/
}
void clearResources(int signum)
{
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)0);
    //TODO Clears all resources in case of interruption
}
