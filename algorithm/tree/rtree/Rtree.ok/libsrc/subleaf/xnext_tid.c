#include <stdio.h>

#include "adms.h"
#include "asec.h"

/************************************************************************
 *									*
 * Name		: xnext_tid						*
 * Function	: Fetch next tuple_id on the A-file and update pointer. *
 * Algorithm	: If ( no more tuples in buffer but pointer to the nest *
 *		       block not equal to NULL )			*
 *		     fetch next block and put it in buffer;		*
 *		  Fetch next tuple_id					*
 * Parameters	: fd (file descriptor of the A-file)			*
 * Returns	: tid (tuple_id of the next tuple) or			*
 *		  -1  (if the last one has been already accessed)	*
 * Globals	: xcpg (intermediate storage structure)			*
 * Calls	: None							*
 * Called by	: xjoin, xselect, xunique, xset_oper, deletea, delete   *
 *									*
 ************************************************************************/
/*********************************************************************
* Change log1: 
* Author     : C. Kemper
* Date       : 3/89
* Purpose    : Changes for CTREEs:
*		1. Accept 'side' argument which, if A-file being read
*		   belongs to a C-tree, specifies the left or right
*		   table of the C-tree. Only tids from the specified
*		   table should be returned. A tid from the left table
*		   will be marked by having its most sig bit set. A tid
*		   from the right table is unmarked.
*		   If A-file belongs to a normal 2ndary index, the 'side'
*		   arg has the value CK_NO_SIDE (0).
*		   Also, some functions such as tuple insert and delete
*		   will want a tuple regardless of which side it belongs
*		   to, so these functions will pass 'side' as zero.
*
**********************************************************************/


#include "ctree.h"

long
        xnext_tid (fd, side)
int     fd;
int     side;/* CK 3/89 */
{
	int     i;
	long    tid;
	extern int      ck_count_index_accs;/* in exec_opn */
loop: 
	if (xcpg.counter < xcpg.num_tid) {/* return TID */
		if ((xcpg.current == (BLOCK_ENT - 1) && xcpg.counter == MAXNOTID) ||
				(xcpg.current > (BLOCK_ENT - 1))) {
 /* get next block */
			xcpg.pg_num = xcpg.page[1];
			xcpg.pg_ptr = BLOCK_SZ * xcpg.pg_num;
			lseek (fd, xcpg.pg_ptr, 0);
			read (fd, xcpg.page, BLOCK_SZ);
			xcpg.current = HDR_SZ;
			++ck_count_index_accs;
		}
		xcpg.counter += 1;
		tid = xcpg.page[xcpg.current++];

		switch (side) {
			default: /* Not a C-tree, or don't care */
				return tid;/* so return any tid */
				break;
			case CK_RIGHT_SIDE: 
 /* Return only unmarked tids */
				if (!ck_is_marked (tid))
					return tid;
				break;
			case CK_LEFT_SIDE: /* Return only marked tids */
				if (ck_is_marked (tid))
					return (ck_unmark_tid (tid));
				break;
		}/* end switch(side) */
		goto loop;/* see if next tid is from correct side */
	}
	else
		return (-1);/* no more tuples; return -1 */
}/* end xnext_tid */
