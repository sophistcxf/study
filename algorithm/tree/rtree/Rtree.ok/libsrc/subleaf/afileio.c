/* afileio.c
||
|| Description:
||	This module contains primitive i/o functions for the ADMS sec-index
|| intermediate file ('a' file).
||
|| Functions:
||	int	opena(fname)
||	int	closea()
*/

#include	<stdio.h>
#include	<ctype.h>
#include	<sys/file.h>

#include	"adms.h"
#include	"asec.h"


/* ADMS Global Specials */
int			fr_xcpg = 0;
int			ck_count_index_accs = 0;
struct va_DOLLAR	xcpg;

/*	opena()
||
||	This function opens and initializes  ADMS sub-list file for reading & writing.
|| As in UNIX it will create the file if it does not exist and the appropriate
|| mode bit is set.
*/
int
opena(fname, mode)
char	*fname;
int	mode;
{
	register int	i;
	long	hdr_blk[BLOCK_ENT], frst_blk[BLOCK_ENT];
	int	fd = 0;

	/* Open the index file according to the mode */
	if (mode & O_CREAT)
		fd = open(fname, mode, 0664);
	else
		fd = open(fname, mode);

	if (fd < 0)
	{
		fprintf(stderr, "Unable to open/create the sub-list file '%s'.\n", fname);
		return(0);
	}

	/* Initialize a newly crated sub-list file */
	if (mode & O_CREAT)
	{
		/* Initialize slist header & first blks */
		for (i = 1; i < BLOCK_ENT; i++)
			hdr_blk[i] = frst_blk[i] = 0;

		hdr_blk[1]  = 1;	/* Num of Pages */
		hdr_blk[2]  = 0;	/* Num of Keys */
		hdr_blk[3]  = 0;	/* Num of Tuples */
		hdr_blk[4]  = 1;	/* Internal blk avail ptr */
		hdr_blk[5]  = -1;	/* Avail blk hdr ptr */

		frst_blk[0] =  1;	/* Block no. */
		frst_blk[1] =  0;	/* Continuation blk ptr */
		frst_blk[2] =  0;	/* Num of Keys */
		frst_blk[3] =  -1;	/* Avail Link */
		frst_blk[BLOCK_ENT-1] = HDR_SZ;

		/* Write out the header block */
		write(fd, hdr_blk, BLOCK_SZ);
		write(fd, frst_blk, BLOCK_SZ);

		/* Init index access count */
		ck_count_index_accs = 1;
	}
	/* Open the sub-list file and init ADMS globals */
	else
	{
		if (lseek (fd, 0L, 0) != 0L)
		{
			perror ("inserta: seek to header");
			close(fd);
			return (0);
		}
		if (read (fd, hdr_blk, BLOCK_SZ) != BLOCK_SZ)
		{
			perror ("inserta: read header");
			close(fd);
			return (0);
		}
		xcpg.fd = fd;
		xcpg.num_pages = hdr_blk[1];
		xcpg.num_keys = hdr_blk[2];
		xcpg.num_tups = hdr_blk[3];
		xcpg.avail_ptr = hdr_blk[4];
		xcpg.avail_blk = hdr_blk[5];

		/* CK 4/89 - save left & right backlog pointer */
		xcpg.l_blog_ptr = hdr_blk[CK_LEFT_BLOG_PTR];
		xcpg.r_blog_ptr = hdr_blk[CK_RIGHT_BLOG_PTR];
		ck_count_index_accs += 2;

		if (xcpg.page == (long *) 0)
			xcpg.page = (long *) malloc (BLOCK_SZ);

		xcpg.pg_ptr = BLOCK_SZ;
		xcpg.pg_num = 1;

		if (lseek (fd, xcpg.pg_ptr, 0) != xcpg.pg_ptr ||
			read (fd, xcpg.page, BLOCK_SZ) != BLOCK_SZ)
		{
			perror("inserta: seek or read of 1st page");
			return (0);
		}
	}

	/* Return the file descriptor */
	return(fd);
}


/* closea()
||
|| Some schlepp wrote this undocumented aberration and stuck it in it's own file
*/
closea (list)
int     list;
{
	close(list);
	xcpg.fd = 0;

	if (fr_xcpg && (xcpg.page != (long *) 0))
		free ((char *) xcpg.page);

	xcpg.page = (long *) 0;
}
