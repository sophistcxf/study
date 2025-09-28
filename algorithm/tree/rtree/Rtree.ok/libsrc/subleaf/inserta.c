/*****************************************************************
*
* inserta.c
* ---------
*
*  
* input parameters:
*	fd	- File descriptor of the intermediate file "A$".	
*	tid	- Tuple-id (from base relation) to be inserted.
*	instype	- "OLD" if the attribute value of the tuple to
*		be inserted was already in the system, "NEW" otherwise.
*
* output parameters:
*	pnewaddr- "TRUE" if instype = "NEW" or the tid of the
*		tuple in the intermediate file had to be changed.
*	paddr	- If pnewaddr = "NEW" ,this variable contains the new
*		tid in the intermediate file for the attribute value,
*		otherwise contains the same input address.
*
****************************************************************/

/*********************************************************************
* Change log1: 
* Author     : C. Kemper
* Date       : 4/89
* Purpose    : Changes for CTREEs:
*		For compatability with CTREE changes, pass 3rd arg, with
*		value 0, to xgettuple().
*		Also, keep left & right backlog pointer values in A file
*		header. Also, update count of index file acesses.
*
**********************************************************************/


#include <stdio.h>

#include "adms.h"
#include "asec.h"
#include "ctree.h"
#include "error.h"

extern	int	ck_count_index_accs;

/* 
  Insert Tid in the intermediate storage structure and return
  pnewaddr = "TRUE" and paddr if a new block was created for the
  corresponding attribute value, otherwise return pnewaddr = "FALSE".
 */
inserta (fd, paddr, tid, instype, pnewaddr)
int	fd;
long	tid,paddr;
FLAG	instype, *pnewaddr;

{
	FLAG	hit, hitpage;
	long	insert_old(), insert_new();/* nikos */
	long    page,
	        xheader[BLOCK_ENT],
	        temp,
	        temp_addr;
	int	offset;

	hit = hitpage = FALSE;

	if (xcpg.fd == fd)
		hit = TRUE;

	if (hit == FALSE) {/* fill buffer */
		if (lseek (fd, 0L, 0) != 0L) {
			perror ("inserta: seek to header");
			return (0);
		}
		if (read (fd, xheader, BLOCK_SZ) != BLOCK_SZ) {
			perror ("inserta: read header");
			return (0);
		}
		xcpg.fd = fd;
		xcpg.num_pages = xheader[1];
		xcpg.num_keys = xheader[2];
		xcpg.num_tups = xheader[3];
		xcpg.avail_ptr = xheader[4];
		xcpg.avail_blk = xheader[5];

		/* CK 4/89 - save left & right backlog pointer */
		xcpg.l_blog_ptr = xheader[CK_LEFT_BLOG_PTR];
		xcpg.r_blog_ptr = xheader[CK_RIGHT_BLOG_PTR];
		ck_count_index_accs += 2;

		if (xcpg.page == (long *) 0) {
			xcpg.page = (long *) malloc (BLOCK_SZ);
		}

		xcpg.pg_ptr = BLOCK_SZ;
		xcpg.pg_num = 1;

		if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
				read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
			return (0);
		}
	}

	switch (instype) {
	case NEW: 
		paddr = insert_new (fd, tid);
		*pnewaddr = TRUE;
		write_header (fd);
		return (paddr);
		break;
	case OLD: 
		temp = insert_old (fd, tid, paddr);
		*pnewaddr = (temp == paddr) ? FALSE : TRUE;
		write_header (fd);
		return (temp);
		break;
	default: 
		ABORT("Unexpected insertion type \n");
	}
}


move_down (no_entry, last_tid, num_slot)
int     no_entry,
        last_tid,
        num_slot;
{
	register int	i;
	int     prt;

	prt = no_entry;
	if (last_tid < no_entry) {
		prt = (int) last_tid;
		for (i = 0; last_tid < (no_entry - i); i++)
			xcpg.page[no_entry - i] = xcpg.page[no_entry - i - 1];
		update_tail (1, num_slot);
	}
	return (prt);
}



