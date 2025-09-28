/*
|| index.c
||
|| Functions:
||	SearchOneRect(rtreep, n, r)
||	InsertSearch(rtreep, n, r, id)
||	InsertRect(rtreep, r, sonpage, root, level)
||	InsertRect2(rtreep, r, sonpage, n, new, level)
||	DeleteRect(rtreep, r, id)
||	DeleteRect2(rtreep, r, n, ee, id)
||	ReInsert(n, ee)
||	NewListNode()
||	FreeListNode(p)
||	PrintIndex(n)
||	PrintData(rtreep, n)
*/
#include	<stdio.h>

/* R-Tree Includes */
#include	"rtree.h"
#include	"assert.h"
#include	"error.h"

/* ADMS Includes */
#include	"asec.h"

	/* R-Tree Functions Returning Ptrs */
struct	ListNode	*NewListNode();
struct	Node		*GetOnePage(), *newNode();
struct	Branch		*newBranch();
struct	Rect		*newRect();
char			*myalloc();

	/* Sub Leaf Functions */
extern	long	xgettuple();

	/* Define local variable/constant for demo copy */
#if	(RECT_LIMIT)
static	int	rect_count = 0;
static	char	demo_msg[] =  "Maximun rectangle count (%d) exceeded in demonstration copy.\n";
#endif


/* int SearchOneRect(rtreep, n, r)
||	Search in an index tree or subtree for all data retangles that
||	overlap the argument rectangle.
||	Returns the number of qualifying data rects.
*/
int
SearchOneRect(rtreep, n, r)
RTREE		*rtreep;
register struct	Node *n;
register struct	Rect *r;
{
	register	int		hitCount = 0;
	register	int		i, level = n->level;
	register	struct Node	*s_node;

	RT_STATS	*statsp = rtreep->rtstats;
	int		ndcard, ndims;

	assert(n);
	assert(level >= 0);
	assert(r);

	ndims = rtreep->rthead->NumDims;
	++statsp->SeTouchCount;

	if (level > 0) /* this is an internal node in the tree */
	{
		s_node = rtreep->SearchBuf[level];
		ndcard = rtreep->rthead->NodeCard;

		for (i=0; i<ndcard; i++)
			if ((n->branch[i]->brobj.son > 0) &&
				RTFctRCmp(rtreep, r, n->branch[i]->rect, ndims))
			{
				ReadOnePage(rtreep, s_node, n->branch[i]->brobj.son);
				hitCount += SearchOneRect(rtreep, s_node, r);
			}
	}
	else /* this is a leaf node */
	{
		ndcard = rtreep->rthead->TNodeCard;

		for (i=0; i<ndcard; i++)
		{
			if ((n->branch[i]->brobj.son != 0) &&
				RTFctRCmpT(rtreep, r, n->branch[i]->rect, ndims))
			{
#ifdef	PRINT
				printf ("----> Compares  with rectangle with sl addr %d\n",
					(n->branch[i]->brobj.son)); 
#endif
				/* SUB-LIST Access Done Here */
				hitCount += GetSLTuples(rtreep, n->branch[i]->rect, n->branch[i]->brobj.son);
			}
		}
	}
	return (hitCount);
}

