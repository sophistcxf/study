/*
|| page.c
||
|| Functions:
||	struct Node	*GetOnePage(rtreep, offset)
||	void	 	PutOnePage(rtreep, offset, node)
||	void	 	ReadOnePage(rtreep, offset, node)
||	int		GetBuffer(rtreep, pgno, rd_flag)
||	void		PutBuffer(rtreep, bufno)
*/

#include <stdio.h>

#ifdef	VMS
#include	<types.h>
#include	<file.h>
#define		off_t	int
#define		L_SET	0
#else
#include	<sys/types.h>
#include	<sys/file.h>
#endif

#include	"rtree.h"
#include	"assert.h"
#include	"error.h"

	/* Define a union to be able to extract the level from a buffer */
typedef	union	cvrt_int
{
	LEVEL_TYPE	cv_int;
	char		cv_char[LEVEL_SIZE];
}	LVL_CVT;


	/* R-Tree Functions Returning Ptrs */
struct	Node	*newNode();

struct	Node *
GetOnePage(rtreep, offset)
RTREE	*rtreep;
off_t	offset;
{
	register int		i, displ, rsize, pt_flag;
	register struct Node	*node;

	LVL_CVT	lvl_buf;
	char	*ws;
	int	ndcard, bno, level;

	assert(offset >= 0);

	/* Get a buffer from the buffer pool for this page */
	bno = GetBuffer(rtreep, offset, 1);
	ws = rtreep->Page_Pool[bno]->buf;

	/* Determine type of node & allocate accordingly */
	memcpy(lvl_buf.cv_char, ws+COUNT_SIZE, LEVEL_SIZE);
	level = lvl_buf.cv_int;
	ndcard = (level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;
	node = newNode(rtreep, ndcard);

	/* Init page & level fields */
	node->pageNo = offset;
	node->level = level;

	/* Copy the node count field into the new node */
	memcpy((char *) &(node->count), ws, COUNT_SIZE);

	/* Copy the coord values & son ptr from the buffer into the node */
	rsize = rtreep->rthead->NumSides * COORD_SIZE;
	displ = COUNT_SIZE + LEVEL_SIZE;

	pt_flag = (level == 0) && (rtreep->rthead->TNodeType == POINT_TYPE);
	if (pt_flag)
		rsize = 2 * COORD_SIZE;

	for (i=0; i<ndcard; i++)
	{
		memcpy((char *) node->branch[i]->rect->boundary, ws+displ, rsize);
		if (pt_flag)
			memcpy((char *) (node->branch[i]->rect->boundary) + rsize, ws+displ, rsize);
		memcpy((char *) &(node->branch[i]->brobj.son), ws+displ+rsize, POINTER_SIZE);
		displ += (rsize + POINTER_SIZE);
	}

	/* the reading page must be the root node */
	if (rtreep->pageNo == 0)
		memcpy((char *) &(rtreep->pageNo), ws+PAGESIZE-HEAD_SIZE, sizeof(int));

#ifdef	PRINT
	printf("I READ PAGE NUMBER %d\n",node->pageNo);
#endif
	return (node);
}

void 
PutOnePage(rtreep, offset, node)
RTREE			*rtreep;
off_t			offset;
register struct	Node	*node;
{
	register int	i, displ, rsize, pt_flag;
	char	*ws;
	int	ndcard, bno, level;

	assert(offset >= 0);

	/* Get a buffer from the buffer pool for this page */
	bno = GetBuffer(rtreep, offset, 0);
	ws = rtreep->Page_Pool[bno]->buf;

	/* Copy the node level & count values from the node into the buffer */
	memcpy(ws, (char *) &(node->count), COUNT_SIZE);
	memcpy(ws+COUNT_SIZE, (char *) &(node->level), LEVEL_SIZE);

	/* Copy the coord values & son ptr from the node into the buffer */
	level = node->level;
	rsize = rtreep->rthead->NumSides * COORD_SIZE;
	displ = COUNT_SIZE + LEVEL_SIZE;

	pt_flag = (level == 0) && (rtreep->rthead->TNodeType == POINT_TYPE);
	if (pt_flag)
		rsize = 2 * COORD_SIZE;

	ndcard = (level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;
	for (i=0; i<ndcard; i++)
	{
		memcpy(ws+displ, (char *) node->branch[i]->rect->boundary, rsize);
		memcpy(ws+displ+rsize, (char *) &(node->branch[i]->brobj.son), POINTER_SIZE);
		displ += (rsize + POINTER_SIZE);
	}

	if (offset == 0)
		memcpy(ws+PAGESIZE-HEAD_SIZE, (char *) &(rtreep->pageNo), sizeof(int));

	/* Write out the buffer */
	PutBuffer(rtreep, bno);

#ifdef PRINT
	printf("I WROTE PAGE NUMBER %d\n",node->pageNo);
#endif
	return;
}

int
ReadOnePage(rtreep, node, offset)
RTREE		*rtreep;
struct Node	*node;
off_t		offset;
{
	register int	i, displ, rsize, pt_flag;
	char	*ws;
	int	ndcard, bno, level;

	assert( offset >= 0 );
	node->pageNo = offset;

	/* Get a buffer from the buffer pool for this page */
	bno = GetBuffer(rtreep, offset, 1);
	ws = rtreep->Page_Pool[bno]->buf;

	/* Copy the node level & count values from the buffer into the node */
	memcpy((char *) &(node->count), ws, COUNT_SIZE);
	memcpy((char *) &(node->level), ws+COUNT_SIZE, LEVEL_SIZE);

	/* Copy the coord values & son ptr from the buffer into the node */
	level = node->level;
	rsize = rtreep->rthead->NumSides * COORD_SIZE;
	displ = COUNT_SIZE + LEVEL_SIZE;

	pt_flag = (level == 0) && (rtreep->rthead->TNodeType == POINT_TYPE);
	if (pt_flag)
		rsize = 2 * COORD_SIZE;

	ndcard = (level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;
	for (i=0; i<ndcard; i++)
	{
		memcpy((char *) node->branch[i]->rect->boundary, ws+displ, rsize);
		if (pt_flag)
			memcpy((char *) (node->branch[i]->rect->boundary) + rsize, ws+displ, rsize);
		memcpy((char *) &(node->branch[i]->brobj.son), ws+displ+rsize, POINTER_SIZE);
		displ += (rsize + POINTER_SIZE);
	}

	/* the reading page must be the root node */
	if (rtreep->pageNo == 0)
		memcpy((char *) &(rtreep->pageNo), ws+PAGESIZE-HEAD_SIZE, sizeof(int));

#ifdef PRINT
	printf("I READ PAGE NUMBER %d\n",node->pageNo);
#endif
	return(1);
}


/* GetBuffer(rtreep, pgno)
||	If a buffer for the page exists return it otherwise find and
|| fill the least recently used buffer and return it.
*/
int
GetBuffer(rtreep, pgno, rd_flag)
RTREE	*rtreep;	/* Infamous R-Tree ptr */
long	pgno;		/* Page to read */
int	rd_flag;	/* Flag != 0 means read the page */
{
	register PAGE_BUF	*pgpoolp;
	register int		i, bindx, musecnt;
	RT_STATS		*statsp = rtreep->rtstats;
	int			nbufs = rtreep->NumPageBufs;
	int			findx, lru_flag = 1;

	/* Init index & LRU counter */
	findx = -1;
	musecnt = bindx = 0;

	/* Update Page Stats */
	if (rd_flag)
		++statsp->PagesRead;
	else
		++statsp->PagesWritten;

	/* Scan thru page pool to find matching or LRU page */
	for (i=0; i<nbufs; i++)
	{
		/* Page found in mem; reset usage cnt */
		pgpoolp = rtreep->Page_Pool[i];
		if (pgpoolp->pgno == pgno)
		{
			findx = i;
			lru_flag = 0;

			if (rd_flag)
				++statsp->CacheHits;
			break;
		}

		else
		{
			/* Find LFU page */
			if (pgpoolp->usecnt > musecnt)
			{
				musecnt = pgpoolp->usecnt;
				bindx = i;
			}
			/* "Age" all pages */
			++pgpoolp->usecnt;
		}
	}

	/* Use the appropriate buffer index */
	bindx = (findx < 0) ? bindx : findx;
	pgpoolp = rtreep->Page_Pool[bindx];

	/* If the page wasn't in the page pool, read it in */
	if (rd_flag && lru_flag)
	{
		lseek(rtreep->rtifd, pgno*PAGESIZE, L_SET);
		if (read(rtreep->rtifd, pgpoolp->buf, PAGESIZE) != PAGESIZE)
		{
			ABORT("Incomplete page read from R-Tree.");
		}

	}

	/* Update usecnt & pgno in current buffer */
	pgpoolp->pgno = pgno;
	pgpoolp->usecnt = 1;

	return(bindx);
}


/* PutBuffer(bno)
||	Write out the buffer corresponding to bno
*/
PutBuffer(rtreep, bno)
RTREE	*rtreep;
int	bno;
{
	PAGE_BUF	*pgpoolp;

	pgpoolp = rtreep->Page_Pool[bno];

	lseek(rtreep->rtifd, pgpoolp->pgno*PAGESIZE, L_SET);
	if (write(rtreep->rtifd, pgpoolp->buf, PAGESIZE) != PAGESIZE)
	{
		ABORT("Unable to write a complete r-tree page.");
	}

	return;
}
