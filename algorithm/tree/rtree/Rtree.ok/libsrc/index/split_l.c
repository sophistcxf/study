/*
|| split_l.c
||
|| Functions:
||	SplitNode(rtreep, n, b, nn)
||	PrintPVars(p)
||	GetBranches(p, n, b)
||	MethodZero(p)
||	PickSeeds(p)
||	Pigeonhole(p)
||	Classify(i, group, p)
||	LoadNodes(rtreep, n, q, p)
||	InitPVars(p)
||	PrintPVars(p)
*/

#include <stdio.h>

#include "rtree.h"
#include "assert.h"

	/* R-Tree Functions Returning Ptrs */
struct	Rect	*newRect();
struct	Branch	*newBranch();
struct	Node	*newNode();

float            RectArea();

/*-----------------------------------------------------------------------------
| Split a node.
| Divides the nodes branches and the extra one between two nodes.
| Old node is one of the new ones, and one really new one is created.
-----------------------------------------------------------------------------*/
SplitNode(rtreep, n, b, nn)
RTREE	*rtreep;
register struct Node *n;
register struct Branch *b;
register struct Node **nn;
{
	register struct	PartitionVars *p;
	register int	i, level;
	register float	area;

	RT_STATS	*statsp = rtreep->rtstats;
	int		ndims, ndcard;

	assert(n);
	assert(b);

	/* Get r-tree dimension * node cardinality */
	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;

#ifdef PRINT
	printf("Splitting:\n");
	PrintNode(rtreep, n);
	PrintBranch(b, ndims);
#endif

	if (statsp->StatFlag)
	{
		if (statsp->Deleting)
			statsp->DeSplitCount++;
		else
			statsp->InSplitCount++;
	}

	/* Get partition struct ptr & set node cardinality */
	level = n->level;
	p = rtreep->rtpartvars;
	ndcard = p->NodeCard = (level > 0)
		? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard ;

	/* load all the branches into a buffer, initialize old node */
	GetBranches(p, n, b);

	/* Use MethodZero to find partitions */
	MethodZero(rtreep, p);

#ifdef PRINT
	PrintPVars(p);
#endif

	/* record how good the split was for statistics */
	area = p->area[0] + p->area[1];
	if (statsp->StatFlag && !statsp->Deleting && area)
		statsp->SplitMeritSum += RectArea(p->CoverSplit, ndims) / area;
	(statsp->NodeCount)++;

	/* put branches from buffer into 2 nodes according to chosen partition */
	*nn = newNode(rtreep, ndcard);
	(*nn)->level = n->level = level;
	LoadNodes(rtreep, n, *nn, p);
	assert(n->count + (*nn)->count == ndcard+1);

#ifdef PRINT
	printf("group 0:\n");
	PrintNode(rtreep, n);
	printf("group 1:\n");
	PrintNode(rtreep, *nn);
	printf("\n");
#endif

}

/*-----------------------------------------------------------------------------
| Load branch buffer with branches from full node plus the extra branch.
-----------------------------------------------------------------------------*/
GetBranches(p, n, b)
register struct PartitionVars	*p;
register struct Node		*n;
register struct Branch		*b;
{
	register int	i;
	int		ndcard, ndims;

	assert(n);
	assert(b);

	/* load the branch buffer */
	ndcard = p->NodeCard;
	ndims = p->NumDims;
	for (i=0; i<ndcard; i++)
	{
		assert(n->branch[i]->brobj.son);  /* node should have every entry full */
		copyBranch(p->BranchBuf[i], n->branch[i], ndims);
	}
	copyBranch(p->BranchBuf[ndcard], b, ndims);

	/* calculate rect containing all in the set */
	copyRect(p->CoverSplit, p->BranchBuf[0]->rect, ndims);
	for (i=1; i<ndcard+1; i++)
	{
		CombineRect(p->CoverSplit, p->CoverSplit, p->BranchBuf[i]->rect, ndims);
	}

	InitNode(n, ndims, ndcard);
}