/* InsertSearch
||
|| Description:
|| 	Search all branches for a given rect & if found, insert one as a
|| duplicate.
||
|| Return Values:
||	FALSE	- rect not found or inserted
||	TRUE	- rect found & inserted (addr may or not be changed)
*/
int 
InsertSearch(rtreep, r, n, id)
register RTREE	*rtreep;
struct Node	*n;
struct Rect	*r;
long		id;
{
	register int	i, c_level = n->level;
	struct Node	*s_node;
	FLAG		new_flg;		/* Sub-Leaf Block Split Flag */
	FLAG		ins_flg = OLD;		/* Sub-Leaf Ins Type Flag */
	long		sladdr;			/* Sub-Leaf File tuple ptr */
	int		rtn_val = FALSE;	/* Return Value */
	int		ndcard, ndims;		/* Rect Dimension, Node Cardinality */

	assert(n);
	assert(c_level >= 0);
	assert(r);

	ndims = rtreep->rthead->NumDims;

	/* Recursively search for an instance of rect */
	if (c_level > 0)
	{
		/* this is an internal node in the tree */
		ndcard = rtreep->rthead->NodeCard;
		s_node = rtreep->SearchBuf[c_level];
		s_node->level = c_level;

		for (i=0; i<ndcard; i++)
			if ((n->branch[i]->brobj.son > 0) &&
				Overlap(r, n->branch[i]->rect, ndims))
			{
				ReadOnePage(rtreep, s_node, n->branch[i]->brobj.son);
				rtn_val = InsertSearch(rtreep, r, s_node, id);

				/* We *are* assuming uniqueness here... */
				if (rtn_val)
				{
					if (rtn_val == NEW)
						PutOnePage(rtreep, n->branch[i]->brobj.son, s_node);
					rtn_val = TRUE;
					break;
				}
			}
	}
	else
	{
		/* this is a leaf node */
		ndcard = rtreep->rthead->TNodeCard;

		for (i=0; i<ndcard; i++)
		{
			if ((n->branch[i]->brobj.son != 0) &&
				(Equal2Rects(r, n->branch[i]->rect, ndims) == TRUE))
			{
				/* Get previous sub-leaf address */
				sladdr = n->branch[i]->brobj.son;

				/* Insert the id of the dup rect into the sub-list & return */
				sladdr = inserta(rtreep->rtsfd, sladdr, id, ins_flg, &new_flg);

				/* Update sub-leaf ptr and write the page back */
				if (new_flg == TRUE)
				{
					n->branch[i]->brobj.son = sladdr;
					rtn_val = NEW;
				}
				else
					rtn_val = OLD;

				/* We should only *have* to find one */
#ifdef PRINT
				fprintf(stderr, "InsertSearch: inserting id [%d] into sub-leaf addr [%d] (Ins Flg [%d].\n",
					id, sladdr, new_flg);
#endif

				break;
			}
		}
	}

	/* Return leaf search/insert value */
	return(rtn_val);
}


/* Insert a data rectangle into an index structure.
** InsertRect provides for splitting the root;
** returns 1 if root was split, 0 if it was not.
** The level argument specifies the number of steps up from the leaf
** level to insert; e.g. a data rectangle goes in at level = 0.
** InsertRect2 does the recursion.
*/
int
InsertRect(rtreep, r, sonpage, level)
RTREE		*rtreep;
register	struct	Rect	*r;
register	int		sonpage;
register	int		level;
{
	register struct	Node *newroot;
	register	int i;
	RT_STATS	*statsp = rtreep->rtstats;
	struct	Node	*nnnn, *xnewnode;
	struct	Branch	*b;
	int		ndims, ndcard, result;

	assert(r && rtreep->root);
	assert(level >= 0 && level <= (rtreep->root)->level);

	/* Init Dimension & Node Cardinality */
	ndims = rtreep->rthead->NumDims;
	ndcard = rtreep->rthead->NodeCard;

	for (i=0; i<ndims; i++)
		assert(r->boundary[i] <= r->boundary[ndims+i]);

#ifdef PRINT
	fprintf(stderr, "InsertRect  level=%d\n", level);
#endif

#if	(RECT_LIMIT)
	if (++rect_count > RECT_LIMIT)
	{
		fprintf(stderr, demo_msg, rect_count);
		return(-1);
	}
#endif

	if (statsp->StatFlag)
	{
		if (statsp->Deleting)
			(statsp->ReInsertCount)++;
		else
			(statsp->InsertCount)++;
	}
	if (!statsp->Deleting)
		(statsp->RectCount)++;

	/* root was split */
	if (InsertRect2(rtreep, r, sonpage, rtreep->root, &xnewnode, level) > 0)
	{
#ifdef PRINT
		fprintf(stderr, "ROOT WAS SPLIT PAGE NUM=%d\n",(rtreep->root)->pageNo);
#endif
		if (statsp->StatFlag)
		{
			if (statsp->Deleting)
				(statsp->DeTouchCount)++;
			else
				(statsp->InTouchCount)++;
		}
		(statsp->NodeCount)++;


		/* Allocate Branch Struct */
		b = newBranch(ndims, 1);

		/* grow a new root, make tree taller */
		newroot = newNode(rtreep, ndcard); 
		(statsp->NonLeafCount)++;
		PutOnePage(rtreep, rtreep->pageNo, rtreep->root);
		newroot->level = (rtreep->root)->level + 1;
		NodeCover(rtreep, rtreep->root, b->rect);
		b->brobj.son = rtreep->pageNo++;
		AddBranch(rtreep, b, newroot, NULL);
		NodeCover(rtreep, xnewnode, b->rect);
		PutOnePage(rtreep, rtreep->pageNo, xnewnode);
		b->brobj.son = (rtreep->pageNo)++;
		AddBranch(rtreep, b, newroot, NULL);
		freeNode(rtreep, rtreep->root);
		rtreep->root = newroot;
		freeNode(rtreep, xnewnode);
		freeBranch(b);
		statsp->EntryCount += 2;
		result = 1;
	}
	else
		result = 0;

	return (result);
}

