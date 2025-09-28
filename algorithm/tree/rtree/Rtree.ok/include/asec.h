#ifndef	_INCL_ASEC_H

#define MASKA 65536
#define MASKB 65535
#define BLOCK_SZ  1024
#define BLOCK_ENT (BLOCK_SZ / 4)
#define MAXNOTID (BLOCK_ENT - 7)
#define HDR_SZ      5

#ifndef OLD
#define OLD   1
#endif
#ifndef NEW
#define NEW   2
#endif

/* C Kemper 4/89. Add two new fields to header of first A-file page,
   for storing backlog pointers for use with C-trees */

#define CK_LEFT_BLOG_PTR  10
#define CK_RIGHT_BLOG_PTR 11

typedef unsigned short FLAG;
struct va_DOLLAR                /* intermediate storage structure buffer */
{       int fd;                 /* file descriptor */
        long num_pages;         /* number of blocks in file */
        long num_keys;           /* number of attribute values in file */
        long num_tups;           /* number of TIDs in file */
        long current;            /* pointer to the current slot in block */
        long avail_blk;          /* header to the available blocks list */
        long *page;              /* current page */
        long pg_num;             /* current page number */
        long pg_ptr;             /* offset to current page */
        long avail_ptr;          /* header of the available space list */
        long num_tid;            /* number of TIDs of the current key */
        long counter;           /* number of TIDs already read of current key*/
	long l_blog_ptr;         /* pointer into left relation's backlog */
	long r_blog_ptr;         /* pointer into right relation's backlog */
};

extern	struct va_DOLLAR xcpg;

#define	_INCL_ASEC_H
#endif
