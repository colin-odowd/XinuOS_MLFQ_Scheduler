#include <xinu.h>
#include <stdio.h>

void timed_execution(uint32 runtime){
	while(proctab[currpid].runtime<runtime);
}

int main() {
	pid32 prA, prB, prC, prD, prE;
	pid32 pr[10];
	uint32 i;

	kprintf("\n");

	kprintf("QUANTUM=%d, TIME_ALLOTMENT=%d, PRIORITY_BOOST_PERIOD=%d, UPRIORITY_QUEUES=%d\n\n", QUANTUM, TIME_ALLOTMENT, PRIORITY_BOOST_PERIOD, UPRIORITY_QUEUES);

	kprintf("========================================== TESTCASE 1::  CPU-intensive jobs ===============================\n");

	reset_timing();

	prA = create_user_process(timed_execution, 1024, "cpu_1000", 1, 1000);
	prB = create_user_process(timed_execution, 1024, "cpu_1000", 1, 1000);

	resume(prA);		
	resume(prB);

	receive();	
	receive();	

	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prA, proctab[prA].prname, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw, proctab[prA].downgrades, proctab[prA].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prB, proctab[prB].prname, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw, proctab[prB].downgrades, proctab[prB].upgrades);

	kprintf("===========================================================================================================\n\n");

        kprintf("========================================== TESTCASE 2::  CPU-intensive jobs ===============================\n");

	reset_timing();

        prA = create_user_process(timed_execution, 1024, "cpu_1000", 1, 1000);
        prB = create_user_process(timed_execution, 1024, "cpu_1000", 1, 1000);
        prC = create_user_process(timed_execution, 1024, "cpu_1000_new", 1, 1000);
        prD = create_user_process(timed_execution, 1024, "cpu_1000_del", 1, 1000);

        resume(prA);
        resume(prB);
        
	receive();
	receive();

	sleepms(10); //wait for first two processes to terminate

	resume(prC);

	sleepms(500);

        resume(prD);

        receive();
        receive();

        sleepms(50); // wait for user process to terminate  

	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prA, proctab[prA].prname, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw, proctab[prA].downgrades, proctab[prA].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prB, proctab[prB].prname, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw, proctab[prB].downgrades, proctab[prB].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prC, proctab[prC].prname, proctab[prC].runtime, proctab[prC].turnaroundtime, proctab[prC].num_ctxsw, proctab[prC].downgrades, proctab[prC].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prD, proctab[prD].prname, proctab[prD].runtime, proctab[prD].turnaroundtime, proctab[prD].num_ctxsw, proctab[prD].downgrades, proctab[prD].upgrades);

	kprintf("===========================================================================================================\n\n");

        kprintf("========================================== TESTCASE 3::  interactive job  =================================\n");

	reset_timing();

        prB = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);

        resume(prB);

        receive();

        sleepms(50); // wait for user processes to terminate    

        kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prB, proctab[prB].prname, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw, proctab[prB].downgrades, proctab[prB].upgrades);

	kprintf("===========================================================================================================\n\n");

        kprintf("========================================== TESTCASE 4::  interactive jobs =================================\n");

	reset_timing();

        prA = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);
        prB = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);
        prC = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);
        prD = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);
        prE = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);

        resume(prA);
        resume(prB);
        resume(prC);
        resume(prD);
        resume(prE);

        receive();
        receive();
        receive();
        receive();
        receive();

        sleepms(50); // wait for user processes to terminate    

	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prA, proctab[prA].prname, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw, proctab[prA].downgrades, proctab[prA].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prB, proctab[prB].prname, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw, proctab[prB].downgrades, proctab[prB].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prC, proctab[prC].prname, proctab[prC].runtime, proctab[prC].turnaroundtime, proctab[prC].num_ctxsw, proctab[prC].downgrades, proctab[prC].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prD, proctab[prD].prname, proctab[prD].runtime, proctab[prD].turnaroundtime, proctab[prD].num_ctxsw, proctab[prD].downgrades, proctab[prD].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prE, proctab[prE].prname, proctab[prE].runtime, proctab[prE].turnaroundtime, proctab[prE].num_ctxsw, proctab[prE].downgrades, proctab[prE].upgrades);
        
	kprintf("===========================================================================================================\n\n");

        kprintf("========================================== TESTCASE 5:: mixed jobs ========================================\n");

	reset_timing();

        prA = create_user_process(timed_execution, 1024, "cpu_1000", 1, 1000);
        prB = create_user_process(timed_execution, 1024, "cpu_1000", 1, 1000);
        prC = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);
        prD = create_user_process(burst_execution, 1024, "burst_100/4/6", 3, 100, 4, 6);

        resume(prA);
        resume(prB);
        resume(prC);
        resume(prD);

        receive();
        receive();
        receive();
        receive();

        sleepms(50); // wait for user processes to terminate    
	
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prA, proctab[prA].prname, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw, proctab[prA].downgrades, proctab[prA].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prB, proctab[prB].prname, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw, proctab[prB].downgrades, proctab[prB].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prC, proctab[prC].prname, proctab[prC].runtime, proctab[prC].turnaroundtime, proctab[prC].num_ctxsw, proctab[prC].downgrades, proctab[prC].upgrades);
	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prD, proctab[prD].prname, proctab[prD].runtime, proctab[prD].turnaroundtime, proctab[prD].num_ctxsw, proctab[prD].downgrades, proctab[prD].upgrades);

	kprintf("===========================================================================================================\n\n");

	kprintf("========================================== TESTCASE 6::  scattered start  =================================\n");
	
	reset_timing();

        prA = create_user_process(timed_execution, 1024, "cpu_2000", 1, 2000);
	
	for(i=0; i<10; i++)
		pr[i] = create_user_process(timed_execution, 1024, "cpu_300", 1, 250);
	
	resume(prA);

	sleepms(TIME_ALLOTMENT*4);

	for(i=0; i<10; i++){
		resume(pr[i]);
		sleepms(TIME_ALLOTMENT);
	}
	
	for(i=0; i<=10; i++) receive();
	
        sleepms(50); // wait for user processes to terminate    

	kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prA, proctab[prA].prname, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw, proctab[prA].downgrades, proctab[prA].upgrades);
	for (i=0;i<10;i++)
		kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",pr[i], proctab[pr[i]].prname, proctab[pr[i]].runtime, proctab[pr[i]].turnaroundtime, proctab[pr[i]].num_ctxsw, proctab[pr[i]].downgrades, proctab[pr[i]].upgrades);

	kprintf("===========================================================================================================\n\n");
	
        kprintf("========================================== TESTCASE 7::  mixed jobs =======================================\n");
        
	reset_timing();

	prA = create_user_process(timed_execution, 1024, "cpu_2000", 1, 2000);

        for(i=0; i<10; i++)
                pr[i] = create_user_process(burst_execution, 1024, "burst_100/2/8", 3, 100, 2, 8);

        resume(prA);

        sleepms(TIME_ALLOTMENT*4);

        for(i=0; i<10; i++) resume(pr[i]);

        for(i=0; i<=10; i++) receive();

        sleepms(50); // wait for user processes to terminate    

        kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",prA, proctab[prA].prname, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw, proctab[prA].downgrades, proctab[prA].upgrades);
        for (i=0;i<10;i++)
                kprintf("process %d:: name=%s, runtime=%d, turnaround time=%d, ctx=%d, downgrades=%d, upgrades=%d\n",pr[i], proctab[pr[i]].prname, proctab[pr[i]].runtime, proctab[pr[i]].turnaroundtime, proctab[pr[i]].num_ctxsw, proctab[pr[i]].downgrades, proctab[pr[i]].upgrades);

	kprintf("===========================================================================================================\n\n");
	
	return OK;

}
