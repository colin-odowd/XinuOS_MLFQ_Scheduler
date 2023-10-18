/* resched.h */

/* Constants and variables related to deferred rescheduling */

#define	DEFER_START	1	/* Start deferred rescehduling		*/
#define	DEFER_STOP	2	/* Stop  deferred rescehduling		*/

#define UPRIORITY_QUEUES 3
#define TIME_ALLOTMENT 100
#define PRIORITY_BOOST_PERIOD 10000000
#define LOWEST_USER_PRIORITY 1
#define MINIMUM_SYSTEM_PRIO 20

/* Structure that collects items related to deferred rescheduling	*/

struct	defer	{
	int32	ndefers;	/* Number of outstanding defers 	*/
	bool8	attempt;	/* Was resched called during the	*/
				/*   deferral period?			*/
};

extern	struct	defer	Defer;
