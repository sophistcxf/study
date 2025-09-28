#include <stdio.h>

#include "adms.h"
#include "asec.h"
/************************************************************************
 * Name		: xgettuple						*
 * Function 	: Fetch the block from the A-file that correspons to	*
 * the input address and put it in the global buffer "xcpg.page",	*
 * update buffer header and return the tuple-id.			*
 * Algorithm	: decode address;  read block from A-file and put it 	*
 * into buffer; fetch number of occurrences of secondary key and put it	*
 * in xcpg.num_tid; update pointer to first occurrance; return tuple_id	*
 * of first occurence							*
 * Parameters	: fd	(file descriptor of A-file)			*
 *		: xaddr	(tuple_id in A-file fro the secondary key)	*
 * Return	: tid	(tuple_id in base relation of the first		*
 *		  occurrence of the secondary key)			*
 * globals	: xcpg (intermediate storage structure )		*
 * Calls	: xfetch_num						*
 * Called by	: xjoin, xselect, xunique, xset_oper, deletea, delete   *
 ***********************************************************************/

/*********************************************************************
* Change log1: 
* Author     : C. Kemper
* Date       : 3/89
* Purpose    : Changes for CTREEs:
*		1. Accept 'side' argument which, if A-file being read
*		   belongs to a C-tree, specifies the left or right
*		   table of the C-tree. This is passed on to xnext_tid().
*	        2. Record left & right backlog pointers, which are
*		   stored in A file header.
*
**********************************************************************/


unsigned long
xgettuple (fd, xaddr, side)
int     fd;
unsigned long int       xaddr;
int     side;/* CK 3/89 */
{
	FLAG	hit, hitpage;
	unsigned int    offset,
	                i;
	long    page,
	        xheader[BLOCK_ENT],
	        tid;
	int     l;
	extern int      ck_count_index_accs;/* CK - from exec_opn */

	page = xaddr / BLOCK_SZ;
	offset = xaddr - page * BLOCK_SZ;

	hitpage = hit = FALSE;

	if (xcpg.fd == fd) {
		hit = TRUE;
		if (xcpg.pg_num == page)
			hitpage = TRUE;
	}
	if (hit == FALSE) {
		/* fill buffer */

		if (lseek (fd, 0L, 0) != 0L) {
			perror ("lseek in xgettuple");
			return (0);
		}
		if (read (fd, xheader, BLOCK_SZ) != BLOCK_SZ) {
			perror ("read in xgettuple");
			return (0);
		}
		xcpg.fd = fd;
		xcpg.num_pages = xheader[1];
		xcpg.num_keys = xheader[2];
		xcpg.num_tups = xheader[3];
		xcpg.avail_ptr = xheader[4];
		xcpg.avail_blk = xheader[5];

		/* CK - preserve C-tree backlog pointers */
		xcpg.l_blog_ptr = xheader[CK_LEFT_BLOG_PTR];
		xcpg.r_blog_ptr = xheader[CK_RIGHT_BLOG_PTR];
		ck_count_index_accs += 2;

		if (xcpg.page == (long *) 0) {
			xcpg.page = (long *) malloc (BLOCK_SZ);
		}

		xcpg.pg_ptr = BLOCK_SZ * page;
		xcpg.pg_num = page;
		if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
				read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ)
			return (0);
	}
	else
		if ((hit == TRUE) && (hitpage == FALSE)) {
			xcpg.pg_num = page;
			xcpg.pg_ptr = BLOCK_SZ * page;
			if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
					read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ)
				return (0);
			++ck_count_index_accs;
		}
	xfetch_num (offset);
	tid = xnext_tid (fd, side);/* CK - pass 'side' arg */
	return (tid);
}


xfetch_num (offset)
unsigned int    offset;
{
	xcpg.current = offset / sizeof (int);
	if (offset % sizeof (int))
		xcpg.current = (xcpg.page[xcpg.current] & MASKB);
	else
		xcpg.current = (xcpg.page[xcpg.current] / MASKA);
	xcpg.num_tid = xcpg.page[xcpg.current++];
	xcpg.counter = 0;
}
