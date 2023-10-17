/* burst_execution.c - burst_execution */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  burst_execution  -  simulates alternate execution phases
 *------------------------------------------------------------------------
 */
void burst_execution(uint32 number_bursts, 
                     uint32 burst_duration, 
                     uint32 sleep_duration)
{
    int i;
    for(i=0; i<number_bursts;i++){
        while(proctab[currpid].runtime<(burst_duration*(i+1)));
        sleepms(sleep_duration);
    }
}

