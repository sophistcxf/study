/* timer.h
||
||	This header file contains structure and constant definitions
|| for R-Tree timing functions.
*/

typedef
struct rtree_timer
{
	long	ElapsedTime;
	long	startElapsed, stopElapsed;
	float	UserTime, SystemTime;
}	TIMER;

