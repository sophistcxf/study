/*
|| printstats.c
||
|| Functions:
||	InsertStats(rtreep)
||	SearchStats(rtreep)
||	DeleteStats(rtreep)
||	GeneralStats(rtreep)
||	TreeStats(rtreep)
*/

#include	<stdio.h>

#include	"rtree.h"
#include	"assert.h"
#include	"timer.h"

int     NO_REFIN;

InsertStats(rtreep, rt_clock)
RTREE	*rtreep;
TIMER	*rt_clock;
{
	RT_STATS	*statsp = rtreep->rtstats;

	printf("\n");
	printf("rectangles inserted:\t\t%d\n", statsp->InsertCount);
	printf("splits:\t\t\t\t%d\n", statsp->InSplitCount);
	if (statsp->InSplitCount > 0)
		printf("average merit of splits:\t%3.2f\n", statsp->SplitMeritSum / statsp->InSplitCount);
	if (statsp->InsertCount > 0)
		printf("splits per insert:\t\t%3.2f\n", (float)statsp->InSplitCount/statsp->InsertCount);
	printf("partition evaluations:\t\t%d\n", statsp->EvalCount);
	if (statsp->InSplitCount > 0)
	{
		printf("evaluations per split:\t\t%3.2f\n",
		(float)statsp->EvalCount / statsp->InSplitCount);
	}
	if (statsp->CallCount > 0 && statsp->InSplitCount > 0)
	{
		printf("recursive calls per split:\t%d\n", statsp->CallCount / statsp->InSplitCount);
	}
	if (statsp->InsertCount > 0)
	{
		printf("evaluations per insert:\t\t%3.2f\n",
		(float)statsp->EvalCount / statsp->InsertCount);
	}
	if (statsp->InsertCount > 0)
	{
		printf("pages touched per insert:\t%3.2f\n",
		(float)statsp->InTouchCount / statsp->InsertCount);
	}
	printf("cpu time:\t\t\t%f\n", rt_clock->UserTime + rt_clock->SystemTime);
	if (statsp->InsertCount > 0)
		printf("cpu time per insert:\t\t%f\n",
			(rt_clock->UserTime+rt_clock->SystemTime) / statsp->InsertCount);
	if (statsp->InSplitCount > 0)
		printf("cpu time per split:\t\t%f\n",
			(rt_clock->UserTime+rt_clock->SystemTime) / statsp->InSplitCount);
	TreeStats(rtreep);
}

SearchStats(rtreep, rt_clock)
RTREE	*rtreep;
TIMER	*rt_clock;
{
	RT_STATS	*statsp = rtreep->rtstats;
	float		fraction;

	printf("\n");
	printf("searches:\t\t\t\t%d\n", statsp->SearchCount);
	printf("qualifying rects:\t\t\t%d\n", statsp->HitCount);
	printf("qualifying rects (no refinement):\t%d\n", NO_REFIN);
	if (statsp->SearchCount > 0)
		printf("avg hits per search:\t\t\t%3.2f\n", (float)statsp->HitCount / statsp->SearchCount);
	if ((statsp->RectCount) * (statsp->SearchCount) > 0)
	{
		fraction = (float)statsp->HitCount / ((statsp->RectCount) * (statsp->SearchCount));
		printf("avg fraction of data each search:\t%d%%\n",
			(int)(100 * fraction+0.5));
	}
	if (statsp->SearchCount > 0)
		printf("pages touched per search:\t\t%3.2f\n",
		(float) statsp->SeTouchCount / statsp->SearchCount);
	if (statsp->HitCount > 0)
	{
		printf("pages touched per hit:\t\t\t%3.2f\n",
		(float) statsp->SeTouchCount / statsp->HitCount);
	}
	printf("cpu time:\t\t\t\t%f\n", rt_clock->UserTime + rt_clock->SystemTime);
	if (statsp->SearchCount > 0)
		printf("cpu time per search:\t\t\t%f\n",
			(rt_clock->UserTime+rt_clock->SystemTime) / statsp->SearchCount);
	if (statsp->HitCount > 0)
		printf("cpu time per hit:\t\t\t%f\n", (rt_clock->UserTime+rt_clock->SystemTime) / statsp->HitCount);
}

