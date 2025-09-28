/* node.c
||
|| Description:
||	This module contains functions to allocate, free & init nodes.
||
|| Functions:
||	struct	Node	*newNode(rtreep, ndcard)
||			freeNode(rtreep, p)
||			InitNode(n, ndims, ndcard)
||	struct	Branch	*newBranch(ndims, nbranches)
||			initBranch(b, ndims)
||			freeBranch(b)
||			copyBranch(bdest, bsrc, ndims)
||			NodeCover(rtreep, n, b)
||			PickBranch(rtreep, r, n)
||			AddBranch(rtreep, b, n, new)
||			DisconBranch(n, i, ndims)
||			PrintNode(rtreep, n)
||			PrintBranch(b, ndims)
*/

#include <stdio.h>

#include "rtree.h"
#include "assert.h"


	/* R-Tree Functions Returning Ptrs */
char	*myalloc();
struct	Rect	*newRect();
struct	Branch	*newBranch();
struct	Node	*newNode();
float		RectArea();


/* struct Node * newNode(rtreep, ndcard)
||	 Make a new node and initialize to have all branch cells empty.
*/
struct Node *
newNode(rtreep, ndcard)
RTREE	*rtreep;
{
	register struct Node	*nodep;
	register struct Branch	*branchp;
	register struct Rect	*rectp;
	register int		i;

	char		*cp;
	int		ndims;


	/* 1st Allocate a Node Struct & Branch Structs & Ptrs */
	ndims = rtreep->rthead->NumDims;
	nodep = (struct Node *) myalloc(sizeof (struct Node) +
		ndcard * sizeof(struct Branch *) + ndcard * sizeof(struct Branch));

	/* Next, allocate Rect Structs & data space */
	rectp = newRect(ndims, ndcard);

	/* Connect the Node to Branches & Rects */
	cp = (char *) nodep + sizeof(struct Node);
	nodep->branch = (struct Branch **) cp;
	cp += (ndcard * sizeof (struct Branch *));
	branchp = (struct Branch *) cp;
	for (i=0; i<ndcard; i++)
	{
		nodep->branch[i] = branchp++;
		nodep->branch[i]->rect = rectp++;
	}

	InitNode(nodep, ndims, ndcard);
	return (nodep);
}

/* freeNode(rtreep, nodep)
||	Free a Node Struct & Underlying Structs
*/
freeNode(rtreep, nodep)
RTREE			*rtreep;
register struct Node	*nodep;
{

	/* First free the rectangles */
	myfree(nodep->branch[0]->rect);

	/* Now the Node & Branch structs */
	myfree(nodep);

	return;
}

/* InitNode()
||	Initialize a Node structure
*/
InitNode(nodep, ndims, ndcard)
register struct Node	*nodep;	/* Node Ptr */
int			ndims;	/* No. of dimensions */
int			ndcard;	/* Node Cardinality */
{
	register int i;

	nodep->count = 0;
	nodep->level = -1;
	for (i = 0; i < ndcard; i++)
		initBranch(nodep->branch[i], ndims);
}


/* struct Branch * newBranch(ndims, nbranches)
||	Allocate one or more Branch Structs
*/
struct Branch *
newBranch(ndims, nbranches)
int	ndims;		/* No. of Dimensions in a rectangle */
int	nbranches;	/* No. of branches to allocate */
{
	register int		i;
	register struct Branch	*b;
	register struct Rect	*r;
	register char		*cp;
	struct Branch		*branch;

	assert(nbranches>0);
	assert(ndims>0);

	/* Allocate all Branch & Rect structs at once */
	branch = b = (struct Branch *) myalloc(nbranches * (sizeof(struct Branch) +
		sizeof(struct Rect) +2 * ndims * COORD_SIZE));
	cp = (char *) b + nbranches * sizeof(struct Branch);
	r = (struct Rect *) cp;
	cp += (nbranches * sizeof(struct Rect));

	/* Link Data to Rects and Rects to Branches */
	for (i=0; i<nbranches; i++)
	{
		r->boundary = (COORD_TYPE *) cp;
		b->rect = r++;
		b++; cp += (ndims * 2 * COORD_SIZE);
	}

	/* Return a pointer to the branch array */
	return(branch);
}