/*-----------------------------------------------------------------------------
| Method 0 for finding a partition:	(QUADRATIC version)
| First find two seeds, one for each group, well separated.
| Then put other rects in whichever group will be smallest after addition.
-----------------------------------------------------------------------------*/
MethodZero(rtreep, p)
RTREE	*rtreep;
register struct PartitionVars *p;
{
	initPVars(p);
	PickSeeds(p);
	Pigeonhole(rtreep, p);
}

/*-----------------------------------------------------------------------------
| Pick two rects from set to be the first elements of the two groups.
| Pick the two that are separated most along any dimension, or overlap least.
| Distance for separation or overlap is measured modulo the width of the
| space covered by the entire set along that dimension.
-----------------------------------------------------------------------------*/
PickSeeds(p)
register struct PartitionVars *p;
{
	register struct Rect	*r;
	register int		i, j;
	register float		d, dmax=0.0;
	int	ndims, ndcard, seed0, seed1;

	assert(p);

	ndims = p->NumDims;
	ndcard = p->NodeCard;
	r = newRect(ndims, 1);
	for (i=0; i<ndcard+1; i++)
		for (j=0; j<ndcard+1; j++)
		{
			CombineRect(r, p->BranchBuf[i]->rect, p->BranchBuf[j]->rect, ndims);
			d = RectArea(r, ndims) - RectArea(p->BranchBuf[i]->rect, ndims) - RectArea(p->BranchBuf[j]->rect, ndims);
			if (d > dmax)
			{
				dmax = d;
				seed0 = i;
				seed1 = j;
			}
		}

#ifdef PRINT
	printf("largest area enlargement = %f\n", dmax);
	printf("seed0 = %d  seed1 = %d\n", seed0, seed1);
	PrintRect(p->BranchBuf[seed0]->rect, ndims);
	PrintRect(p->BranchBuf[seed1]->rect, ndims);
#endif

	if (seed0 != seed1)
	{
		Classify(seed0, 0, p);
		Classify(seed1, 1, p);
	}
}

/*-----------------------------------------------------------------------------
| Put each rect that is not already in a group into a group.
| Process one rect at a time, using the following hierarchy of criteria.
| In case of a tie, go to the next test.
| 1) If one group already has the max number of elements that will allow
| the minimum fill for the other group, put r in the other.
| 2) Put r in the group whose cover will expand less.  This automatically
| takes care of the case where one group cover contains r.
| 3) Put r in the group whose cover will be smaller.  This takes care of the
| case where r is contained in both covers.
| 4) Put r in the group with fewer elements.
| 5) Put in group 1 (arbitrary).
|
| Also update the covers for both groups.
-----------------------------------------------------------------------------*/
Pigeonhole(rtreep, p)
RTREE	*rtreep;
register struct PartitionVars *p;
{
	register int	i, group, besti, bestg, assigned=0;
	struct	Rect	*newCover[2];
	int		newArea[2], increase[2], maxincrease;
	int		ndims, ndcard;

	/* Allocate Rects for newCover */
	ndcard = p->NodeCard;
	ndims = p->NumDims;
	newCover[0] = newRect(ndims, 2);
	newCover[1] = newCover[0] + 1;

	for (i=0; i<ndcard+1; i++)
		if (p->taken[i])
			assigned ++;

	while (assigned < ndcard+1)
	{
		/* if one group too full, put rects in the other regardless */
		if (rtreep->Pack && (p->count[0] == ndcard))
		{
			for (i=0; i<ndcard+1; i++)
				if (!p->taken[i])
				{
					Classify(i, 1, p);
					assigned ++;
				}
		}
		else if (rtreep->Pack)
		{
			for (i=0; i<ndcard+1; i++)
				if (!p->taken[i])
				{
					Classify(i, 0, p);
					assigned ++;
				}
		}
		else if (p->count[0] >= ndcard+1-(rtreep->MinFill))
		{
			for (i=0; i<ndcard+1; i++)
				if (!p->taken[i])
				{
					Classify(i, 1, p);
					assigned ++;
				}
		}
		else if (p->count[1] >= ndcard+1-(rtreep->MinFill))
		{
			for (i=0; i<ndcard+1; i++)
				if (!p->taken[i])
				{
					Classify(i, 0, p);
					assigned ++;
				}
		}
		else
		{
			/* find entry with greatest preference for one group */
			maxincrease = -1;
			for (i=0; i<ndcard+1; i++)
				if (!p->taken[i])
				{
					for (group=0; group<2; group++)
					{
						if (p->count[group]>0)
							CombineRect(newCover[group], p->BranchBuf[i]->rect, p->cover[group], ndims);
						else
							copyRect(newCover[group], p->BranchBuf[i]->rect, ndims);
						newArea[group] = RectArea(newCover[group], ndims);
						increase[group] = newArea[group] - p->area[group];
					}
					if (ABS(increase[0] - increase[1]) > maxincrease)
					{
						maxincrease = ABS(increase[0] - increase[1]);
						besti = i;
						if (increase[0] < increase[1])
							bestg = 0;
						else
							bestg = 1;
					}
				}
			Classify(besti, bestg, p);
			assigned ++;
		}
	}

	assert(p->count[0] + p->count[1] == ndcard + 1);
	freeRect(newCover[0]);
	return;
}

