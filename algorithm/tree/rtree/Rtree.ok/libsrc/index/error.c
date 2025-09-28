/*********************************************************************
* 1. Name:	error.c
*
* 2. Description:
*	This module contains "basic" info and error message peocessing
*    functions.
*
* 3. Functions:
*	rt_msg(msg)
*	rt_err(msg)
*	rt_errlong(msg, filename, lineno)
*	rt_abort(msg, filename, lineno)
*
* 4. Parameters:
*	char	*msg;
*	char	*filename;
*	int	lineno;
*
* 5. Author:	skelley
* 6. Date:	4/22/91
**********************************************************************/

#include	<stdio.h>
#include	<sys/errno.h>
#include	"error.h"

extern	int		errno;


int
rt_msg(msg)
char	*msg;
{
	/* Print msg to stdout & return */
	printf("<%s> (R-Tree Message)\n", msg);
	return(1);
}

int
rt_err(msg)
char	*msg;
{
	/* Print msg to stderr, flush it & return */
	fprintf(stderr, "R-Tree Error: <%s>\n", msg);
	fflush(stderr);
	return(1);
}

/* VARARGS
|| Print out an error message w/ a variable no. of str args
|| Note that the first argument *should* contain formats for following ones
*/
int
rt_errlong(format, str1, str2, str3, str4, str5, str6, str7)
char	*format;
char	*str1, *str2, *str3, *str4, *str5, *str6, *str7;
{
	char	lformat[1024];

	/* Print (extended) msg to stderr, flush it  & return */
	sprintf(lformat, "R-Tree Error: <%s>\n", format);
	fprintf(stderr, lformat, str1, str2, str3, str4, str5, str6, str7);
	fflush(stderr);
	return(1);
}

int
rt_abort(msg, filename, lineno)
char	*msg;
char	*filename;
int	lineno;
{
	/* Print msg to stderr, flush em & abort */
	fprintf(stderr, "***YOU'VE DEALT the R-TREE  a MORTAL BLOW***\n");
	fprintf(stderr, " \t(more)\n%s %d: <%s>\n", filename, lineno, msg);
	fflush(stdout);
	fflush(stderr);
	abort();
}
