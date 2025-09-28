#ifndef	_INCL_CTREE_H

#define CK_FIRST 1
#define CK_NEXT 2

#define CK_NO_SIDE 0
#define CK_LEFT_SIDE 1
#define CK_RIGHT_SIDE 2
#define CK_BOTH_SIDES 3

#define CK_KEY_MORE   1
#define CK_KEY_LAST   2
#define CK_NOMORE     3
#define CK_SKIP       4

/* for marking tids in c-trees */
#define CK_LEFT_MASK 0x80000000
#define ck_mark_tid( atid ) (atid | CK_LEFT_MASK)
#define ck_unmark_tid( atid ) (atid & ~CK_LEFT_MASK)
#define ck_is_marked( atid ) (atid & CK_LEFT_MASK)

struct tidlist
  {
     int tid;
     struct tidlist *next;
  };

typedef struct tidlist TidList;

#define CK_DELIM "~~"
#define CK_DELIM_LEN 2

/******************************************************************
* 1. Macro name : ck_make_afilename(fname, t1, a1, t2, a2)
* 2. Function Description :
*	Compose a C-tree A-file name for table t1, t2, attribs a1 and
*	a2. These should be sorted properly before calling this (see
*	ck_find_index.c)
*	Name is returned in fname, which is assumed large enough.
*
* 3. Parameter : (all char *)
*    fname  : name for A-file (output)
*    t1     : name of base table 1
*    a1     : attrib from t1
*    t2     : name of base table 2
*    a2     : attrib from t2
*********************************************************************/

#define ck_make_afilename(fname, t1, a1, t2, a2) \
 sprintf(fname, "A%s%s%s%s%s%s%s%s%s",CK_DELIM, t1, CK_DELIM, a1, \
 CK_DELIM, t2, CK_DELIM, a2, CK_DELIM)

/* get high order (left) 2 bytes from 32 bit word */
#define ck_get_left_16(i) ( ((unsigned) i) >> 16 )

/* get low order 2 bytes */
#define ck_get_right_16(i) ( i & 0x0FFFF)

/* form a 32 bit word using 'high' as high 2 bytes and 'low' as low 2 bytes */
#define ck_combine_high_low(high, low) \
	  ( ((high << 16) & 0xFFFF0000) | (ck_get_right_16(low)) )

#define	_INCL_CTREE_H
#endif	_INCL_CTREE_H