/*-----------------------------------------------------------------------------
| Put a branch in one of the groups.
-----------------------------------------------------------------------------*/
Classify(i, group, p)
register int	i, group;
register struct PartitionVars *p;
{
	int	ndims;
	assert(p);
	assert(!p->taken[i]);

	ndims = p->NumDims;
	p->partition[i] = group;
	p->taken[i] = TRUE;

	if (p->count[group] == 0)
		copyRect(p->cover[group], p->BranchBuf[i]->rect, ndims);
	else
		CombineRect(p->cover[group], p->BranchBuf[i]->rect, p->cover[group], ndims);

	p->area[group] = RectArea(p->cover[group], ndims);
	p->count[group]++;
}

/*-----------------------------------------------------------------------------
| Copy branches from the buffer into two nodes according to the partition.
-----------------------------------------------------------------------------*/
LoadNodes(rtreep, n, q, p)
RTREE			*rtreep;
register struct Node	*n, *q;
register struct PartitionVars *p;
{
	register int i, ndcard;

	assert(n);
	assert(q);
	assert(p);

	ndcard = p->NodeCard;
	for (i=0; i<ndcard+1; i++)
	{
		if (p->partition[i] == 0)
			AddBranch(rtreep, p->BranchBuf[i], n, NULL);
		else if (p->partition[i] == 1)
			AddBranch(rtreep, p->BranchBuf[i], q, NULL);
		else
			assert(FALSE);
	}
}

/*-----------------------------------------------------------------------------
| Allocate a PartitionVars structure.
-----------------------------------------------------------------------------*/
struct PartitionVars *
newPVars(rtreep)
RTREE	*rtreep;
{
	register int		i;
	register PARTVARS	*p;
	int			ndims, ndcard;

	ndims = rtreep->rthead->NumDims;
	ndcard = MAX(rtreep->rthead->NodeCard, rtreep->rthead->TNodeCard);

	/* First Allocate the Partition Struct itself */
	p = (PARTVARS *) myalloc(sizeof(PARTVARS));

	/* Duplicate NodeCard & NumDim values (for now) */
	p->NodeCard = ndcard;
	p->NumDims = ndims;

	/* Allocate the Branch Buffer */
	p->BranchBuf = (struct Branch **) myalloc((ndcard +1) * sizeof(struct Branch *));
	p->BranchBuf[0] = newBranch(ndims, ndcard+1);

	/* Allocate the Cover Rects */
	p->CoverSplit = newRect(ndims, 1);
	p->cover[0] = newRect(ndims, 2);
	p->cover[1] = p->cover[0] + 1;

	/* Allocate & init taken/partition int arrays */
	p->partition = (int *) myalloc((ndcard+1) * sizeof(int));
	p->taken = (int *) myalloc((ndcard+1) * sizeof(int));
	for (i=0; i<ndcard+1; i++)
	{
		if (i)
			p->BranchBuf[i] = p->BranchBuf[i-1] + 1;
		p->taken[i] = FALSE;
		p->partition[i] = -1;
	}
	p->BranchBuf[ndcard] = p->BranchBuf[ndcard-1] + 1;

	/* Init count & area */
	p->count[0] = p->count[1] = 0;
	p->area[0] = p->area[1] = 0.0;

	/* Return the Partition Vars struct ptr */
	return(p);
}

