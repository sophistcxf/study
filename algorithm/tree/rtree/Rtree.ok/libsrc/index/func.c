/* func.c
||
|| Description:
||	This modoule contains functions to create and drop r-tree indices
||
|| Functions:
||	RTREE	*OpenIndex(indexName, mode, nodims, tntype)
||	RT_HEAD	*newRTHead(ndims, tntype)
||		CloseIndex(rtreep)
||		DropIndex(indexName)
||		GetOneRect(fp, rtreep, rect)
||		GetManyRect(fp, rtreep, rect, id)
||		PackInput(rtreep, fileName)
||		BatchInsert(rtreep, fileName)
||		RectInsert(rtreep, root, rect, id)
||		BatchDelete(rtreep, fileName)
||		RectDelete(rtreep, rect, id)
||		BatchSearch(rtreep, fileName)
||		RectSearch(rtreep, rect)
*/

#include <stdio.h>
#include <ctype.h>

#ifdef	VMS
#include	<types.h>
#include	<file.h>
#else
#include	<sys/types.h>
#include	<sys/file.h>
#endif

	/* R-Tree Includes */
#include	"rtree.h"
#include	"assert.h"
#include	"error.h"

	/* ADMS Includes */
#include	"asec.h"

	/* R-Tree Functions Returning Ptrs */
PAGE_BUF	**newPagePool();
RT_HEAD		*newRTHead();
PARTVARS	*newPVars();
struct	Node	*newNode();
struct	Rect	*newRect();

int		Overlap();

	/* Define local variable/constant for demo copy */
#if	(RECT_LIMIT)
static	int	rect_count = 0;
static	char	demo_msg[] =  "Maximum rectangle count (%d) exceeded in demonstration copy.\n";
#endif



/* RTREE * OpenIndex(indexName, mode, ndims, tntype)
||
||	Create an index file with the given index name in the first argument. If 
|| if doesn't exis, create it. Then initialize the RTree Struct.
*/
RTREE	*
OpenIndex(indexName, mode, ndims, tntype)
char	*indexName;	/* Base name of R-Tree files */
int	mode;		/* R-Tree Open Mode */
int	ndims;		/* No. of dimension in R-Tree */
int	tntype;		/* Terminal Node Type */
{
	register int	i, idxp;
	RTREE		*rtreep;
	RT_HEAD		*rtheadp;
	RT_STATS	*rtstatsp;
	char	ixfname[MAXNAMLEN], slfname[MAXNAMLEN];
	int	ndcard;

	/* Form the index file name */
	strcpy(ixfname, indexName);
	strcat(ixfname, IXSUFFIX);

	/* Open the index file(s) according to the mode */
	if (mode & O_CREAT)
		idxp = open(ixfname, mode, 0666);
	else
		idxp = open(ixfname, mode);
		
	if (idxp < 0)
	{
		ERROR1("Unable to open/create the index file '%s'", ixfname);
		return (NULL);
	}

	/* Allocate sub structures:
	 * Allocation failure is considered non-recoverable and generates an
	 * abort in the allocation primitive 'myalloc'.
	 */

	/* Allocate an R-Tree Structure */
	rtreep = (RTREE *) myalloc(sizeof(RTREE));

	/* Allocate an R-Tree Header Structure */
	rtreep->rthead = rtheadp = newRTHead(ndims, tntype);

	/* Allocate an R-Tree Buffer Pool
	 * Use a default - can be changed dynamically if necessary
	 */
	rtreep->Page_Pool = newPagePool(NUMBUFFS);
	rtreep->NumPageBufs = NUMBUFFS;

	/* Allocate an R-Tree Statistics Structure */
	rtstatsp = rtreep->rtstats = (RT_STATS *) myalloc(sizeof(RT_STATS));

	/* Allocate & init covering rect struct */
	rtreep->CoverAll = newRect(ndims, 1);

	/* Save the index file desc., open mode & "base" index name into r-tree struct */
	rtreep->rtifd = idxp;
	rtreep->opnmode = mode;
	strcpy(rtreep->rtfnam, indexName);

	/* If we're creative, write out the header page */
	if (mode & O_CREAT)
	{
		/* Allocate the root node */
		rtreep->root = newNode(rtreep, rtheadp->TNodeCard);

		rtreep->root->level = 0;
		PutOnePage(rtreep, 0, rtreep->root);
		rtreep->pageNo = 1;

		/* Init Node Counts */
		rtstatsp->LeafCount = 1;
		rtstatsp->NodeCount = 1;

	}
	/* Otherwise, read in the header page */
	else 
	{
		rtreep->pageNo = 0;
		rtreep->MinFill = (rtheadp->NodeCard + 1) * 0.4;
		rtreep->root = (struct Node *) GetOnePage(rtreep, 0);
	}

	/* Allocate the Split Partition Struct */
	rtreep->rtpartvars = newPVars(rtreep);


	/* ======================================================================= */
	/*           ADMS SUB-LIST Initialization Follows                          */
	/*                                                                         */
	/* ======================================================================= */
	/* Form the sub-list file name */
	strcpy(slfname, indexName);
	strcat(slfname, SLSUFFIX);

	/* Failure to open sub-leaf file is recoverable */
	if ((rtreep->rtsfd = opena(slfname, mode)) == 0)
	{
		ERROR1("Unable to open/create sub-list file '%s' .", slfname);
		freeNode(rtreep->root);
		myfree(rtreep->CoverAll);
		myfree(rtreep->rtstats);
		myfree(rtreep->Page_Pool);
		myfree(rtreep->rthead);
		myfree(rtreep);
		close(idxp);
		rtreep = NULL;
	}

	/* Return the ptr to the r-tree structure */
	return (rtreep);
}

