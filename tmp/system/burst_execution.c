/* burst.c - burst_execution */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  burst_execution  -  Simulate execution with alternating bursts of CPU use and CPU inactivity
 *------------------------------------------------------------------------
 */
 void   burst_execution(uint32 number_bursts, uint32 burst_duration, uint32 sleep_duration) {
    uint32 i;
    for(i=0; i<number_bursts; i++) 
    {
        while(proctab[currpid].runtime < ((i+1) * burst_duration));
        sleepms(sleep_duration);
    }
 }