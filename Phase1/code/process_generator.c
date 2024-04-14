#include "headers.h"
#define NUMOFPROC 3
#include "PCB.h"
#include "PriorityQueue.h"
void clearResources(int);
void readFile(char*l);
int algo;
int quantuam;
struct PCB currentprocess;
struct PriorityQueue ready;
    
int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    
    // 1. Read the input files.
    readFile(argv[1]);
    algo =atoi(argv[1]);
    if(argc==4)
    {
        quantuam=argv[3];
    }

    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
    return 0;
}

void readFile(char * filename)
{
    FILE *file;
    file = fopen(filename, "r");
    int id[NUMOFPROC];
    int arrival[NUMOFPROC];
    int runtime[NUMOFPROC];
    int priority[NUMOFPROC];
    char data[100];
    if(file) 
    {
        int i=0;
        while(fgets(data,100,file))
        { 
            if(data[0]=='#')
                continue;
            sscanf(data,"%d %d %d %d",&id[i],&arrival[i],&runtime[i],&priority[i]);
            currentprocess.arrivaltime=arrival[i];
            currentprocess.runningtime=runtime[i];
            currentprocess.priority=priority[i];
            currentprocess.pid=id[i];
            setPCB(& currentprocess,id[i],arrival[i],runtime[i],priority[i]);
            enqueue(&ready,currentprocess,currentprocess.arrivaltime);
            printf("%d    %d     %d     %d\n",id[i],arrival[i],runtime[i],priority[i]);
            i++;
        }

    } 
    else 
    {
        printf("Not able to open the file.");
    }
    fclose(file); 
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
    //TODO Clears all resources in case of interruption
}
