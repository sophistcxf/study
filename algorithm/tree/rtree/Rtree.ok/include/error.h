/* error.h
||
|| Description:
||	This include file contains definitions & macros for rtree error
|| processing.
*/

	/* Macros to "hide" actual error fct names */
#define	MESSAGE(p)		rt_msg(p)
#define	ERROR(p)		rt_err(p)
#define	ERROR1(f, s1)		rt_errlong(f, s1)
#define	ERROR2(f, s1, s2)	rt_errlong(f, s1, s2)
#define	ERROR3(f, s1, s2, s3)	rt_errlong(f, s1, s2, s3)
#define	ABORT(p)		rt_abort(p, __FILE__, __LINE__)