/* Inserts a new data rectangle into the index structure.
** Recursively descends tree, propagates splits back up.
** Returns 0 if node was not split.  Old node updated.
** If node was split, returns 1 and sets the pointer pointed to by
** new to point to the new node.  Old node updated to become one of two.
** The level argument specifies the number of steps up from the leaf
** level to insert; e.g. a data rectangle goes in at level = 0.
*/
int
InsertRect2(rtreep, r, sonpage, n, new, level)
RTREE		*rtreep;
register struct	Rect *r;
register int	sonpage;
register struct Node *n, **new;
register int	level;
{
	register	int i, j;
	RT_STATS	*statsp = rtreep->rtstats;
	struct	Node	*nnn, *n2;
	struct	Branch	*b;
	int		ndims, result;
	long		sladdr, id;	/* Sub-Leaf Blk Addr, tuple id */
	FLAG		new_flg;	/* Sub-Leaf Blk Split Flag */
	FLAG		ins_flg = NEW;	/* Sub-Leaf Insertion Type Flag */


	assert(r && n && new);
	assert(level >= 0 && level <= n->level);

	/* Init Dimension */
	ndims = rtreep->rthead->NumDims;

	if (statsp->StatFlag)
	{
		if (statsp->Deleting)
			(statsp->DeTouchCount)++;
		else
			(statsp->InTouchCount)++;
	}

	/* Allocate a branch struct */
	b = newBranch(ndims, 1);

	/* Still above level for insertion, go down tree recursively */
	if (n->level > level)
	{
		if (rtreep->Pack)
			i = Pa_PickBranch(rtreep, r, n);
		else
			i = PickBranch(rtreep, r, n);
		
		assert(n->branch[i]->brobj.son > 0);
		nnn = GetOnePage(rtreep, n->branch[i]->brobj.son);

		if (!InsertRect2(rtreep, r, sonpage, nnn, &n2, level))
		{
			/* son was not split */
			CombineRect(n->branch[i]->rect, r, n->branch[i]->rect, ndims);
			PutOnePage(rtreep, nnn->pageNo, nnn);
			freeNode(rtreep, nnn);
			freeBranch(b);
			return (0);
		}

		/* son was split */
		else
		{
#ifdef PRINT
		printf ("SON WAS SPLIT PAGE NUM=%d\n",nnn->pageNo);
#endif
			if (n->level == 0)
				(statsp->LeafCount)++;
			else
				(statsp->NonLeafCount)++;

			NodeCover(rtreep, nnn, n->branch[i]->rect);
			PutOnePage(rtreep, nnn->pageNo, nnn);
			b->brobj.son = rtreep->pageNo;
			NodeCover(rtreep, n2, b->rect);
			PutOnePage(rtreep, (rtreep->pageNo)++, n2);
			result = AddBranch(rtreep, b, n, new);
			freeNode(rtreep, n2);
			freeNode(rtreep, nnn);
			freeBranch(b);
			(statsp->EntryCount)++;
			return (result);
		}
	}

	/* Have reached level for insertion.  Add rect & split, if necessary */
	else if (n->level == level)
	{
		assert(sonpage != 0);

		/* Insert id into sub-leaf list file */
		id = sonpage;
		sladdr = inserta(rtreep->rtsfd, sladdr, id, ins_flg, &new_flg);

		/* Put sub-leaf addr into r-tree */
		copyRect(b->rect, r, ndims);
		b->brobj.son = sladdr;

		/* son field of leaves contains r of data record */
		(statsp->EntryCount)++;
		result = AddBranch(rtreep, b, n, new);
		freeBranch(b);
		return (result);
	}

	else
	{
		/* Not supposed to happen */
		assert (FALSE);
		return (0);
	}
}