DeleteStats(rtreep, rt_clock)
RTREE	*rtreep;
TIMER	*rt_clock;
{
	RT_STATS	*statsp = rtreep->rtstats;

	printf("\n");
	printf("rectangles deleted:\t\t%d\n", statsp->DeleteCount);
	printf("nodes eliminated:\t\t%d\n", statsp->ElimCount);
	if (statsp->DeleteCount > 0)
		printf("nodes eliminated per delete:\t%3.3f\n",
			(float)statsp->ElimCount / statsp->DeleteCount);
	printf("nodes reinserted:\t\t%d\n", statsp->ReInsertCount);
	if (statsp->ElimCount > 0)
		printf("reinsertions per node elim:\t%3.2f\n",
		(float)statsp->ReInsertCount / statsp->ElimCount);
	if (statsp->DeleteCount > 0)
		printf("reinsertions per rect delete:\t%3.3f\n",
		(float)statsp->ReInsertCount / statsp->DeleteCount);
	printf("node splits from reinsertions:\t%d\n", statsp->DeSplitCount);
	if (statsp->DeleteCount > 0)
	{
		printf("pages touched per delete:\t%3.2f\n",
		(float)statsp->DeTouchCount / statsp->DeleteCount);
	}
	printf("cpu time:\t\t\t%f\n", rt_clock->UserTime + rt_clock->SystemTime);
	if (statsp->DeleteCount > 0)
		printf("cpu time per delete:\t\t%f\n",
			(rt_clock->UserTime+rt_clock->SystemTime) / statsp->DeleteCount);
}

GeneralStats(rtreep)
RTREE	*rtreep;
{
	int	ndims, ndcard;

	ndims = rtreep->rthead->NumDims;
	ndcard = rtreep->rthead->NodeCard;

	assert(rtreep->MinFill>=2 && rtreep->MinFill<=((ndcard+1)/2));
	assert(rtreep->MinFill>=2 && rtreep->MinFill<=((ndcard+1)/2));

	printf("\n");
	printf("bytes per page:\t\t\t%d\n", PAGESIZE);
	printf("dimensions:\t\t\t%d\n", ndims);
	printf("entries per node:\t\t%d\n", ndcard);
	printf("minimum node fill:\t\t%d\n", rtreep->MinFill);
}

TreeStats(rtreep)
RTREE	*rtreep;
{
	RT_STATS	*statsp = rtreep->rtstats;
	int	ndcard;

	assert(statsp->RectCount >= 0);
	ndcard = rtreep->rthead->NodeCard;

	printf("\n");
	printf("rectangles indexed:\t\t%d\n", statsp->RectCount);
	printf("levels in tree:\t\t\t%d\n", rtreep->root->level + 1);
	printf("nodes in tree:\t\t\t%d\n", statsp->NodeCount);
	printf("leaf nodes:\t\t\t%d\n", statsp->LeafCount);
	printf("non-leaf nodes:\t\t\t%d\n", statsp->NonLeafCount);
	if (statsp->RectCount > 0)
		printf("index nodes per data item:\t%3.2f\n",
			(float)statsp->NodeCount / statsp->RectCount);
	printf("size of index in bytes:\t\t%d\n", (statsp->NodeCount) * PAGESIZE);
	printf("no. of pages read:\t\t%d\n", statsp->PagesRead);
	printf("no. of pages written:\t\t%d\n", statsp->PagesWritten);
	printf("no. of cache hits:\t\t%d\n", statsp->CacheHits);
	if (statsp->RectCount > 0)
		printf("index bytes per data item:\t%3.2f\n",
			(float) ((statsp->NodeCount) * PAGESIZE) / statsp->RectCount);
	printf("total entries:\t\t\t%d\n", statsp->EntryCount);
	printf("total slots for entries:\t%d\n", (statsp->NodeCount) * ndcard);
	if (statsp->NodeCount > 0)
		printf("space utilization:\t\t%d%%\n",
			(int) (100 * (float) statsp->EntryCount/((statsp->NodeCount) * ndcard)));
}
