/* rtree.h
||
||	This header file contains the core structure and constant
|| definitions for the ADMS/R-Tree library.
*/

/*	Include the generic macros/constants header file */
#include	"macros.h"

/*
 * The following definitions of constants may be altered to change
 * characteristics of the r-trees.
 * All R-Tree code must be re-compiled if any one  of these is changed.
 */

	/* Un-coment this constant to turn on debug printing */
/* #define PRINT 1 */

	/* Disk Page Size & R-Tree Max Depth/Dimension Constants */
#define	PAGESIZE	1024
#define	NUMBUFFS	10
#define	MAXLEVELS	5
#define	MAXDIMS		4
#define	MAXSIDES	(MAXDIMS * 2)
#define	COORD_TYPE	float
#define	COORD_SIZE	sizeof(COORD_TYPE)
#define	COUNT_TYPE	short
#define	COUNT_SIZE	sizeof(COUNT_TYPE)
#define	LEVEL_TYPE	short
#define	LEVEL_SIZE	sizeof(LEVEL_TYPE)
#define	POINTER_SIZE	sizeof(int)
#define	HEAD_SIZE	(COUNT_SIZE + LEVEL_SIZE)

	/* Terminal Node Type Constants */
#define	RECT_TYPE	1
#define	POINT_TYPE	2

	/* Operation Flag - for Initialization */
#define	LOAD	1
#define	INSERT	2
#define	SEARCH	3
#define	DELETE	4

	/* Delete All Ids Flag */
#define	ALL_IDS	-1


/*-----------------------------------------------------------------------------
| Index definitions.
-----------------------------------------------------------------------------*/
struct Rect
{
	COORD_TYPE	*boundary;	/* Ptr to array of coordinate pairs */
};

typedef	union	branch_object
{
	int	son;			/* Son page ptr */
	int	ival;			/* Integer value (in leaf) */
	long	lval;			/* Long value (in leaf) */
	float	fval;			/* Float value (in leaf) */
	char	*cpval;			/* Char string ptr value (in leaf) */
}	BR_OBJ;

struct Branch
{
	struct	Rect	*rect;		/* Ptr to a rectangle */
	BR_OBJ		brobj;		/* Branch object type */
};

struct Node
{
	int		pageNo;		/* the page number for that node */
	short		level;		/* 0 is leaf, others positive */
	short		count;		/* number of entries */
	struct Branch	**branch;
};

struct ListNode
{
	struct ListNode	*next;
	struct Node	*node;
	int		pageNo;
};


/*-----------------------------------------------------------------------------
| R-Tree Stat Structure definitions
-----------------------------------------------------------------------------*/
typedef struct rtree_stats
{
	/* Variables for statistics */
	int	StatFlag; /* tells if we are counting or not */
	int	Deleting;


	/* Counters used w/out stat flag */
	int	RectCount;
	int	NodeCount;
	int	LeafCount, NonLeafCount;
	int	EntryCount;
	int	SearchCount;
	int	HitCount;
	int	PagesRead;
	int	PagesWritten;
	int	CacheHits;

	/* Counters affected only when StatFlag set */
	int	InsertCount;
	int	DeleteCount;
	int	ReInsertCount;
	int	InSplitCount;
	int	DeSplitCount;
	int	ElimCount;
	int	EvalCount;
	int	InTouchCount;
	int	DeTouchCount;
	int	SeTouchCount;
	int	CallCount;
	float	SplitMeritSum;
}	RT_STATS;


/*-----------------------------------------------------------------------------
| The R-Tree structure definitions
-----------------------------------------------------------------------------*/
/* Define constants for index file names */
#define	MAXNAMLEN	40
#define	IXSUFFIX	".idx"
#define	SLSUFFIX	".sll"
#define FILE_PATH	""

	/* Include this *for now* */
#include	"split_l.h"

typedef	char	APPL_TYPE;

	/* R-Tree Header Structure - defines node/rect dimensions */
typedef	struct	rtree_head
{
	int	NumDims;	/* No. of dimensions in r-tree */
	int	NumSides;	/* No. of sides (2 * NumDims) */

	/* (int)((PAGESIZE-LEVEL_SIZE-COUNT_SIZE-HEAD_SIZE)/BRANCHSIZE) */
	int	NodeCard;	/* Non-Terminal Node Cardinality */
	int	TNodeCard;	/* Terminal Node Cardinality */
	int	TNodeType;	/* Terminal Node Type */
}	RT_HEAD;

	/* R-Tree Page Buffer Pool Struct */
typedef struct	rtree_bpool
{
	int	pgno;		/* Page in rtree corresponding to buffer */
	int	usecnt;		/* Frequency count for this page */
	char	buf[PAGESIZE];	/* Node page buffer */
}	PAGE_BUF;

	/* R-Tree Main Structure */
typedef	struct rtree_info
{
	int	rtifd;			/* Index tree file descriptor */
	int	rtsfd;			/* Sub-list (data) file descriptor */
	int	opnmode;		/* Index open mode */
	char	rtfnam[MAXNAMLEN];	/* Base rtree file name */

	RT_HEAD	*rthead;		/* R-Tree Header Structure */

	/* Application/User Specific Function Ptrs & Variables */
	APPL_TYPE	*appl_var;		/* Application variable (ie struct *) */
	FILE		*rtoutfp;		/* Output file pointer */
	int		(* rtoutfct) ();	/* User defined output function */
	int		(* rtinfct) ();		/* User defined input function */
	int		(* rtrectcmp) ();	/* Fct Ptr for intermediate nodes */
	int		(* rtrectcmpT) ();	/* Fct Ptr for terminal nodes */

	/* R-Tree Index Specific Parameter */
	struct Node	*SearchBuf[MAXLEVELS];
	struct Node	*root;			/* index root node pointer */
	struct Rect	*CoverAll;		/* rectangle covering all data read */
	struct Rect	*MaxArea;		/* the whole permited area */
	int		pageNo;
	int		MinFill;
	int		Pack;

	/* Buffer Pool Parameters */
	PAGE_BUF	**Page_Pool;	/* Ptr to array of buffer pool structs */
	int		NumPageBufs;	/* Total no. of buffers in the pool */


	PARTVARS	*rtpartvars;	/* R-Tree Split Partition Structure */

	RT_STATS	*rtstats;	/* R-Tree Statistics Accumulation Structure */
}	RTREE;

	/* Macros to simplify indirect function reference */
#define	RTFctIn(rp, fp, r, i)	(*(rp->rtinfct)) (fp, r, i)
#define	RTFctOut(rp, op, r, v)	(*(rp->rtoutfct)) (op, r, v)
#define	RTFctRCmp(rp, r, s, i)	(*(rp->rtrectcmp)) (r, s, i)
#define	RTFctRCmpT(rp, r, s, i)	(*(rp->rtrectcmpT)) (r, s, i)

	/* Application specific variable usage macro(s) */
#define	APPL_VAR(rtreep)	(rtreep->appl_var)
