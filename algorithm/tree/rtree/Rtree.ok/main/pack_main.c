/* pack_main.c
||
|| Description:
||	A test main program to "batch load" and pack an r-tree
||
|| Parameters:
||	1 - no. of dimensions
||	2 - raw data file to load from
||	3 - r-tree "base name" (.idx and .dat suffixes will be added)
*/

#include	<stdio.h>
#include	<ctype.h>

#ifdef	VMS
#include	<file.h>
#else
#include	<sys/file.h>
#endif

#include	"rtree.h"
#include	"assert.h"
#include	"timer.h"


main(argc, argv)
int argc;
char *argv[];
{
	RTREE	*rtreep, *OpenIndex();
	TIMER	rt_clock;
	int	opn_flag;

	if (argc != 4)
	{
		fprintf(stderr, "Usage: rload <no. dimensions> <raw data file> <r-tree file>\n");
		exit(1);
	}

	/* Create/Open an index file */
	opn_flag = O_RDWR | O_CREAT;
	if ((rtreep = OpenIndex(argv[3], opn_flag, atoi(argv[1]), RECT_TYPE)) == NULL)
		exit (1);

	/*Do Pack Specific Initialization */
	Initialize(rtreep, LOAD, NULL);
	ResetClock(&rt_clock);
	StartClock(&rt_clock);

	PackInput(rtreep, argv[2]);

	StopClock(&rt_clock);
	InsertStats(rtreep, &rt_clock);

	CloseIndex(rtreep);
	exit(0);
}
