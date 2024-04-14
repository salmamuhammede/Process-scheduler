#include "headers.h"
#define NUMOFPROC 3
void clearResources(int);

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE *file;
    file = fopen("processes.txt", "r");
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
    printf("Choose a scheduling algorithm\n1)RR\n2)HPF\n3)SRTN\n");
    int algo,quantum;
    scanf("%d",&algo);
    if(algo==1)
    {
        printf("Enter Quantum for RR\n");
        scanf("%d",&quantum);
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
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