update_tail (n, num_slot)
int     n,
        num_slot;
{
	register int	i;
	int     slot1,
	        slot2;

	for (i = 0; i < ((num_slot + 1) / 2); i++) {
		if ((slot1 = (int) xcpg.page[BLOCK_ENT - 1 - i] / MASKA) > xcpg.current)
			slot1 += n;
		if ((slot2 = (int) xcpg.page[BLOCK_ENT - 1 - i] & MASKB) > xcpg.current)
			slot2 += n;
		xcpg.page[BLOCK_ENT - 1 - i] = (long) slot1 * MASKA + slot2;
	}
}



int
move_up (no_entry, last_tid, n, num_slot)
int     no_entry,
        n,
        last_tid,
        num_slot;
{
	register int	i;

	for (i = last_tid; i < no_entry; i++)
		xcpg.page[xcpg.current + (i - last_tid)] = xcpg.page[i];
	for (i = xcpg.current + no_entry - last_tid; i < no_entry; i++)
		xcpg.page[i] = (long) 0;
	update_tail (-n, num_slot);
}


reset_new_block (fd)
int     fd;
{
	long    temp_blk;

	if (xcpg.avail_blk == -1) {
		add_block (xcpg.num_pages);
		xcpg.num_pages += 1;
	}
	else {
		xcpg.pg_num = xcpg.avail_blk;
		xcpg.pg_ptr = xcpg.pg_num * BLOCK_SZ;
		if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
				read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
			return (0);
		}
		temp_blk = xcpg.page[1];
		add_block (xcpg.avail_blk);
		xcpg.avail_blk = temp_blk;
		++ck_count_index_accs;
	}
}


long
insert_new (fd, tid)
int	fd;
long	tid;
{
	register int     i;
	FLAG	avail_mark = FALSE;
	int	m,
	        num_key,
	        no_entries;
	long    addr,
	        num_slot,
	        temp_blk;

	/* 
	 access block with enought space to hold the new tid. 
	 */
	while (avail_mark == FALSE && xcpg.avail_ptr != -1) {
		if (xcpg.pg_num != xcpg.avail_ptr) {
			xcpg.pg_ptr = xcpg.avail_ptr * BLOCK_SZ;
			xcpg.pg_num = xcpg.avail_ptr;
			if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
					read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
				return (0);
			}
			++ck_count_index_accs;
		}
		num_key = xcpg.page[2] / MASKA;
		no_entries = xcpg.page[2] & MASKB;
		num_slot = xcpg.page[4];
		if ((no_entries + (num_slot / 2) + 2) >= BLOCK_ENT ||
				(xcpg.page[2] == 0)) {
			xcpg.avail_ptr = xcpg.page[3];
			xcpg.page[3] = (long) 0;
			if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
					write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
				return (0);
			}
			++ck_count_index_accs;
		}
		else
			avail_mark = TRUE;
	}

	if (xcpg.avail_ptr == -1) {/* create a new block */
		if (xcpg.avail_blk == -1) {
			add_block (xcpg.num_pages);
			num_key = 0;
			no_entries = HDR_SZ;
			num_slot = 0;
			xcpg.num_pages += 1;
		}
		else {
			xcpg.pg_num = xcpg.avail_blk;
			xcpg.pg_ptr = xcpg.pg_num * BLOCK_SZ;
			if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
					read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
				return (0);
			}
			temp_blk = xcpg.page[1];
			add_block (xcpg.avail_blk);
			num_key = 0;
			no_entries = HDR_SZ;
			num_slot = 0;
			xcpg.avail_blk = temp_blk;
			++ck_count_index_accs;
		}
	}
	/* Update pointer of the new tuple. */
	m = (BLOCK_ENT - 1) - (num_slot / 2);
	if (num_slot % 2) {
		xcpg.page[m] += (unsigned long) MASKA * no_entries;
		addr = xcpg.page[0] * BLOCK_SZ + (m * sizeof (int));
	}
	else {
		xcpg.page[m] = (unsigned long) no_entries;
		addr = xcpg.page[0] * BLOCK_SZ + (m * sizeof (int)) + 2;
	}

	/* Enter new tid. */
	xcpg.page[no_entries++] = 1;
	xcpg.page[no_entries++] = tid;
	xcpg.num_keys += 1;
	xcpg.num_tups += 1;
	xcpg.page[2] = (unsigned long) MASKA * (num_key + 1) + no_entries;
	xcpg.page[4] += 1;
	num_slot += 1;

	/* Update available space. */
	if ((no_entries + (num_slot / 2) + 2) >= BLOCK_ENT) {
		xcpg.avail_ptr = xcpg.page[3];
		xcpg.page[3] = (long) 0;
	}
	if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
			write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
		return (0);
	}
	++ck_count_index_accs;
	return (addr);
}



