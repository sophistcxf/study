/*-----------------------------------------------------------------------------
| Definitions and variables used in linear split code.
-----------------------------------------------------------------------------*/

/* variables for finding a partition */
typedef struct PartitionVars
{
	struct	Branch	**BranchBuf;
	struct	Rect	*CoverSplit;
	struct	Rect	*cover[2];
	float		area[2];
	int		NumDims;
	int		NodeCard;
	int		*partition;
	int		*taken;
	int		count[2];
}	PARTVARS;