/* initBranch(b, ndims)
||	Initialize one branch cell in a node.
*/
initBranch(b, ndims)
struct	Branch	*b;
int		ndims;
{
	initRect(b->rect, ndims);
	b->brobj.son = 0;
}

/* copyBranch(bdest, bsrc, ndims)
||	 Copy the contents of one branch to another
*/
copyBranch(bdest, bsrc, ndims)
struct	Branch	*bdest, *bsrc;
int		ndims;
{
	copyRect(bdest->rect, bsrc->rect, ndims);
	bdest->brobj.son = bsrc->brobj.son;
}

/* freeBranch(b)
||	De-Allocate a Branch
|| Since it was allocated in one piece, deallocate it all at once
*/
freeBranch(b)
struct	Branch	*b;
{
	myfree(b);
}


/* NodeCover(rtreep, n, r)
||	Find the smallest rectangle that includes all rectangles in
||	branches of a node.
*/
NodeCover(rtreep, n, r)
RTREE			*rtreep;
register struct Node	*n;
register struct Rect	*r;
{
	register int	i, flag;
	int		ndims, ndcard;

	assert(n);

	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;
	initRect(r, ndims);

	flag = 1;
	for (i = 0; i < ndcard; i++)
		if (n->branch[i]->brobj.son != 0)
		{
			if (flag)
			{
				copyRect(r, n->branch[i]->rect, ndims);
				flag = 0;
			}
			else
				CombineRect(r, r, n->branch[i]->rect, ndims);
		}

	return;
}

/* Pick a branch.  Pick the one that will need the smallest increase
** in area to accomodate the new rectangle.  This will result in the
** least total area for the covering rectangles in the current node.
** In case of a tie, pick the one which was smaller before, to get
** the best resolution when searching.
*/
int
PickBranch(rtreep, r, n)
RTREE		*rtreep;
struct	Rect	*r;
struct	Node	*n;
{
	register struct	Rect *rr;
	register int	i, flag, increase, bestIncr;
	register float	area, bestArea;
	struct	Rect	*rtmp;
	int		best, ndims, ndcard;

	assert(r && n);

	flag = 1;
	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;
	rtmp = newRect(ndims, 1);

	for (i=0; i<ndcard; i++)
	{
		if (n->branch[i]->brobj.son != 0)
		{
			rr = n->branch[i]->rect;

			area = RectArea(rr, ndims);
			CombineRect(rtmp, r, rr, ndims);
			increase = (int) (RectArea(rtmp, ndims) - area);

			if (increase <  bestIncr || flag)
			{
				best = i;
				bestArea = area;
				bestIncr = increase;
				flag = 0;
			}
			else if (increase == bestIncr && area < bestArea)
			{
				best = i;
				bestArea = area;
				bestIncr = increase;
			}
#ifdef	PRINT
			printf("i=%d  area before=%f  area after=%f  increase=%d\n",
				i, area, area+increase, increase);
#endif
		}
	}
#ifdef	PRINT
	printf("\tpicked %d\n", best);
#endif
	freeRect(rtmp);
	return (best);
}

/* Add a branch to a node.  Split the node if necessary.
** Returns 0 if node not split.  Old node updated.
** Returns 1 if node split, sets *new to address of new node.
** Old node updated, becomes one of two.
*/
int
AddBranch(rtreep, b, n, new)
RTREE	*rtreep;
register struct Branch	*b;
register struct Node	*n;
register struct Node	**new;
{
	register int	i;
	RT_STATS	*statsp = rtreep->rtstats;
	int		ndims, ndcard;

	assert(b);
	assert(n);

	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;

	if (n->count < ndcard)  /* split won't be necessary */
	{
		for (i = 0; i < ndcard; i++)  /* find empty branch */
		{
			if (n->branch[i]->brobj.son == 0)
			{
				copyBranch(n->branch[i], b, ndims);
				n->count++;
				break;
			}
		}
		assert(i<ndcard);
		return (0);
	}
	else
	{
		if (statsp->StatFlag)
		{
			if (statsp->Deleting)
				statsp->DeTouchCount++;
			else
				statsp->InTouchCount++;
		}
		assert(new);
		SplitNode(rtreep, n, b, new);
		if (n->level == 0)
			(statsp->LeafCount)++;
		else
			(statsp->NonLeafCount)++;
		return (1);
	}
}