long
insert_old (fd, tid, loc)
int	fd;
long	tid,
	loc;
{
	FLAG mark, first, moved;
	long    temp_tid,
	        temp_page[BLOCK_ENT],
	        block[BLOCK_ENT],
	        last_tid,
	        pre_page,
	        addr;
	int     num_key,
	        no_entry,
	        i,
	        num_slot,
	        ptr,
	        n;

	temp_tid = xgettuple (fd, loc, 0);/* CK 4/89 */
	mark = TRUE;
	moved = FALSE;
	first = TRUE;
	xcpg.current -= 2;
	addr = loc;

	/* Update no entries and file header */
	xcpg.page[xcpg.current] += 1;
	xcpg.num_tups += 1;
	xcpg.num_tid += 1;
	pre_page = xcpg.pg_ptr;

	/* Fetch last block in chain if any. */
	while (xcpg.page[1] != 0) {
		pre_page = xcpg.pg_ptr;
		xcpg.pg_num = xcpg.page[1];
		xcpg.pg_ptr = xcpg.pg_num * BLOCK_SZ;
		if (first) {
			if (lseek (fd, pre_page, 0) != pre_page ||
					write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
				return (0);
			}
			first = FALSE;
			++ck_count_index_accs;
		}
		if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
				read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
			return (0);
		}
		++ck_count_index_accs;
		xcpg.current = HDR_SZ;
	}

	/* Unpack 3^ field of current block header */
	num_key = xcpg.page[2] / MASKA;
	no_entry = xcpg.page[2] & MASKB;
	num_slot = xcpg.page[4];

	/* Check for and handle block overflow */
	if ((no_entry) + ((num_slot + 1) / 2) >= BLOCK_ENT)
	{
		/* Split an already full synonym page */
		if (num_key == 0) {
			if (xcpg.avail_blk != -1)
				xcpg.page[1] = xcpg.avail_blk;
			else
				xcpg.page[1] = xcpg.num_pages;

			if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
					write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
				return (0);
			}
			++ck_count_index_accs;
			reset_new_block (fd);
			xcpg.current = HDR_SZ;
			ptr = HDR_SZ;
		}
		/* Split a "mixed" tuple page */
		else
		{
			last_tid = xcpg.current + xcpg.num_tid;
			if (xcpg.num_tid > MAXNOTID) {
				last_tid = ((xcpg.num_tid + 1) % (MAXNOTID + 2)) + HDR_SZ;
				mark = FALSE;
			}
			if (last_tid - xcpg.current <= MAXNOTID) {
				for (i = xcpg.current; i < last_tid; i++)
					temp_page[i - xcpg.current] = xcpg.page[i];
				moved = TRUE;
				n = last_tid - xcpg.current;
				move_up (no_entry, last_tid, n, num_slot);
			}
			if (mark)
				xcpg.page[2] = (long) (num_key - 1) * MASKA;
			else
				xcpg.page[2] = (long) num_key * MASKA;
			xcpg.page[2] += (no_entry - n);
			if (xcpg.page[3] == 0) {
				xcpg.page[3] = xcpg.avail_ptr;
				xcpg.avail_ptr = xcpg.pg_num;
			}
			if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
					write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
				return (0);
			}
			++ck_count_index_accs;
			if (pre_page != xcpg.pg_ptr) {
				if (lseek (fd, pre_page, 0) != pre_page ||
						read (fd, block, BLOCK_SZ) != BLOCK_SZ) {
					return (0);
				}
				if (xcpg.avail_blk != -1)
					block[1] = xcpg.avail_blk;
				else
					block[1] = xcpg.num_pages;
				if (lseek (fd, pre_page, 0) != pre_page ||
						write (fd, block, BLOCK_SZ) != BLOCK_SZ) {
					return (0);
				}
				++ck_count_index_accs;
				n = last_tid - HDR_SZ;
			}
			else
				if (xcpg.num_tid > MAXNOTID) {
					if (xcpg.avail_blk != -1)
						xcpg.page[1] = xcpg.avail_blk;
					else
						xcpg.page[1] = xcpg.num_pages;
					if (lseek (fd, pre_page, 0) != pre_page ||
							write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
						return (0);
					}
					++ck_count_index_accs;
				}
			reset_new_block (fd);
			ptr = HDR_SZ + n;
			for (i = HDR_SZ; i < ptr; i++)
				xcpg.page[i] = temp_page[i - HDR_SZ];
			if (mark) {
				xcpg.page[2] = (long) MASKA + ptr;
				xcpg.page[4] = (long) 1;
				xcpg.page[BLOCK_ENT - 1] = (long) HDR_SZ;
				addr = xcpg.pg_num * BLOCK_SZ + ((BLOCK_ENT - 1) *
						sizeof (int)) + 2;
			}
			else
				xcpg.page[2] = (long) ptr;
		}
	}
	else
	{
		if (xcpg.num_tid > MAXNOTID)
			last_tid = ((xcpg.num_tid + 1) % (MAXNOTID + 2)) + HDR_SZ;
		else
			last_tid = xcpg.current + xcpg.num_tid;
		ptr = move_down (no_entry, last_tid, num_slot);
	}
	xcpg.page[ptr] = tid;
	xcpg.page[2] += 1;
	if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
			write (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ) {
		return (0);
	}
	/* Write header of the file. */
	++ck_count_index_accs;
	return (addr);
}



