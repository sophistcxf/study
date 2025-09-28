/*
|| getsltuples.c
||
|| Description:
||	This module contains ADMS sec index access function(s) which
|| are related to and needed by r-tree duplicate key handling.
*/

#include	<stdio.h>

/* ADMS Includes */
#include	"asec.h"

/* R-Tree Includes */
#include	"rtree.h"
#include	"error.h"

extern	int	ck_count_index_accs;

int
GetSLTuples(rtreep, rect, loc)
RTREE		*rtreep;
struct Rect *rect;
long		loc;
{
	register int	i, n_tuples, count = 0;
	long		page;
	unsigned int    offset;
	int		n_keys, n_entries, n_slots, last;

	/* Decompose location into page & offset */
	page = loc / BLOCK_SZ;
	offset = loc - page * BLOCK_SZ;

	/* Fill page buffer w/ appropriate data if not filled already */
	if (xcpg.pg_num != page)
	{
		xcpg.pg_num = page;
		xcpg.pg_ptr = BLOCK_SZ * page;
		if (lseek (rtreep->rtsfd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
			read (rtreep->rtsfd, xcpg.page, BLOCK_SZ) != BLOCK_SZ)
		{
			ABORT("Bad seek/read from into index file.\n");
			return (FALSE);
		}
		++ck_count_index_accs;
	}

	/* Set current tuple ptr */
	xcpg.current = offset / sizeof (int);
	if (offset % sizeof (int))
		xcpg.current = (xcpg.page[xcpg.current] & MASKB);
	else
		xcpg.current = (xcpg.page[xcpg.current] / MASKA);

	/* Get no. of tuple id's, position on first one, calculate last one */
	n_tuples = xcpg.num_tid = xcpg.page[xcpg.current++];
	if ((last = xcpg.current + n_tuples) > BLOCK_ENT-1)
		last = BLOCK_ENT - 1;

#ifdef	PRINT
	fprintf(stderr, "page: %d, offset: %d, current: %d, last %d, # tuples: %d\n",
		page, offset, xcpg.current, last, n_tuples);
#endif

	/* Call tuple processing fct for each one found */
	for (i = xcpg.current; i < last; i++)
	{
		RTFctOut(rtreep, rtreep, rect, xcpg.page[i]);
		count++;
	}

	/* Decrement tuple count by the number written */
	n_tuples -= (last - xcpg.current);

	/* Sequence thru continuation pages if they exist */
	while (xcpg.page[1] != 0)
	{
		xcpg.pg_num = xcpg.page[1];
		xcpg.pg_ptr = xcpg.pg_num * BLOCK_SZ;
		if (lseek (rtreep->rtsfd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
			read (rtreep->rtsfd, xcpg.page, BLOCK_SZ) != BLOCK_SZ)
		{
			ABORT("Bad seek/read from into (continued) index file.\n");
			return (FALSE);
		}
		++ck_count_index_accs;

		/* Set current tuple ptr */
		xcpg.current = HDR_SZ;
	
		/* Get no. of keys, entries, open slots & tuples (synonyms)
		 * in this page.
		 */
		n_keys = xcpg.page[2] / MASKA;
		n_entries = xcpg.page[2] & MASKB;
		n_slots = xcpg.page[4];
		
		/* Calculate position of last tuple:
		 * Note: the number per page varies - ie 2nd *full* page contains more than
		 * 1st *full* page.
		 */
		if (n_tuples > BLOCK_ENT)
			last = BLOCK_ENT;
		else
			last = n_tuples + HDR_SZ;

#ifdef	PRINT
		fprintf(stderr, "\tn_keys: %d, n_tuples: %d, n_entries: %d, n_slots: %d\n",
			n_keys, n_tuples, n_entries, n_slots);
		fprintf(stderr, "\tcurrent: %d, last: %d\n",
			xcpg.current, last);
#endif

		/* Call tuple processing fct for each one found */
		for (i = xcpg.current; i < last; i++)
		{
			RTFctOut(rtreep, rtreep, rect, xcpg.page[i]);
			count++;
		}

		/* Decrement tuple count by the number written */
		n_tuples -= (last - xcpg.current);
	}

	return(count);
}

