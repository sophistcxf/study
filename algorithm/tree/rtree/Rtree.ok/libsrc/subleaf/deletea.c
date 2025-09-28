#include <stdio.h>

#include "adms.h"
#include "asec.h"
#include "ctree.h"

/************************************************************************
 *									*
 * Name		: deletea 						*
 * Function	: delete a tuple from A-file				*
 * Algorithm	: xgettuple 						*
 *		  update block's and buffer's header			*
 *		  if current block does not contain the tuple_id	*
 *			write it back;					*
 *		  fetch block that contains tuple_id			*
 *		  if (last tuple_id)					*
 *			delete tuple_id and counter			*
 *		  else							*
 *		    if (only one block hold all tuple)			*
 *			delete tuple_id					*
 *		    else						*
 *		      if (block is the last in the list)		*
 *			   delete tuple_id				*
 *			   if (tuple_id is the last in the block)	*
 *			        fetch previous block and make pointer   *
 *				     to next block equal NULL		*
 *			   fi						*
 *		      else						*
 *			   delete tuple_id				*
 *			   if (tuple_id is the last in the block)	*
 *				fetch previous block and update pointer *
 *				     to the next block.			*
 *			   fi						*
 *		      fi						*
 *		   fi							*
 *		  write back file header				*
 * Parameters	: fd (file descriptor of the A-file)			*
 *		  addr (tuple-id in the A-file to be removed)		*
 *		  tid  (tuple-id in the base relation)			*
 * Returns	: count (number of tuples remaining for the seconday    *
 *		  key)							*
 * Globals	: xcpg (intermediate storage structure)			*
 * Calls	: xgettuple, xnext_tid, xmove_up, xupdate_tail          *
 * Called by    : wdeleteb						*
 *									*
 ************************************************************************/
/*********************************************************************
* Change log1: 
* Author     : C. Kemper
* Date       : 4/89
* Purpose    : Changes for CTREEs:
*	Accept 'side' argument, which identifies from which side of the c-tree
*	the tid is to be removed. If a 2ndary index is being updated, this
*	arg will be zero.
*         - In calls to xgettuple() and xnext_tid(), a third
            arg with value zero is used, which returns next tid from A-file
            regardless of which side of C-tree it belongs to (i.e. returns
	    both marked & unmarked tids -- marked tids are returned marked.
	    This is necessary since the code below expects each page of A-file
            from addr to be read until tid is found. If the side of the C-tree
            is the left side, then the tid being searched for is marked so it
            will match only marked (left) tids.
  
*	Also, keep left & right backlog pointer values in A file
*	header. Also, update count of index file acesses.
*
**********************************************************************/

deletea (fd, addr, tid, side)
int     fd;
long    addr,
        tid;
int     side;/* CK 4/89 */