/* newRTHead()
||	allocate & initialize an R-Tree Header
*/
RT_HEAD	*
newRTHead(ndims, tntype)
int	ndims;		/* No. of dimensions */
int	tntype;		/* Terminal Node Type */
{
	RT_HEAD	*rtheadp;

	rtheadp = (RT_HEAD *) myalloc(sizeof(RT_HEAD));

	/* Init Dimension Parameters for R-Tree */
	rtheadp->NumDims = ndims;
	rtheadp->NumSides = 2 * ndims;

	/* Init Node Cardinality for R-Tree
	 * Note: the divisor is the size of a branch - 1 Rect + 1 ptr
	 */
	rtheadp->NodeCard = (PAGESIZE - LEVEL_SIZE - COUNT_SIZE - HEAD_SIZE) /
		((ndims * 2) * COORD_SIZE + POINTER_SIZE);

	/* For points *increase (NOT double** the terminal node cardinality */
	rtheadp->TNodeType = tntype;
	if (tntype == POINT_TYPE)
		rtheadp->TNodeCard = (PAGESIZE - LEVEL_SIZE - COUNT_SIZE - HEAD_SIZE) /
			(2 * COORD_SIZE + POINTER_SIZE);
	else
		rtheadp->TNodeCard = rtheadp->NodeCard;

	return(rtheadp);
}


/* newPagePool(nbufs)
||	allocate & initialize buffer pool for current r-tree
*/
PAGE_BUF **
newPagePool(nbufs)
int	nbufs;
{
	register int		i;
	register PAGE_BUF	**poolp;
	char			*cp;

	poolp = (PAGE_BUF **) myalloc(nbufs * (sizeof(PAGE_BUF *) + sizeof(PAGE_BUF)));
	cp = (char *) poolp + (nbufs * sizeof(PAGE_BUF *));
	poolp[0] = (PAGE_BUF *) cp;

	poolp[0]->pgno = -1;
	for (i=1; i<nbufs; i++)
	{
		poolp[i] = poolp[i-1] + 1;
		poolp[i]->pgno = -1;
	}

	return(poolp);
}
		
/* CloseIndex()
||	close the index and sub-list files and free RTree structures
*/
CloseIndex(rtreep)
RTREE	*rtreep;
{
	/* Write out root page if it was opened to write */
	if (rtreep->opnmode & O_RDWR)
		PutOnePage(rtreep, 0, rtreep->root);

	/* Close SUB-LIST file */
	closea(rtreep->rtsfd);

	/* CLose R-Tree index & de-allocate structs */
	close(rtreep->rtifd);
	freeNode(rtreep, rtreep->root);
	freeRect(rtreep->CoverAll);
	myfree(rtreep->rtstats);
	myfree(rtreep->Page_Pool);
	myfree(rtreep->rthead);
	myfree(rtreep);
}

/* DropIndex()
||	remove (unlink) index & sub-list files
*/
int
DropIndex(indexName )
char	*indexName;
{
	char	fname[MAXNAMLEN];

	/* Form index file name & unlink it */
	strcpy(fname, indexName);
	strcat(fname, IXSUFFIX);
	unlink(fname);

	/* Form sublist file name & unlink it */
	strcpy(fname, indexName);
	strcat(fname, SLSUFFIX);
	unlink(fname);
}