/* Delete a data rectangle from an index structure.
** Pass in a pointer to a Rect, the r of the record, ptr to ptr to root node.
** Returns 1 if record not found, 0 if success.
** DeleteRect provides for eliminating the root.
*/
int
DeleteRect(rtreep, r, id)
RTREE			*rtreep;
register struct	Rect	*r;
long			id;
{
	register int			i;
	register struct	Node		*t;
	register struct	Node		**nn = &rtreep->root;
	register struct	ListNode	*e = NULL;

	RT_STATS		*statsp = rtreep->rtstats;
	struct	 ListNode	*reInsertList = NULL;
	int			ndcard;

	assert(r && nn);
	assert(*nn);

	statsp->Deleting = TRUE;

#ifdef PRINT
	printf("DeleteRect\n");
#endif

	if (!DeleteRect2(rtreep, r, *nn, &reInsertList, id))
	{
		/* found and deleted a data item */
		if (statsp->StatFlag)
			statsp->DeleteCount++;
		(statsp->RectCount)--;

		/* reinsert any branches from eliminated nodes */
		while (reInsertList)
		{
			t = reInsertList->node;
			ndcard = (t->level > 0) ? rtreep->rthead->NodeCard :
				rtreep->rthead->TNodeCard;
			for (i = 0; i < ndcard; i++)
			{
				if (t->branch[i]->brobj.son != 0)
				{
					InsertRect(rtreep,
						(t->branch[i]->rect),
						t->branch[i]->brobj.son,
						nn,
						t->level);
					(statsp->EntryCount)--;
				}
			}
			e = reInsertList;
			reInsertList = reInsertList->next;
			freeNode(rtreep, e->node);
			FreeListNode(e);
		}

		/* check for redundant root (not leaf, 1 son) and eliminate */
		if ((*nn)->count == 1 && (*nn)->level > 0)
		{
			if (statsp->StatFlag)
				statsp->ElimCount++;
			(statsp->EntryCount)--;
			for (i = 0; i < ndcard; i++)
			{
				if ((*nn)->branch[i]->brobj.son != 0)
				{
					t = GetOnePage(rtreep, (*nn)->branch[i]->brobj.son);
					break;
				}
			}
			i = (*nn)->pageNo;
			freeNode(rtreep, *nn);
			*nn = t;
			(*nn)->pageNo = i;
		}
		statsp->Deleting = FALSE;
		printf("------> The rectangle is deleted\n");
		return (0);
	}
	else
	{
		statsp->Deleting = FALSE;
		printf("------> The rectangle is not in the index.\n");	
		return (1);
	}
}

