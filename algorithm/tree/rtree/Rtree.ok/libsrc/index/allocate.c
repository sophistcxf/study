/*
|| allocate.c
||
|| Description:
||	This module contains functions to track memory allocations. They should
|| be called instead of the UNIX versions.
||
|| Functions:
||	char	*myalloc()	- storage allocater
||	int	myfree()	- storage de-allocator
*/

#include <stdio.h>

#include "rtree.h"
#include "assert.h"

/*-----------------------------------------------------------------------------
| Space allocation routines. - just keep track of No. of allocations
|
| Call myalloc() like calloc().
| Call myfree() like free().
-----------------------------------------------------------------------------*/

static	int	puse=0; vuse=0;
static	int	all = 0;
static	int	maxuse=0;

char *
myalloc(n)
register int	 n;
{
	char	*calloc();

	all++;
	puse++;

	if ((puse-vuse) > maxuse)
		maxuse++;

	return calloc(n, sizeof(char));
}

myfree(p)
char	*p;
{
	all--;
	vuse++;

	free(p);
}