/* int	GetOneRect(fp, rtreep, rect)
||	Call user defined input fct to read rect & 'data'
|| Currently 'data' is assumed to be an int.
*/
int
GetOneRect(fp, rtreep, rect)
FILE		*fp;
RTREE		*rtreep;
struct Rect	*rect;
{
	static	float	coord[MAXSIDES];
	register int	i;
	int		id, nsides;

	/* Call user defined input/conversion fct */
	nsides = rtreep->rthead->NumSides;
	if ( (id = RTFctIn(rtreep, fp, coord, nsides)) == 0)
		return(id);

	/* Fill in the "rectangle" */
	for (i=0; i< nsides; i++)
		rect->boundary[i] = (COORD_TYPE) coord[i];

	return (id);
}

/* int	GetManyRect(fp, rtreep, rect, id)
||	Call GetOneRect until a TERMINAL node is filled or end
|| of file (id ==0).
*/
int
GetManyRect(fp, rtreep, rect, id)
FILE		*fp;
RTREE		*rtreep;
struct Rect	**rect;
int		*id;
{
	register int	i=0;
	register int	ndcard = rtreep->rthead->TNodeCard;
 
	while ((id[i] = GetOneRect(fp, rtreep, rect[i])) != 0)
	{
		i++;
		if (i == ndcard)
			break;
	}
	return (i);
}


int
InsertManyRect(rtreep, rect, id, num)
RTREE		*rtreep;
struct Rect	**rect;
int		*id, num;
{
	register int	j;

	/* Assertion Changed - id's are now page addressed */
	for (j=0;j<num;j++)
		assert(id[j] != 0);

	return(Pa_InsertRect(rtreep, rect, id, 0, num));
}


/* int	PackInput(rtreep, fileName)
||	Insert rectangles & data (ids) into the R-Tree a TERMINAL
|| Node at a time.
*/
int
PackInput(rtreep, fileName)
RTREE	*rtreep;
char	*fileName;
{
	register	int i, n_uniq;
	FILE		*fp;
	struct Rect	**rectins, **rectrd;
	struct Rect	*rhold1, *rhold2, *rtmp;
	long		num, *idrd, *idins;
	long		newaddr, savaddr;
	int		ndcard, ndims;
	FLAG		ins_flg, newblk_flg;

	if ((fp = fopen( fileName, "r" )) == NULL )
	{
		ERROR1("Unable to open data file '%s'.", fileName );
		return FALSE;
	}

	/* Init dimension & (terminal) node cardinality */
	ndcard = rtreep->rthead->TNodeCard;
	ndims = rtreep->rthead->NumDims;

	/* Init arrays to hold a (terminal) node's no.  of rectangles */
	rectrd = (struct Rect **) myalloc(ndcard * sizeof(struct Rect *));
	rectins = (struct Rect **) myalloc((ndcard + 1) * sizeof(struct Rect *));
	rhold1 = rectrd[0] = newRect(ndims, ndcard);
	rhold2 = rectins[0]= newRect(ndims, ndcard+1);
	for (i=1; i<ndcard; i++)
	{
		rectrd[i] = rectrd[i-1] + 1;
		rectins[i] = rectins[i-1] + 1;
	}
	rectins[ndcard] = rectins[ndcard-1] + 1;

	/* Allocate enough space for 2 (terminal) node's no. of ids */
	idrd = (long *) myalloc(ndcard * sizeof(long));
	idins = (long *) myalloc((ndcard+1) * sizeof(long));

	/* Init unique rectangle counter */
	n_uniq = 0;

	/* Read a "branch's" worth of nodes at a time */
	while (num = GetManyRect(fp, rtreep, rectrd, idrd))
	{
		/* Insert all the rects into the sublist */
		for (i=0; i<num; i++)
		{
			/* Insert a new id into the sub-list file.
			 * (if both rect indicies == 0 or rects are different)
			 */
			if (!n_uniq || (Equal2Rects(rectrd[i], rectins[n_uniq-1], ndims) == FALSE))
			{
				ins_flg = NEW;
				newaddr = inserta(rtreep->rtsfd, newaddr, idrd[i], ins_flg, &newblk_flg);
				/* Put the SL addr in insert list:
				 * Note: the addr isn't valid till another unique id is found.
				 */
				if (n_uniq)
					idins[n_uniq-1] = savaddr;
				savaddr = newaddr;

				/* Put the new rect in the insert list (swapping ptrs) */
				rtmp = rectrd[i];
				rectrd[i] = rectins[n_uniq];
				rectins[n_uniq] = rtmp;

				/* Insert sublist addrs into r-tree if we have enough */
				if (n_uniq == ndcard)
				{
					/* DEMO Max Exceeded, back out "gracefully" */
					if (InsertManyRect(rtreep, rectins, idins, ndcard) == -1)
					{
						myfree(idins);
						myfree(idrd);
						freeRect(rhold2);
						freeRect(rhold1);
						myfree(rectins);
						myfree(rectrd);
						return (TRUE);
					}

					/* Move last rect & id to first position */
					rtmp = rectins[n_uniq];
					rectins[n_uniq] = rectins[0];
					rectins[0] = rtmp;
					idins[0] = idins[n_uniq];
					n_uniq = 0;
				}

				/* Set "unique" index to next open slot */
				++n_uniq;
			}
			/* Put duplicates in sub-list file but not in r-tree */
			else
			{
				ins_flg = OLD;
				newaddr = inserta(rtreep->rtsfd, savaddr, idrd[i], ins_flg, &newblk_flg);

				/* If a new blk was created, save the ptr */
				if (newblk_flg == TRUE)
					savaddr = newaddr;
			}
#ifdef PRINT
			printf("Inserting ID [%d] into sublist - ", idrd[i]);
			printf("Rd Num: [%d], Uniq Num: [%d]\n", i, n_uniq);
#endif
		}
	}

	/* Insert any leftover entries into the r-tree */
	if (n_uniq)
	{
		/* Put the last address into the last id slot */
		idins[n_uniq-1] = savaddr;

		if (n_uniq < ndcard)
			InsertManyRect(rtreep, rectins, idins, n_uniq);
		else
		{
			/* Extreme Boundary Condition:
			 * The last buffer contains 1 page + 1 nodes
			 */
			InsertManyRect(rtreep, rectins, idins, ndcard);
			rectins[0] = rectins[ndcard];
			idins[0] = idins[ndcard];
			InsertManyRect(rtreep, rectins, idins, 1);
		}
	}

	/* Free up rectangle & id arrays and return */
	myfree(idins);
	myfree(idrd);
	freeRect(rhold2);
	freeRect(rhold1);
	myfree(rectins);
	myfree(rectrd);
	return (TRUE);
}

