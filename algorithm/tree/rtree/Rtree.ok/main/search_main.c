/*
|| search_main.c
||
|| Description:
||	Search an r-tree w/ a given window.
||
|| Arguments:
||	1 - no. of dimensions
||	2 - index file(s) name
||	3 - "window" file name
||	4 - output filename
||	5 - topological/direction relation
||	6 - mode of search (OBJ or MBR)
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

        /* Topological Functions */
int             Overlap();
int             Disjoint();             int     Disjoint1();
                                        int     Disjoint2();
int             SOverlap();             int     SOverlap1();
                                        int     SOverlap2();
int             Cover();                int     Cover1();
                                        int     Cover2();
int             Contain();              int     Contain1();
                                        int     Contain2();
int             Equal();                int     Equal1();
                                        int     Equal2();
int             Inside();               int     Inside1();
                                        int     Inside2();
int             Covered_by();           int     Covered_by1();
                                        int     Covered_by2();
int             Meet();                 int     Meet1();
                                        int     Meet2();

        /* Direction Functions */
int             Strong_North();         int     Strong_North1();
int             Weak_North();           int     Weak_North1();
int             Strong_Bounded_North(); int     Strong_Bounded_North1();
int             Weak_Bounded_North();   int     Weak_Bounded_North1();
int             Strong_NorthEast();     int     Strong_NorthEast1();
int             Weak_NorthEast();       int     Weak_NorthEast1();
int             Same_Level();           int     Same_Level1();
int             Strong_Same_Level();    int     Strong_Same_Level1();
int             Just_North();           int     Just_North1();
int             North_South();          int     North_South1();
int             North();          	int     North1();

int     NO_REFIN;

main(argc, argv)
int	argc;
char	*argv[];
{

	RTREE	*rtreep, *OpenIndex();
	FILE	*fp, *fopen();
	TIMER	rt_clock;
	int	opn_flag, mbr;

	if (argc != 7)
	{
		fprintf(stderr, "Usage: rsearch <dim.> <index> <window> <output> <top/dir rel.> <OBJ/MBR>\n");
		exit(0);
	}

	/* Open an existing rtree index for searching */
	opn_flag = O_RDONLY;
	if ((rtreep = OpenIndex(argv[2], opn_flag, atoi(argv[1]), RECT_TYPE)) == NULL)
		exit(1);

	/* Open output file */
	if (! (fp = fopen(argv[4], "w+")) )
	{
		fprintf(stderr, "Unable to open output file <%s>\n", argv[3]);
		CloseIndex(rtreep);
		exit(1);
	}

	Initialize(rtreep, SEARCH, fp);

        if (strcmp(argv[6],"OBJ")==0)
                mbr = 0;
        else if (strcmp(argv[6],"MBR")==0)
                mbr = 1;
        else
        {
                fprintf(stderr, "ERROR:\taccepted modes are: OBJ MBR\n");
                CloseIndex(rtreep);
                exit(1);
        }

        /* Assign Rectangle Fct for intermediate/terminal nodes */
        if (strcmp(argv[5],"OV")==0)
        {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Overlap;
        }
        else if (strcmp(argv[5],"DJ")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Disjoint1;
                rtreep->rtrectcmpT = Disjoint2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Disjoint;
                }
        }
        else if (strcmp(argv[5],"SO")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = SOverlap1;
                rtreep->rtrectcmpT = SOverlap2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = SOverlap;
                }
        }
        else if (strcmp(argv[5],"CV")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Cover1;
                rtreep->rtrectcmpT = Cover2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Cover;
                }
        }
        else if (strcmp(argv[5],"CN")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Contain1;
                rtreep->rtrectcmpT = Contain2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Contain;
                }
        }
        else if (strcmp(argv[5],"EQ")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Equal1;
                rtreep->rtrectcmpT = Equal2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Equal;
                }
        }
        else if (strcmp(argv[5],"IN")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Inside1;
                rtreep->rtrectcmpT = Inside2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Inside;
                }
        }
        else if (strcmp(argv[5],"CB")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Covered_by1;
                rtreep->rtrectcmpT = Covered_by2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Covered_by;
                }
        }
        else if (strcmp(argv[5],"MT")==0)
        {
                if (mbr==0)
                {
                rtreep->rtrectcmp = Meet1;
                rtreep->rtrectcmpT = Meet2;
                }
                else
                {
                rtreep->rtrectcmp = Overlap;
                rtreep->rtrectcmpT = Meet;
                }
        }
        else if (strcmp(argv[5],"SN")==0)
        {
                rtreep->rtrectcmp = Strong_North1;
                rtreep->rtrectcmpT = Strong_North;
        }
        else if (strcmp(argv[5],"WN")==0)
        {
                rtreep->rtrectcmp = Weak_North1;
                rtreep->rtrectcmpT = Weak_North;
        }
        else if (strcmp(argv[5],"SBN")==0)
        {
                rtreep->rtrectcmp = Strong_Bounded_North1;
                rtreep->rtrectcmpT = Strong_Bounded_North;
        }
        else if (strcmp(argv[5],"WBN")==0)
        {
                rtreep->rtrectcmp = Weak_Bounded_North1;
                rtreep->rtrectcmpT = Weak_Bounded_North;
        }
        else if (strcmp(argv[5],"SNE")==0)
        {
                rtreep->rtrectcmp = Strong_NorthEast1;
                rtreep->rtrectcmpT = Strong_NorthEast;
        }
        else if (strcmp(argv[5],"WNE")==0)
        {
                rtreep->rtrectcmp = Weak_NorthEast1;
                rtreep->rtrectcmpT = Weak_NorthEast;
        }
        else if (strcmp(argv[5],"SL")==0)
        {
                rtreep->rtrectcmp = Same_Level1;
                rtreep->rtrectcmpT = Same_Level;
        }
        else if (strcmp(argv[5],"SSL")==0)
        {
                rtreep->rtrectcmp = Strong_Same_Level1;
                rtreep->rtrectcmpT = Strong_Same_Level;
        }
        else if (strcmp(argv[5],"JN")==0)
        {
                rtreep->rtrectcmp = Just_North1;
                rtreep->rtrectcmpT = Just_North;
        }
        else if (strcmp(argv[5],"NS")==0)
        {
                rtreep->rtrectcmp = North_South1;
                rtreep->rtrectcmpT = North_South;
        }
        else if (strcmp(argv[5],"N")==0)
        {
                rtreep->rtrectcmp = North1;
                rtreep->rtrectcmpT = North;
        }
        else
        {
                fprintf(stderr, "ERROR:\taccepted topological relations are: OV DJ SO CV CN EQ IN CB MT\n");
                fprintf(stderr, "\taccepted direction relations are: SN WN SBN WBN SNE WNE SL SSL JN NS N\n");
                CloseIndex(rtreep);
                exit(1);
        }

        NO_REFIN = 0;

	ResetClock(&rt_clock);
	StartClock(&rt_clock);

	if (BatchSearch(rtreep, argv[3]))
	{
		StopClock(&rt_clock);
		SearchStats(rtreep, &rt_clock);
	}

	CloseIndex(rtreep);
	fclose(fp);
	exit(0);
}