/* Delete a rectangle from non-root part of an index structure.
** Called by DeleteRect.  Descends tree recursively,
** merges branches on the way back up.
*/
int
DeleteRect2(rtreep, r, n, ee, id)
RTREE			*rtreep;
register struct Rect	*r;
register struct Node	*n;
register struct ListNode **ee;
long			id;
{
	register int	i, j;
	RT_STATS	*statsp = rtreep->rtstats;
	struct Node	*nnn;
	int		ndcard, ndims;
	long		sladdr;		/* Sub-Leaf Blk Addr */
	int		tplcnt;		/* SUb-Leaf tuple count */


	assert(r && n && ee);
	assert(n->level >= 0);

	ndims = rtreep->rthead->NumDims;

	if (statsp->StatFlag)
		statsp->DeTouchCount++;

	if (n->level > 0) /* not a leaf node */
	{
		ndcard = rtreep->rthead->NodeCard;
		for (i = 0; i < ndcard; i++)
		{
			if ((n->branch[i]->brobj.son > 0)  /* not a leaf node as above */
					 && Overlap(r, n->branch[i]->rect, ndims))
			{
				nnn = GetOnePage(rtreep, n->branch[i]->brobj.son);
				if (!DeleteRect2(rtreep, r, nnn, ee, id))
				{
					if (nnn->count >= rtreep->MinFill)
					{
						NodeCover(rtreep, nnn, n->branch[i]->rect);
						PutOnePage(rtreep, nnn->pageNo, nnn);
						freeNode(rtreep, nnn);
					}
					else
					{
						/* not enough entries in son, eliminate son node */
						ReInsert(nnn, ee);
						DisconBranch(n, i, ndims);
						(statsp->EntryCount)--;

						if (statsp->StatFlag)
							statsp->ElimCount++;
					}
					return (0);
				}
				else
					freeNode(rtreep, nnn);
			}
		}
		return (1);
	}
	else  /* a leaf node */
	{
		ndcard = rtreep->rthead->TNodeCard;
		for (i = 0; i < ndcard; i++)
		{
			if ((n->branch[i]->brobj.son != 0)
				&& (Equal2Rects(n->branch[i]->rect, r, ndims)))
			{
				/* Remove (one or many) id(s) from sub-leaf list file */
				sladdr = n->branch[i]->brobj.son;
				if (id != ALL_IDS)
					tplcnt = deletea(rtreep->rtsfd, sladdr, id, 0);
				else
				{
					do
					{
						id = xgettuple(rtreep->rtsfd, sladdr, 0);
						tplcnt = deletea(rtreep->rtsfd, sladdr, id, 0);
					}	while(tplcnt > 0);
				}

				/* If all id's have been removed from sub-leaf list
				 * remove the rtree branch as well.
				 */
				if (tplcnt == 0)
				{
					DisconBranch(n, i, ndims);
					(statsp->EntryCount)--;
				}
				return (0);
			}
		}
		return (1);
	}
}

/* Add a node to the reinsertion list.  All its branches will later
** be reinserted into the index structure.
*/
ReInsert(n, ee)
register struct Node		*n;
register struct ListNode	**ee;
{
	register struct	ListNode *l;

	l = NewListNode();
	l->node = n;
	l->pageNo = n->pageNo;

#ifdef PRINT
	printf("THE ONLY PLACE I RECORD PAGE NUMS IN LISTS %d\n",l->pageNo);
#endif
	l->next = *ee;
	*ee = l;
}

/* Allocate space for a node in the list used in DeletRect to
** store Nodes that are too empty.
*/
struct ListNode *
NewListNode()
{
	return (struct ListNode *) myalloc(sizeof(struct ListNode));
}

FreeListNode(p)
register struct ListNode *p;
{
	myfree(p);
}


/* Insert many data rectangles into an index structure.
** InsertRect provides for splitting the root;
** returns 1 if root was split, 0 if it was not.
** The level argument specifies the number of steps up from the leaf
** level to insert; e.g. a data rectangle goes in at level = 0.
** InsertRect2 does the recursion.
*/