int
BatchInsert(rtreep, fileName)
register RTREE	*rtreep;
char	*fileName;
{
	register int	i, n_uniq = 0, n_dup = 0;
	FILE		*fp;
	struct Rect	*rect;
	int		id, ndims, ndcard;

	/* Open batch search data file */
	if ((fp = fopen( fileName, "r" )) == NULL )
	{
		ERROR1("There is no input file %s", fileName);
		return FALSE;
	}

	/* Allocate the 1 Rect for insertion */
	ndims = rtreep->rthead->NumDims;
	rect =  newRect(ndims, 1);

	/* Allocate a stack of nodes (for searching purposes) */
	ndcard = MAX(rtreep->rthead->NodeCard, rtreep->rthead->TNodeCard);
	for (i=0; i<MAXLEVELS; i++)
		rtreep->SearchBuf[i] = newNode(rtreep, ndcard);

	/* Sequence thru input datafile */
	while (id = GetOneRect(fp, rtreep, rect))
	{
#ifdef PRINT
        	printf("Inserting ID = %d\n",id);
#endif
		if (!n_uniq)
		{
			if (InsertRect(rtreep, rect, id, 0) == -1)
				break;
			++n_uniq;
			continue;
		}

		/* Handle the possibility of duplicate rects by trying a
		 * InsertSearch first. If this fails do a straight insertion
		 * (it's a new rect)
		 */
		if (InsertSearch(rtreep, rect, rtreep->root, id) == FALSE)
		{
			if (InsertRect(rtreep, rect, id, 0) == -1)
				break;
			++n_uniq;
		}
		else
			++n_dup;
	}

	/* Output unique & duplicate counts */
	fprintf(stderr, "%d unique ids, %d duplicates found.\n", n_uniq, n_dup);

	/* De-Allocate the stack of nodes */
	for (i=MAXLEVELS-1; i>=0; i--)
		freeNode(rtreep, rtreep->SearchBuf[i]);

	/* Free the rect */
	freeRect(rect);

	return (TRUE);
}