/* Disconnect a dependent node. */
DisconBranch(n, i, ndims)
struct	Node	*n;
int		i;
int		ndims;
{
	assert(n && (n->branch[i]->brobj.son != 0));

	initBranch(n->branch[i], ndims);
	n->count--;
}

int
Pa_PickBranch(rtreep, r, n)
RTREE		*rtreep;
register struct Rect *r;
register struct Node *n;
{
	register struct Rect *rr;
	register int	i;
	int		best, ndcard;

	assert(r && n);
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;

#ifdef PRINT
	printf("Pa_PickBrunch\n");
#endif

	for (i=0; i<ndcard; i++)
	{
		if (n->branch[i]->brobj.son != 0)
		{
			rr = n->branch[i]->rect;
			if (rr->boundary[0] <= r->boundary[0])
				best = i+1;
		}
	}
	return (best-1);
}


/* Add a branch to a node.  Split the node if necessary.
** Returns 0 if node not split.  Old node updated.
** Returns 1 if node split, sets *new to address of new node.
** Old node updated, becomes one of two.
*/
int
Pa_AddBranch(rtreep, b, n, new, num)
RTREE	*rtreep;
register struct Branch	*b;
register struct Node	*n;
register struct Node	**new;
register int		num;
{
	register int	i;
	RT_STATS	*statsp = rtreep->rtstats;
	int		ndims, ndcard;

	assert(n);

#ifdef PRINT
	printf("In Pa_AddBranch  num=%d\n", num);
#endif

	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;

	if (n->count < ndcard)  /* split won't be necessary */
	{
		for (i=0; i<ndcard && i<num; i++)  /* find empty branch */
		{
			if (n->branch[i]->brobj.son == 0)
			{
				copyBranch(n->branch[i], &b[i], ndims);
				n->count++;
			}
		}
		assert(i<=ndcard);
#ifdef PRINT
		printf("Pa_AddBranch:  SPLIT NOT needed\n");
		PrintNode(rtreep, n);
#endif
		return (0);
	}
	else
	{
		if (statsp->StatFlag)
		{
			if (statsp->Deleting)
				statsp->DeTouchCount++;
			else
				statsp->InTouchCount++;
		}
		assert(new);

		Pa_SplitNode(rtreep, n, b, new, num);
		if (n->level == 0)
			(statsp->LeafCount)++;
		else
			(statsp->NonLeafCount)++;
#ifdef PRINT
		printf("Pa_AddBranch:  SPLIT was  needed\n");
		printf("========================================\n\tOld Node\n");
		PrintNode(rtreep, n);
		printf("========================================\n\tNew Node\n");
		PrintNode(rtreep, *new);
		printf("========================================\n");
#endif
		return (1);
	}
}

/*===================================================================*
 *      Node & Branch Print Functions                                *
 *===================================================================*/
/* PrintNode
||	Print out the data in a node.
*/
PrintNode(rtreep, n)
RTREE		*rtreep;
struct Node	*n;
{
	register int	i;
	int		ndims, ndcard;

	assert(n);

	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;

	if (n->level == 0)
		printf("Leaf Node:");
	else if (n->level > 0)
		printf("Non-Leaf Node:");

	printf("  level=%d  count=%d  address=%d\n", n->level, n->count, n->pageNo);
	for (i=0; i<ndcard; i++)
		if((n->branch[i]->brobj.son) > 0)
		{
			printf("branch %d : rect id %d\n", i, n->branch[i]->brobj.son);
			PrintBranch(n->branch[i], ndims);
		}
}

/* PrintBranch
||	Print out the data in a branch
*/
PrintBranch(b, ndims)
struct Branch *b;
{
#ifdef PRINT
		printf("page number %d\n", b->brobj.son);
#endif 

		PrintRect(b->rect, ndims);
}
