/* rtutils.c
||
|| Description:
||	This modoule contains utility functions for the base r-tree
|| library.
||
|| Functions:
||	Initialize(rtreep)
||	GenericInput(fp, coords, ncoords)
||	GenericOutput(fp, coords, ncoords)
*/

#include	"stdio.h"
#include	"rtree.h"
#include	"error.h"

int	GenericInput();
int	GenericOutput();


Initialize(rtreep, op, fp)
RTREE	*rtreep;
int	op;
FILE	*fp;
{
	RT_STATS        *statsp  = rtreep->rtstats;

	switch(op) {
	case DELETE:
	case SEARCH:

		/* Put output function and file ptr in R-Tree struct */
		rtreep->rtoutfp = fp;
		break;

	case INSERT:
	
		/* Disable packing */
		rtreep->MinFill = (int) rtreep->rthead->NodeCard * 0.4;
		rtreep->Pack = 0;
		break;

	case LOAD:

		/* Enable  packing (to 83%) */
		rtreep->MinFill = (int) 5 * (rtreep->rthead->NodeCard/6);
		rtreep->Pack = 1;
		break;

	default:
		break;
	}


	/* Assign Data Specific Input & Output Functions */
	rtreep->rtinfct = GenericInput;
	rtreep->rtoutfct = GenericOutput;

	/* Turn on Stats */
	statsp->StatFlag = 1;
	return;
}

int
GenericInput(fp, coords, ncoords)
FILE		*fp;
float		coords[];
register int	ncoords;
{
	int	id;
	char	buf[PAGESIZE+1];

	if (fgets(buf, PAGESIZE, fp) == NULL)
		return(0);

	switch(ncoords) {
	case 2:
		sscanf(buf, "%d %e %e",
			&id, &coords[0], &coords[1]);
		break;
	case 4:
		sscanf(buf, "%d %e %e %e %e",
			&id, &coords[0], &coords[1], &coords[2], &coords[3]);
		break;
	case 6:
		sscanf(buf, "%d %e %e %e %e %e %e",
			&id, &coords[0], &coords[1], &coords[2], &coords[3], &coords[4], &coords[5]);
		break;
	case 8:
		sscanf(buf, "%d %e %e %e %e %e %e %e %e",
			&id, &coords[0], &coords[1], &coords[2], &coords[3], &coords[4], &coords[5],
			&coords[6], &coords[7]);
		break;
	default:
		break;
	}

	return(id);
}

int
GenericOutput(rtreep, r, id)
RTREE		*rtreep;
struct Rect	*r;
int		id;
{
	register int	i, j, numdims;

	/* Print out ID X1 Y1 X2 Y2 */

	numdims = rtreep->rthead->NumDims;
	fprintf (rtreep->rtoutfp, "ID# %d:",id);

	for (i = 0; i < numdims; i++)
	{
		j = i * numdims;
		fprintf(rtreep->rtoutfp, " %f %f", r->boundary[j], r->boundary[j+1]);
	}
	fprintf (rtreep->rtoutfp, "\n");

	return(1);
}

