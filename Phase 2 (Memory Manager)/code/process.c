#include "headers.h"

/* Modify this file as needed*/
// Comments Salma :topics req 6
int Pause = 0;
void handler1(int signum);
void handler2(int signum);
int *shmaddrss;
int shmidd;
int remainingtime;
int main(int agrc, char *argv[])
{
    printf("begin\n");
    initClk();
    signal(SIGSTOP, handler1);
    signal(SIGCONT, handler2);
    key_t keymem = ftok("RT", 'r');
    if (keymem == -1)
    {
        perror("Error in ftok memory");
        exit(-1);
    }
    shmidd = shmget(keymem, 4, IPC_CREAT | 0644);

    if (shmidd == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    shmaddrss = (int *)shmat(shmidd, (void *)0, 0);
    remainingtime = atoi(argv[1]);

    // TODO it needs to get the remaining time from somewhere
    *shmaddrss = remainingtime;
    while (1)
    {
        // remainingtime--;
        sleep(0.1);
        remainingtime = *shmaddrss;
        //printf("\nRT: %d \n",remainingtime);
        if (remainingtime == 0)
        {
            printf("killed %d %d\n", getpid(), getppid());
            kill(getppid(), SIGUSR1);
            break;
        }
    }

    destroyClk(false);

    return 0;
}
void handler1(int signum)
{
    // *shmaddrss = -404;
    remainingtime++;
    shmdt(shmaddrss);
}
void handler2(int signum)
{
    remainingtime++;
    shmaddrss = shmat(shmidd, (void *)0, 0);
}