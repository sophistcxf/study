/*
|| print_main.c
*/

#include <stdio.h>
#include <ctype.h>

#ifdef	VMS
#include	<file.h>
#else
#include	<sys/file.h>
#endif

#include	"rtree.h"
#include	"assert.h"


main(argc, argv)
int	argc;
char	*argv[];
{
	RTREE	*rtreep, *OpenIndex();
	int	opn_flag;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: rprint <no. dimensions> <index name>\n");
		exit(0);
	}

	/* Open an existing index file */
	opn_flag = O_RDONLY;
	if ((rtreep = OpenIndex(argv[2], opn_flag, atoi(argv[1]), RECT_TYPE)) == NULL)
		exit (1);

	PrintData(rtreep, rtreep->root);

	CloseIndex(rtreep);
	exit(0);
}
