#ifndef pcbn
#define pcbn

struct PCB
{
    int pid;
    int arrivaltime;
    int runningtime;
    int priority;
    int turnaroundtime;
    
    int waitingtime;
    int state;
    int remainingtime;
    int acc_pid;
};

void setPCB(struct PCB * x,int id,int at,int rt,int pr)
{
x->pid=id;
x->arrivaltime=at;
x->runningtime=rt;
x->remainingtime=rt;
x->priority=pr;
x->acc_pid=-1;
x->state=0;
/*x->turnaroundtime=tta;
x->waitingtime=wt;
x->remainingtime=rmt;*/
}
#endif