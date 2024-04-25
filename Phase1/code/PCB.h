#ifndef pcbn
#define pcbn

struct PCB
{
    int pid;
    int arrivaltime;
    int runningtime;
    int priority;
    int turnaroundtime;
    int begin;
    int waitingtime;
    int state;
    int remainingtime;
    int acc_pid;
    int last;
    int flag;
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
x->last=0;
x->begin=rt;
x->flag=0;
/*x->turnaroundtime=tta;
x->waitingtime=wt;
x->remainingtime=rmt;*/
}
#endif