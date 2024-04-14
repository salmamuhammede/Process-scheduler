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
    int remainingtime = argv[1];
    while (remainingtime > 0)
    {
        
        strcpy((char *)shmaddr, itoa(remainingtime--));
        //strcpy((char *)shmaddr, itoa(remainingtime--));

    }
    
    destroyClk(false);
    
    return 0;
}