int
Pa_InsertRect(rtreep, r, sonpage, level, num)
RTREE		*rtreep;
register struct Rect **r;
register int	*sonpage;
register int	level;
register int	num;
{
	register int	i,j;
	register struct Node *newroot;

	RT_STATS	*statsp = rtreep->rtstats;
	struct	Node	*xnewnode, *nnnn;
	struct	Branch	*b;
	int		ndims, ndcard;
	int		result;

#ifdef PRINT
	printf("Pa_InsertRect  level=%d\n", level);
#endif

#if	(RECT_LIMIT)
	rect_count += num;
	if (rect_count > RECT_LIMIT)
	{
		fprintf(stderr, demo_msg, RECT_LIMIT);
		return(-1);
	}
#endif

	/* Init Dimension & Node Cardinality */
	ndims = rtreep->rthead->NumDims;
	ndcard = rtreep->rthead->NodeCard;

	for (j=0; j<num; j++)
	{
		assert(r[j]);
		for (i=0; i<ndims; i++)
			assert(r[j]->boundary[i] <= r[j]->boundary[ndims+i]);
	}
	assert(rtreep->root);
	assert(level >= 0 && level <= (rtreep->root)->level);

	if (statsp->StatFlag)
	{
		if (statsp->Deleting)
			statsp->ReInsertCount++;
		else
			statsp->InsertCount += num;
	}
	if (!statsp->Deleting)
		statsp->RectCount += num;

	/* root was split */
	if (Pa_InsertRect2(rtreep, r, sonpage, rtreep->root, &xnewnode, level, num)>0) 
	{
#ifdef PRINT
		printf ("ROOT WAS SPLIT PAGE NUM=%d\n",(rtreep->root)->pageNo);
#endif
		if (statsp->StatFlag)
		{
			if (statsp->Deleting)
				statsp->DeTouchCount++;
			else
				statsp->InTouchCount++;
		}
		(statsp->NodeCount)++;

		/* Allocate Branch Struct */
		b = newBranch(ndims, 1);

		/* grow a new root, make tree taller */
		newroot = newNode(rtreep, ndcard);
		(statsp->NonLeafCount)++;
		PutOnePage(rtreep, rtreep->pageNo, rtreep->root);
		newroot->level = (rtreep->root)->level + 1;
		NodeCover(rtreep, rtreep->root, b->rect);
		b->brobj.son = (rtreep->pageNo)++;
		AddBranch(rtreep, b, newroot, NULL);
		NodeCover(rtreep, xnewnode, b->rect);
		PutOnePage(rtreep, rtreep->pageNo, xnewnode);
		b->brobj.son =  (rtreep->pageNo)++;
		AddBranch(rtreep, b, newroot, NULL);
		freeNode(rtreep, rtreep->root);
		rtreep->root = newroot;
		freeBranch(b);
		freeNode(rtreep, xnewnode);
		statsp->EntryCount += 2;
		result = 1;
	}
	else
		result = 0;

	return (result);
}