add_block (blk_no)
long    blk_no;
{
	register int     i;

	xcpg.page[0] = blk_no;
	xcpg.page[1] = 0;
	xcpg.page[2] = HDR_SZ;
	if (xcpg.page[3] == 0) {
		xcpg.page[3] = xcpg.avail_ptr;
		xcpg.avail_ptr = blk_no;
	}
	for (i = 4; i < BLOCK_ENT; i++)
		xcpg.page[i] = 0;
	xcpg.pg_num = blk_no;
	xcpg.pg_ptr = xcpg.pg_num * BLOCK_SZ;
}


write_header (fd)
int     fd;
{
	register int     i;
	long    xheader[BLOCK_ENT];

	for (i = 0; i < BLOCK_ENT; i++)
		xheader[i] = (long) 0;
	xheader[1] = xcpg.num_pages;
	xheader[2] = xcpg.num_keys;
	xheader[3] = xcpg.num_tups;
	xheader[4] = xcpg.avail_ptr;
	xheader[5] = xcpg.avail_blk;

	/* CK 4/89 - save backlog pointers */
	xheader[CK_LEFT_BLOG_PTR] = xcpg.l_blog_ptr;
	xheader[CK_RIGHT_BLOG_PTR] = xcpg.r_blog_ptr;
	++ck_count_index_accs;

	if (lseek (fd, 0L, 0) != 0L ||
			write (fd, xheader, BLOCK_SZ) != BLOCK_SZ) {
		return (1);
	}
	return (0);
}
