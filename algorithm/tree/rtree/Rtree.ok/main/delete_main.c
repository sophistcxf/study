/*
|| delete_main.c
||
|| Description:
||	Delete from an r-tree rects from a file.
||
|| Arguments:
||	1 - no. of dimensions
||	2 - index file(s) name
||	3 - "rects" file name
||	4 - output filename
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
int	argc;
char	*argv[];
{

	RTREE	*rtreep, *OpenIndex();
	FILE	*fp, *fopen();
	TIMER	rt_clock;
	int	opn_flag;

	if (argc != 5)
	{
		fprintf(stderr, "Usage: rdelete <no. dimensions> <index filename> <window filename> <output filename>\n");
		exit(0);
	}

	/* Open an existing rtree index for deleting */
	opn_flag = O_RDWR;
	if ((rtreep = OpenIndex(argv[2], opn_flag, atoi(argv[1]), RECT_TYPE)) == NULL)
		exit(1);

	/* Open output file */
	if (! (fp = fopen(argv[4], "w+")) )
	{
		fprintf(stderr, "Unable to open output file <%s>\n", argv[3]);
		CloseIndex(rtreep);
		exit(1);
	}

	Initialize(rtreep, DELETE, fp);
	ResetClock(&rt_clock);
	StartClock(&rt_clock);

	if (BatchDelete(rtreep, argv[3]))
	{
		fprintf(stderr, "Delete finished!!!\n\n\n");
		StopClock(&rt_clock);
		DeleteStats(rtreep, &rt_clock);
	}

	CloseIndex(rtreep);
	fclose(fp);
	exit(0);
}