/*-----------------------------------------------------------------------------
| Initialize a PartitionVars structure.
-----------------------------------------------------------------------------*/
initPVars(p)
register struct PartitionVars *p;
{
	register int	i;
	int		ndcard = p->NodeCard;
	assert(p);

	p->count[0] = p->count[1] = 0;
	for (i=0; i<ndcard+1; i++)
	{
		p->taken[i] = FALSE;
		p->partition[i] = -1;
	}
}

/*-----------------------------------------------------------------------------
| Print out data for a partition from PartitionVars struct.
-----------------------------------------------------------------------------*/
PrintPVars(p)
struct PartitionVars *p;
{
	register int	i;
	int		ndcard, ndims;

	assert(p);

	ndims = p->NumDims;
	ndcard = p->NodeCard;

	printf("\npartition:\n");
	for (i=0; i<ndcard+1; i++)
	{
		printf("%3d\t", i);
	}
	printf("\n");
	for (i=0; i<ndcard+1; i++)
	{
		if (p->taken[i])
			printf("  t\t");
		else
			printf("\t");
	}
	printf("\n");
	for (i=0; i<ndcard+1; i++)
	{
		printf("%3d\t", p->partition[i]);
	}
	printf("\n");

	printf("count[0] = %d  area = %f\n", p->count[0], p->area[0]);
	printf("count[1] = %d  area = %f\n", p->count[1], p->area[1]);
	printf("total area = %f  effectiveness = %3.2f\n",
		p->area[0] + p->area[1],
		RectArea(p->CoverSplit, ndims) / (p->area[0] + p->area[1]));

	printf("cover[0]:\n");
	PrintRect(p->cover[0], ndims);

	printf("cover[1]:\n");
	PrintRect(p->cover[1], ndims);
}

/*-----------------------------------------------------------------------------
| Split a node.
| Divides the nodes branches and the extra one between two nodes.
| Old node is one of the new ones, and one really new one is created.
-----------------------------------------------------------------------------*/
Pa_SplitNode(rtreep, n, b, nn, num)
RTREE	*rtreep;
struct	Node	*n;
struct	Branch	*b;
struct	Node	**nn;
register int	num;
{
	register int	j, level;
	RT_STATS	*statsp = rtreep->rtstats;
	int		ndims, ndcard;

	assert(n);
	for (j=0; j<num; j++)
		assert(&b[j]);

	level = n->level;
	ndcard = (level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;
	ndims = rtreep->rthead->NumDims;

#ifdef PRINT
	printf("Splitting:\n");
	PrintNode(rtreep, n);
	for (j=0;j<num;j++)
		PrintBranch(&b[j], ndims);
#endif

	if (statsp->StatFlag)
	{
		if (statsp->Deleting)
			statsp->DeSplitCount++;
		else
			statsp->InSplitCount++;
	}
	(statsp->NodeCount)++;

	/* put branches from buffer into 2 nodes according to chosen partition */
	*nn = newNode(rtreep, ndcard);
	(*nn)->level = n->level = level;

	for (j=0; j<num; j++)
		AddBranch(rtreep, &b[j], *nn, NULL);

#ifdef PRINT
	printf("group 0:\n");
	PrintNode(rtreep, n);
	printf("group 1:\n");
	PrintNode(rtreep, *nn);
	printf("\n");
#endif
}