{
	extern	int      ck_count_index_accs;
	long    xheader[BLOCK_ENT],
	        temp,
	        pre_page,
	        block[BLOCK_ENT],
	        current_page,
	        temp_ptr,
	        count,
	        page;
	int     num_slot,
	        num_key,
	        no_entry,
	        block_tids,
	        i,
	        cnt = 1;
	FLAG first;

	/* get tuple from intermediate file. */
	temp = xgettuple (fd, addr, 0);

	if (side == CK_LEFT_SIDE)
		tid = ck_mark_tid (tid);

	/* Update block and file headers. */
	xcpg.num_tups -= 1;

	/* decrement count of tids with this key */
	xcpg.page[xcpg.current - 2] -= 1;
	for (i = 0; i < BLOCK_ENT; i++)
		block[i] = xcpg.page[i];

	first = TRUE;
	page = xcpg.pg_ptr / BLOCK_SZ;
	pre_page = xcpg.pg_ptr;
	current_page = xcpg.pg_ptr;

	while (temp != tid && temp != -1)
	 {
		temp = xnext_tid (fd, 0);
		if (xcpg.pg_ptr != current_page)
		 {
			pre_page = current_page;
			current_page = xcpg.pg_ptr;
			cnt++;
			if (first)
			 {
				if (lseek (fd, pre_page, 0) != pre_page ||
						write (fd, block, BLOCK_SZ) != BLOCK_SZ) {
					return (0);
				}
				first = FALSE;
				++ck_count_index_accs;
			}
		}
	}
	if (temp == -1)
	{
		fprintf(stderr, "Tuple-id : %ld does not exixt in the system.\n", tid);
		exit(1);
	}
	xcpg.num_tid -= 1;
	count = xcpg.num_tid;

	num_key = (int) xcpg.page[2] / MASKA;
	no_entry = (int) xcpg.page[2] & MASKB;
	num_slot = xcpg.page[4];
	if (xcpg.num_tid == 0)
	{
		xcpg.num_keys -= 1;
		num_key -= 1;

		xmove_up (no_entry, xcpg.current - 2, 2, num_slot, num_key);
	}
	else
		if (xcpg.num_tid < MAXNOTID)
		{
			xcpg.page[xcpg.current - 1] = 0;
			xmove_up (no_entry, xcpg.current - 1, 1, num_slot, num_key);
		}
		else
			if (xcpg.page[1] == 0)
			{
				xcpg.page[xcpg.current - 1] = 0;
				xmove_up (no_entry, xcpg.current - 1, 1, num_slot, num_key);
				if (xcpg.current == (HDR_SZ + 1) &&
						xcpg.counter == (xcpg.num_tid + 1)) {
					if (lseek (fd, pre_page, 0) != pre_page ||
							read (fd, block, BLOCK_SZ) != BLOCK_SZ) {
						return (0);
					}

					if (lseek (fd, pre_page, 0) != pre_page)
						return (0);
					block[1] = (long) 0;
					if (write (fd, block, BLOCK_SZ) != BLOCK_SZ)
						return (0);
					ck_count_index_accs += 2;
				}
			}
			else {
				pre_page = xcpg.pg_ptr;
				temp_ptr = xcpg.page[1] * BLOCK_SZ;
				if (lseek (fd, temp_ptr, 0) != temp_ptr ||
						read (fd, block, BLOCK_SZ) != BLOCK_SZ) {
					return (0);
				}

				++ck_count_index_accs;
				while (block[1] != 0)
				{
					pre_page = temp_ptr;
					temp_ptr = block[1] * BLOCK_SZ;
					cnt++;
					if (lseek (fd, temp_ptr, 0) != temp_ptr ||
							read (fd, block, BLOCK_SZ) != BLOCK_SZ) {
						return (0);
					}

					++ck_count_index_accs;
				}
				xcpg.counter = (((xcpg.num_tid + 1) - MAXNOTID) %
						(MAXNOTID + 2));
				if (xcpg.counter == 0)
					xcpg.counter = MAXNOTID + 2;
				xcpg.page[xcpg.current - 1] =
					block[HDR_SZ + xcpg.counter - 1];
				if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
						write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
					return (0);
				}
				++ck_count_index_accs;
				xcpg.pg_num = 0;
				for (i = 0; i < BLOCK_ENT; i++)
					xcpg.page[i] = block[i];
				xcpg.pg_ptr = temp_ptr;
				xcpg.pg_num = temp_ptr / BLOCK_SZ;
				xcpg.current = HDR_SZ + xcpg.counter;
				num_key = xcpg.page[2] / MASKA;
				no_entry = xcpg.page[2] & MASKB;
				num_slot = xcpg.page[4];
				xcpg.page[xcpg.current - 1] = 0;
				xmove_up (no_entry, xcpg.current - 1, 1, num_slot, num_key);
				if (xcpg.current - 1 == HDR_SZ) {
					if (lseek (fd, pre_page, 0) != pre_page ||
							read (fd, block, BLOCK_SZ) != BLOCK_SZ) {
						return (0);
					}

					block[1] = (long) 0;
					if (lseek (fd, pre_page, 0) != pre_page ||
							write (fd, block, BLOCK_SZ) != BLOCK_SZ) {
						return (0);
					}
					ck_count_index_accs += 2;
				}
			}
	if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
			write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
		return (0);
	}
	++ck_count_index_accs;
	xcpg.pg_num = 0;

	/* Write header of the file. */
	for (i = 0; i < BLOCK_ENT; i++)
		xheader[i] = (long) 0;
	xheader[1] = xcpg.num_pages;
	xheader[2] = xcpg.num_keys;
	xheader[3] = xcpg.num_tups;
	xheader[4] = xcpg.avail_ptr;
	xheader[5] = xcpg.avail_blk;

	/* CK 4/89 - keep C-tree backlog pointers */
	xheader[CK_LEFT_BLOG_PTR] = xcpg.l_blog_ptr;
	xheader[CK_RIGHT_BLOG_PTR] = xcpg.r_blog_ptr;
	++ck_count_index_accs;

	if (lseek (fd, 0L, 0) != 0L ||
			write (fd, xheader, BLOCK_SZ) != BLOCK_SZ)
	{
		return (0);
	}
	return (count);
}



xmove_up (no_entry, ptr, n, num_slot, num_key)
int     ptr,
        n,
        no_entry,
        num_slot,
        num_key;

{
	int     i;
	for (i = ptr; i < no_entry - 1; i++) {
		xcpg.page[i] = xcpg.page[i + n];
	}
	xupdate_tail (-n, num_slot);
	no_entry -= n;
	if (no_entry == HDR_SZ) {
		xcpg.page[1] = xcpg.avail_blk;
		xcpg.page[2] = (long) 0;
		xcpg.avail_blk = xcpg.pg_num;
	}
	else {
		xcpg.page[2] = (long) num_key * MASKA + no_entry;
		if (xcpg.page[3] == 0) {
			xcpg.page[3] = xcpg.avail_ptr;
			xcpg.avail_ptr = xcpg.pg_num;
		}
	}
}




xupdate_tail (n, num_slot)
int     n,
        num_slot;

{
	int     slot1,
	        slot2,
	        i;
	for (i = 0; i < ((num_slot + 1) / 2); i++) {
		if ((slot1 = (int) xcpg.page[BLOCK_ENT - 1 - i] / MASKA) >= xcpg.current)
			slot1 += n;
		if ((slot2 = (int) xcpg.page[BLOCK_ENT - 1 - i] & MASKB) >= xcpg.current)
			slot2 += n;
		xcpg.page[BLOCK_ENT - 1 - i] = (long) slot1 * MASKA + slot2;
	}
}
