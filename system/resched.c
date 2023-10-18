/* resched.c - resched, resched_cntl */

#include <xinu.h>

#define DEBUG_CTXSW 
#define LOWEST_USER_PRIORITY 1
#define MINIMUM_SYSTEM_PRIO 20

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	pid32  old_pid;
	qid16  curr;
	pri16  curr_prio = 0;
	uint32 i;

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */
	ptold = &proctab[currpid];
	old_pid = currpid;

	/* Places ptold back into readylist */
	if (ptold->prstate == PR_CURR) 
	{ 
		ptold->prstate = PR_READY;
		if ((ptold->user_process == TRUE) &&
		 	(ptold->prprio != LOWEST_USER_PRIORITY))
		{
			proctab[currpid].time_allotment--; 
		}
		insert(currpid, readylist, ptold->prprio);
	}

	/* Priority boost of all user processes */
	if (ctr1000 % PRIORITY_BOOST_PERIOD == 0)
	{
		for (i = 0; i < NPROC; i++)
		{
			if ((proctab[i].user_process == TRUE) &&
			    (proctab[i].prprio < UPRIORITY_QUEUES))
			{
				proctab[i].prprio = UPRIORITY_QUEUES;
				proctab[i].time_allotment = TIME_ALLOTMENT;
				proctab[i].upgrades++;

				if (proctab[i].prstate == PR_READY) 
				{
					/* Remove from readylist and then reinsert */
					queuetab[queuetab[i].qprev].qnext = queuetab[i].qnext;
					queuetab[queuetab[i].qnext].qprev = queuetab[i].qprev;
					queuetab[i].qnext = EMPTY;
					queuetab[i].qprev = EMPTY;
					
					insert(i, readylist, UPRIORITY_QUEUES);
				}
			}
		}
	}

	/* Search through readylist and decrement prio if time allotment is zero */
	curr = firstid(readylist);
	while (curr != queuetail(readylist)) 
	{
		if ((proctab[curr].user_process == TRUE) &&
			(proctab[curr].time_allotment == 0)  &&
			(proctab[curr].prprio != LOWEST_USER_PRIORITY))
		{
			/* Remove from readylist and then reinsert */
			queuetab[queuetab[curr].qprev].qnext = queuetab[curr].qnext;
			queuetab[queuetab[curr].qnext].qprev = queuetab[curr].qprev;
			queuetab[curr].qnext = EMPTY;
			queuetab[curr].qprev = EMPTY;

			curr_prio = proctab[curr].prprio--;
			proctab[curr].time_allotment = proctab[curr].init_time_allotment * (1 << (UPRIORITY_QUEUES - curr_prio));
			proctab[curr].downgrades++;

			insert(curr, readylist, curr_prio);
		}
		curr = queuetab[curr].qnext;
	}

 	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	if (currpid != old_pid) ptnew->num_ctxsw++;
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		

	#ifdef DEBUG_CTXSW
		//if (currpid != old_pid) kprintf("ctxsw::%d-%d\n", old_pid, currpid);
	#endif

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}

void reset_timing(){
 ctr1000 = 0;
 /* reset counter used to trigger priority upgrades */
}