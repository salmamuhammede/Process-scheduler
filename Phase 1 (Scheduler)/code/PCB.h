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
};

void setPCB(struct PCB * x,int id,int at,int rt,int pr,int tta,int wt,int st,int rmt )
{
x->pid=id;
x->arrivaltime=at;
x->runningtime=rt;
x->priority=pr;
x->turnaroundtime=tta;
x->waitingtime=wt;
x->state=st;
x->remainingtime=rmt;
}