/* int RectInsert(rtreep, rect, id)
||
||	Insert One Rectangle into the rtree
*/
int 
RectInsert(rtreep, rect, id)
RTREE		*rtreep;
struct Rect	*rect;
int		id;
{

#if	(RECT_LIMIT)
	if (++rect_count > RECT_LIMIT)
	{
		fprintf(stderr, demo_msg, rect_count);
		return(-1);
	}
#endif

	assert(id != 0 );
	return(InsertRect(rtreep, rect, id, 0));
}


/* int BatchDelete(rtreep, fileName)
||
||	Open the file of rectangles to dleete and call DeleteRect for each
|| one found. It allocates a Rect struct to be use as a current rect
|| buffer
*/
int
BatchDelete(rtreep, fileName)
RTREE	*rtreep;
char	*fileName;
{
	FILE		*fp;
	struct Rect	*rect;
	int		ndims;
	long		id;

	if ((fp = fopen(fileName, "r" )) == NULL)
	{
		ERROR1("There is no input file %s.", fileName);
		return FALSE;
	}   

	ndims = rtreep->rthead->NumDims;
	rect = newRect(ndims, 1);
	while (id = GetOneRect(fp, rtreep, rect))
	{
		printf ("\n");
		printf ("DELETE Rectangle -----\n");
		PrintRectIdent(rect, ndims);
		DeleteRect(rtreep, rect, id);
	}

	freeRect(rect);
	return (TRUE);
}

/* int RectDelete(rtreep, rect, id)
||
||	Delete one rectangle including all ids.
*/
int
RectDelete(rtreep, rect, id)
RTREE		*rtreep;
struct Rect	*rect;
long		id;
{
	DeleteRect(rtreep, rect, id);
	return (TRUE);
}


/* int BatchSearch(rtreep, fileName)
||
||	Open the file of rectangles to search and call Search for each
|| one found. It allocates a Rect struct and a stack of Node structs
|| as well
*/
int 
BatchSearch(rtreep, fileName)
register RTREE	*rtreep;
char	*fileName; 
{ 
	register int	i;
	FILE		*fp; 
	RT_STATS	*statsp;
	struct	Rect	*rect;
	int		ndims, ndcard;

	/* Open batch search data file */
	if ((fp = fopen( fileName, "r" )) == NULL )
	{
		ERROR1("There is no input file %s.", fileName); 
		return FALSE; 
	}  

	/* Allocate 1 Rect to search with */
	ndims = rtreep->rthead->NumDims;
	rect = newRect(ndims, 1);

	/* Allocate a stack of Nodes */
	ndcard = MAX(rtreep->rthead->NodeCard, rtreep->rthead->TNodeCard);
	for (i=0; i<MAXLEVELS; i++)
		rtreep->SearchBuf[i] = newNode(rtreep, ndcard);

	statsp = rtreep->rtstats;
	/* Sequence thru batch rect file */
	while (GetOneRect(fp, rtreep, rect))
	{
		printf ("\n");
		printf ("SEARCH Rectangle -----\n");
		PrintRectIdent(rect, ndims);

		(statsp->SearchCount)++;
		fprintf(rtreep->rtoutfp, "\n");

		i = SearchOneRect(rtreep, rtreep->root, rect);
		statsp->HitCount = statsp->HitCount + i;
	}

	/* De-allocate the  stack of Nodes & the 1 Rect*/
	for (i=MAXLEVELS-1; i >= 0; i--)
		freeNode(rtreep, rtreep->SearchBuf[i]);
	freeRect(rect);

	return (TRUE);
}

/* RectSearch
||
||	Search for all values w/in 1 rectangle
|| It allocates & dealloates a stack of Nodes in the process
*/
int 
RectSearch(rtreep, rect)
register RTREE	*rtreep;
struct Rect	*rect;
{ 
	register int	i, nfound, ndcard;
	RT_STATS	*statsp = rtreep->rtstats;

	/* Allocate a stack of Nodes */
	ndcard = MAX(rtreep->rthead->NodeCard, rtreep->rthead->TNodeCard);
	for (i=0; i<MAXLEVELS; i++)
		rtreep->SearchBuf[i] = newNode(rtreep, ndcard);

	/* Search one rectangle */
	++(statsp->SearchCount);

	nfound = SearchOneRect(rtreep, rtreep->root, rect);
	statsp->HitCount = statsp->HitCount + nfound;

	/* De-allocate the  stack of Nodes */
	for (i=MAXLEVELS-1; i >= 0; i--)
		freeNode(rtreep, rtreep->SearchBuf[i]);

	return (nfound);
}