/* Inserts many data rectangles into the index structure.
** Recursively descends tree, propagates splits back up.
** Returns 0 if node was not split.  Old node updated.
** If node was split, returns 1 and sets the pointer pointed to by
** new to point to the new node.  Old node updated to become one of two.
** The level argument specifies the number of steps up from the leaf
** level to insert; e.g. a data rectangle goes in at level = 0.
*/
int
Pa_InsertRect2(rtreep, r, sonpage, n, new, level, num)
RTREE		*rtreep;
register	struct Rect	**r;
register	int		*sonpage;
register	struct Node	*n, **new;
register	int		level;
register	int		num;
{
	register int	i,j;
	RT_STATS	*statsp = rtreep->rtstats;
	struct Node	*nnn, *n2, *GetOnePage();
	struct Branch	*b, *b1;
	int		ndims, ndcard, result;

	assert(n && new);
	assert(level >= 0 && level <= n->level);
	for (j=0;j<num;j++)
		assert(r[j]);

#ifdef PRINT
	printf("Pa_InsertRect2  num=%d  level=%d\n",num, level);
#endif

	ndims = rtreep->rthead->NumDims;
	ndcard = (n->level > 0) ? rtreep->rthead->NodeCard : rtreep->rthead->TNodeCard;

	if (statsp->StatFlag)
	{
		if (statsp->Deleting)
			statsp->DeTouchCount++;
		else
			statsp->InTouchCount++;
	}

	/* Still above level for insertion, go down tree recursively */
	if (n->level > level)
	{
		if (rtreep->Pack)
			i = Pa_PickBranch(rtreep, r[0], n);
		else
			i = PickBranch(rtreep, r[0], n);
		
		assert(n->branch[i]->brobj.son > 0);
		nnn = GetOnePage(rtreep, n->branch[i]->brobj.son);

		b = newBranch(ndims,1);
		if (!Pa_InsertRect2(rtreep, r, sonpage, nnn, &n2, level,num))
		{
			/* son was not split */
			CombineRect(n->branch[i]->rect, r[0], n->branch[i]->rect, ndims);
			PutOnePage(rtreep, nnn->pageNo, nnn);
			freeBranch(b);
			freeNode(rtreep, nnn);
			return (0);
		}
		else		/* son was split */
		{
			if (n->level == 0)
				(statsp->LeafCount)++;
			else
				(statsp->NonLeafCount)++;

			NodeCover(rtreep, nnn, n->branch[i]->rect);
			PutOnePage(rtreep, nnn->pageNo, nnn);
			b->brobj.son = rtreep->pageNo;
			NodeCover(rtreep, n2, b->rect);
			PutOnePage(rtreep, (rtreep->pageNo)++, n2);
			(statsp->EntryCount)++;
			result = AddBranch(rtreep, b, n, new);
			freeNode(rtreep, n2);
			freeBranch(b);
			freeNode(rtreep, nnn);
			return(result);
		}
	}

	/* Have reached level for insertion. Add rect, split if necessary */
	else if (n->level == level)
	{
		b1 = newBranch(ndims, ndcard);
		for (j=0; j<num; j++)
		{
			copyRect((b1+j)->rect, r[j], ndims);
			assert(sonpage[j] != 0);
			(b1+j)->brobj.son = sonpage[j];
		}

		/* son field of leaves contains r of data record */
		statsp->EntryCount += num;
		result = Pa_AddBranch(rtreep, b1, n, new, num);
		freeBranch(b1);
		return(result);
	}

	else
	{
		/* Not supposed to happen */
		assert (FALSE);
		return (0);
	}
}

/* ======================================================================== *
| Print Functions
| _________________________________________________________________________ */

/* PrintIndex(rtreep, n)
||	Print out all nodes in an index
*/
PrintIndex(rtreep, n)
RTREE	*rtreep;
struct Node *n;
{
	struct Node	*nnn;
	int		i, next;
	int		 ndcard = rtreep->rthead->NodeCard;

	assert(n);
	assert(n->level >= 0);

	if (n->level > 0)
	{
		for (i = 0; i < ndcard; i++)
		{
			if ((next = n->branch[i]->brobj.son) >0)
			{
				nnn = GetOnePage(rtreep, next);
				PrintIndex(rtreep, nnn);
				freeNode(rtreep, nnn);
			}
		}
	}

}

/* PrintData(rtreep, n)
||	Print out all the data rectangles in an index.
*/
PrintData(rtreep, n)
RTREE		*rtreep;
struct Node	*n;
{
	struct Node	*nnn;
	int		i, next;
	int		ndcard = rtreep->rthead->NodeCard;

	assert(n);
	assert(n->level >= 0);

	if (n->level == 0)
		PrintNode(rtreep, n);
	else
	{
		PrintNode(rtreep, n);
		for (i = 0; i < ndcard; i++)
		{
			if ((next = n->branch[i]->brobj.son) > 0)
			{
				nnn = GetOnePage(rtreep, next);
				PrintData(rtreep, nnn);
				freeNode(rtreep, nnn);
			}
		}
	}
}
