/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	
	ctr1000++;
	priority_counter--;
	proctab[currpid].runtime++;

	if ((proctab[currpid].user_process == TRUE) &&
		(proctab[currpid].prprio != LOWEST_USER_PRIORITY))
	{
		if (proctab[currpid].time_allotment == 0)
		{
			proctab[currpid].time_allotment = 0;
		}
		else 
		{
			proctab[currpid].time_allotment--; 
		}
	}

	/* Priority boost of all user processes */
	pid32 	i;
	if (priority_counter == 0)
	{
		for (i = 0; i < NPROC; i++)
		{
			if ((proctab[i].user_process == TRUE) &&
				(proctab[i].prstate != PR_FREE))
			{
				proctab[i].prprio = UPRIORITY_QUEUES;
				proctab[i].time_allotment = TIME_ALLOTMENT;
				proctab[i].upgrades++;
    			
				if (proctab[i].prstate == PR_READY)
				{
					getitem(i);
					insert(i, readylist, proctab[i].prprio);
				}
			}
		}
		priority_counter = PRIORITY_BOOST_PERIOD;
		resched();
	}
	
	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		preempt = QUANTUM;
		resched();
	}
}
