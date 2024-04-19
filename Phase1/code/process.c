#include "headers.h"

/* Modify this file as needed*/
//Comments Salma :topics req 6 
int remainingtime;

int main(int agrc, char * argv[])
{
    initClk();
     key_t keymem = ftok("keyfile", 's');
    if (keymem == -1)
    {
        perror("Error in ftok memory");
        exit(-1);
    }

    int shmid = shmget(keymem, 4, IPC_CREAT | 0644);

    if (shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    } 
    void *shmaddr = shmat(shmid, (void *)0, 0);
    
    //TODO it needs to get the remaining time from somewhere
    int remainingtime = atoi(argv[1]);
    int y,x = getClk();
    while (1)
    {
        sleep(0.1); // to reduce lag
        y = getClk();
        if(y!=x){
            printf("clock: %d\n" ,getClk());
            x=y;
            remainingtime--;
        }
        if (remainingtime == 0)
        {
            printf("process %d finished",getpid());
            kill(getppid(), SIGUSR1);
            break;
        }
        
    }
    
    destroyClk(false);
    
    return 0;
}
