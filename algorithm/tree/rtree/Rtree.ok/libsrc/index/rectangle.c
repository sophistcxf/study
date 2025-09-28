/*
|| rectangle.c
||
|| Functions:
||	struct	Rect	*FillOneRect(r, coords, ncoords)
||	struct	Rect	*NewRect(ndims, nrects);
||			initRect(r, ndims)
||			freeRect(r)
||			copyRect(rdest, rsrc, ndims)
||			NullRect(r, ndims)
||			RandomRect(r, ndims)
||			CombineRect(rdest, rsrc1, rsrc2, ndims)
||			SearchRect(search, data, ndims)
||	int		Equal2Rects(r1, r2, ndims)
||			PrintRect(r, ndims)
||			PrintRectIdent(r, ndims)
||	float		RectArea(r, ndims)
||	int		Overlap(r, s, ndims)
||	int		Contained(r, s, ndims)
*/

#include	<stdio.h>

#include	"rtree.h"
#include	"assert.h"

	/* R-Tree Functions Returning Ptrs */
struct	Rect	*newRect();
struct	Branch	*newBranch();
struct	Node	*newNode();


#define Undefined(x,dim) ((x)->boundary[0] > (x)->boundary[dim])

long	int seed;

int     NO_REFIN;

/*-----------------------------------------------------------------------------
|| struct Rect	*	FillOneRect(r, coords, ncoords)
||
|| Allocate (as necessary) and fill a rect structure w/ the given values and
|| return a pointer to it.
||---------------------------------------------------------------------------*/
struct Rect	*
FillOneRect(r, coords, ncoords)
register struct Rect	*r;
float			coords[];
register int		ncoords;
{
	register int	i;

	/* If r comes in NULL, allocate space for it */
	if (r == NULL)
		r = newRect(ncoords/2, 1);

	/* Fill in the rect w/ int form of coordinates */
	for (i=0; i<ncoords; i++)
		r->boundary[i] = (COORD_TYPE) coords[i];

	return(r);
}

/*-----------------------------------------------------------------------------
| initRect(r, ndims)
|	Initialize a rectangle to have all 0 coordinates.
-----------------------------------------------------------------------------*/
initRect(r, ndims)
register struct Rect	*r;
register int		ndims;
{
	register int	i;

	for (i=0; i<ndims*2; i++)
		r->boundary[i] = (COORD_TYPE) 0;
}

/*-----------------------------------------------------------------------------
| struct Rect * newRect(ndims, nrect)
|	Allocate rectangle(s) and return them in a (non-linked) list
-----------------------------------------------------------------------------*/
struct Rect *
newRect(ndims, nrect)
int	ndims;		/* No. of dimensions */
int	nrect;		/* No. of rects to allocate */
{
	register struct Rect	*rtmp;
	register int		i;
	register char		*cp;
	struct Rect		*rect;

	rtmp = rect = (struct Rect *) myalloc(nrect *
		(sizeof(struct Rect) + ndims * 2 * COORD_SIZE));

	/* Link coordinate data to rect struct */
	cp = (char *) rect + nrect * sizeof(struct Rect);
	for (i=0; i<nrect; i++)
	{
		rtmp->boundary = (COORD_TYPE *) cp;
		cp += (ndims * 2 * COORD_SIZE);
		rtmp++;
	}

	return(rect);
}

/*-----------------------------------------------------------------------------
| copyRect(rdest, rsrc, ndims)
|	Copy one Rect into another 
-----------------------------------------------------------------------------*/
copyRect(rdest, rsrc, ndims)
struct	Rect	*rdest;		/* Destination Rect */
struct	Rect	*rsrc;		/* Source Rect */
int		ndims;		/* Dimension of  Rects */
{
	/* Due to allocation scheme do a memcopy of the vals */
	memcpy((char *) rdest->boundary, (char *) rsrc->boundary,
		ndims * 2 * COORD_SIZE);
}

/*-----------------------------------------------------------------------------
| freeRect(rect)
|	Free the rectangle list (can be done all at once)
-----------------------------------------------------------------------------*/
freeRect(rect)
struct	Rect	*rect;
{
	myfree(rect);
}

/*-----------------------------------------------------------------------------
| NullRect(ndims)
|	Return a rect whose first low side is higher than its opposite side -
|	interpreted as an undefined rect.
-----------------------------------------------------------------------------*/
struct Rect *
NullRect(ndims)
int	ndims;
{
	register struct	Rect	*r;

	r = newRect(ndims, 1);

	r->boundary[0] = (COORD_TYPE) 1;
	r->boundary[ndims] = (COORD_TYPE) -1;

	return (r);
}

/*-----------------------------------------------------------------------------
| RandomRect(r, ndims)
|	Fills in random coordinates in a rectangle.
|	The low side is guaranteed to be less than the high side.
-----------------------------------------------------------------------------*/
RandomRect(r, ndims)
register struct Rect	*r;
register int		ndims;
{
	register int		i;
	register COORD_TYPE	width;

	for (i = 0; i < ndims; i++)
	{
		/* width from 1 to 1000 / 4, more small ones */
		width = (COORD_TYPE) (rand() % (1000 / 4) + 1);

		/* sprinkle a given size evenly but so they stay in [0,100] */
		r->boundary[i] = (COORD_TYPE) (rand() % (1000 - (int) width)); /* low side */
		r->boundary[i + ndims] = (COORD_TYPE) (r->boundary[i] + width);  /* high side */
	}
}

/*-----------------------------------------------------------------------------
| SearchRect(search, data, ndims)
|	Fill in the boundaries for a random search rectangle.
|	Pass in a pointer to a rect that contains all the data,
|	and a pointer to the rect to be filled in.
|	Generated rect is centered randomly anywhere in the data area,
|	and has size from 0 to the size of the data area in each dimension,
|	i.e. search rect can stick out beyond data area.
-----------------------------------------------------------------------------*/
SearchRect(search, data, ndims)
register struct Rect	*search, *data;
int			ndims;
{
	register COORD_TYPE	size, center;
	register int		i, j;

	assert(search);
	assert(data);

	for (i=0; i<ndims; i++)
	{
		j = i + ndims; /* index for high side boundary */
		if (data->boundary[i] > (COORD_TYPE) MININT
			&& data->boundary[j] < (COORD_TYPE) MAXINT)
		{
			size = (rand() % (int) (data->boundary[j] - data->boundary[i] + 1)) / 2;
			center = data->boundary[i]
				+ rand() % (int) (data->boundary[j] - data->boundary[i] + 1);
			search->boundary[i] = center - size/2;
			search->boundary[j] = center + size/2;
		}
		else /* some open boundary, search entire dimension */
		{
			search->boundary[i] = (COORD_TYPE) MININT;
			search->boundary[j] = (COORD_TYPE) MAXINT;
		}
	}
}

/*-----------------------------------------------------------------------------
| Equal2Rects(r, s, ndims)
|	Decide whether two rectangles are the same
-----------------------------------------------------------------------------*/
int
Equal2Rects(r, s, ndims)
register struct Rect	*r, *s;
register	int	ndims;
{
	register int	i;

	assert(r && s);

	for (i=0; i<ndims*2; i++)
	{
		if (r->boundary[i] - s->boundary[i])
			return (FALSE);
	}
	return(TRUE);
}

/*-----------------------------------------------------------------------------
| PrintRect(r, ndims)
|	Print out the data for a rectangle.
-----------------------------------------------------------------------------*/
PrintRect(r, ndims)
register struct Rect	*r;
register int		ndims;
{
	register int	i;

	assert(r);

	printf("rect:");
	for (i = 0; i < ndims; i++)
		printf("\t%f\t%f\n", r->boundary[i], r->boundary[i + ndims]);
}

/*-----------------------------------------------------------------------------
| PrintRectIdent(r, ndims)
|	Print out the data for a rectangle (indented).
-----------------------------------------------------------------------------*/
PrintRectIdent(r, ndims)
register struct Rect	*r;
register int		ndims;
{
	register int	i;

	assert(r);

	printf("\trect:");
		printf("\t%f\t%f\n", r->boundary[0], r->boundary[0 + ndims]);
	for (i = 1; i < ndims; i++)
		printf("\t\t%f\t%f\n", r->boundary[i], r->boundary[i + ndims]);
}


/*-----------------------------------------------------------------------------
| RectArea(r, ndims)
|	Calculate the n-dimensional area of a rectangle
-----------------------------------------------------------------------------*/
float
RectArea(r, ndims)
register struct Rect	*r;
register int		ndims;
{
	register int	i;
	register double	area = 1.0;

	assert(r);

	if (Undefined(r, ndims))
		return (0);

	for (i=0; i<ndims; i++)
	{
		area *= r->boundary[i+ndims] - r->boundary[i];
	}
	return ((float) area);
}

/*-----------------------------------------------------------------------------
| CombineRect(rdest, rsrc1, rsrc2, ndims)
|	Combine two rectangles, make one that includes both.
-----------------------------------------------------------------------------*/
CombineRect(rdest, rsrc1, rsrc2, ndims)
register struct Rect	*rdest, *rsrc1, *rsrc2;
register int		ndims;
{
	register int i, j;

	assert(rsrc1 && rsrc2);

	if (Undefined(rsrc1, ndims))
	{
		copyRect(rdest, rsrc2, ndims);
		return;
	}

	if (Undefined(rsrc2, ndims))
	{
		copyRect(rdest, rsrc1, ndims);
		return;
	}

	for (i = 0; i < ndims; i++)
	{
		rdest->boundary[i] = MIN(rsrc1->boundary[i], rsrc2->boundary[i]);
		j = i + ndims;
		rdest->boundary[j] = MAX(rsrc1->boundary[j], rsrc2->boundary[j]);
	}
	return;
}

/*-----------------------------------------------------------------------------
| Overlap(r, s, ndims)
|	Decide whether two rectangles overlap.
-----------------------------------------------------------------------------*/
Overlap(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	register int i, j;
	assert(r && s);

	for (i=0; i<ndims; i++)
	{
		j = i + ndims;  /* index for high sides */
		if (r->boundary[i] > s->boundary[j] || s->boundary[i] > r->boundary[j])
			return (FALSE);
	}
	return (TRUE);
}

/*-----------------------------------------------------------------------------
| Contained(r, s, ndims)
|	Decide whether rectangle r is contained in rectangle s.
-----------------------------------------------------------------------------*/
Contained(r, s, ndims)
register struct Rect	*r, *s;
int			ndims;
{
	register int i, j, result;

	assert((int)r && (int)s);

 	/* undefined rect is contained in any other */
	if (Undefined(r, ndims))
		return TRUE;

	/* no rect (except an undefined one) is contained in an undef rect */
	if (Undefined(s, ndims))
		return FALSE;

	result = TRUE;
	for (i = 0; i < ndims; i++)
	{
		j = i + ndims;  /* index for high sides */
		result = result
			&& r->boundary[i] >= s->boundary[i]
			&& r->boundary[j] <= s->boundary[j];
	}
	return (result);
}


/******************************************************************************
***********************   TOPOLOGICAL RELATIONS *******************************
*******************************************************************************/

/*-----------------------------------------------------------------------------
| Disjoint(r, s, ndims)
|       Decide whether rect s and rect r are disjoint.
-----------------------------------------------------------------------------*/
Disjoint(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
	if (Overlap(r, s, ndims))
		return (FALSE);
	else
		return (TRUE);
}

Disjoint2(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        if (RYX(r,s,ndims,4,6) || RYX(r,s,ndims,4,7) || RYX(r,s,ndims,4,9) || 
            RYX(r,s,ndims,4,10) || RYX(r,s,ndims,5,6) || RYX(r,s,ndims,5,7) || 
            RYX(r,s,ndims,5,9) || RYX(r,s,ndims,5,10) || RYX(r,s,ndims,7,6) || 
            RYX(r,s,ndims,7,7) || RYX(r,s,ndims,7,9) || RYX(r,s,ndims,7,10) || 
            RYX(r,s,ndims,10,6) || RYX(r,s,ndims,10,7) || RYX(r,s,ndims,10,9) || 
            RYX(r,s,ndims,10,10) || RYX(r,s,ndims,6,4) || RYX(r,s,ndims,6,5) || 
            RYX(r,s,ndims,6,7) || RYX(r,s,ndims,6,8) || RYX(r,s,ndims,7,4) || 
            RYX(r,s,ndims,7,5) || RYX(r,s,ndims,7,7) || RYX(r,s,ndims,7,8) || 
            RYX(r,s,ndims,9,4) || RYX(r,s,ndims,9,5) || RYX(r,s,ndims,9,7) || 
            RYX(r,s,ndims,9,8) || RYX(r,s,ndims,10,4) || RYX(r,s,ndims,10,5) || 
            RYX(r,s,ndims,10,7) || RYX(r,s,ndims,10,8))
        	return (FALSE);
	else
		{
		if (RY(r,s,ndims,1) || RY(r,s,ndims,13) ||
	  	    RX(r,s,ndims,1) || RX(r,s,ndims,13))
			NO_REFIN ++;
		return (TRUE);
		}
}

Disjoint1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        return (TRUE);
}

/*-----------------------------------------------------------------------------
| SOverlap(r, s, ndims)
|	Decide whether rect s strictly overlaps rect r.
-----------------------------------------------------------------------------*/
SOverlap(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Overlap(r, s, ndims))
	{
		if (Cover(r, s, ndims) || Contain(r, s, ndims) || 
			Equal(r, s, ndims) || Inside(r, s, ndims) || 
			Covered_by(r, s, ndims) || Meet(r, s, ndims))
			return (FALSE);
		return (TRUE);
	}
	return (FALSE);
}

SOverlap2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RX(r,s,ndims,1) || RX(r,s,ndims,2) || 
            RX(r,s,ndims,12) || RX(r,s,ndims,13) || 
            RY(r,s,ndims,1) || RY(r,s,ndims,2) || 
            RY(r,s,ndims,12) || RY(r,s,ndims,13))
        	return (FALSE);
	else
		{
        	if (RYX(r,s,ndims,4,9) || RYX(r,s,ndims,5,6) || 
            	    RYX(r,s,ndims,5,9) || RYX(r,s,ndims,5,10) || 
            	    RYX(r,s,ndims,6,5) || RYX(r,s,ndims,8,9) || 
            	    RYX(r,s,ndims,9,4) || RYX(r,s,ndims,9,5) || 
            	    RYX(r,s,ndims,9,8) || RYX(r,s,ndims,10,5))
			NO_REFIN ++;
		return (TRUE);
		}
}

SOverlap1(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Disjoint(r, s, ndims) || Meet(r, s, ndims))
		return (FALSE);
	else
		return (TRUE);
}

/*-----------------------------------------------------------------------------
| Cover(r, s, ndims)
|	Decide whether rect s covers rect r
-----------------------------------------------------------------------------*/
Cover(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	register int i, j;
	assert(r && s);

	for (i=0; i<ndims; i++)
	{
		j = i + ndims;  /* index for high sides */
		if (s->boundary[i] > r->boundary[i] || s->boundary[j] < r->boundary[j])
			return (FALSE);
	}
	if (Contain(r, s, ndims) || Equal(r, s, ndims))
		return (FALSE);
	return (TRUE);
}

Cover2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RYX(r,s,ndims,4,4) || RYX(r,s,ndims,4,5) || RYX(r,s,ndims,4,7) || 
            RYX(r,s,ndims,4,8) || RYX(r,s,ndims,5,4) || RYX(r,s,ndims,5,5) || 
            RYX(r,s,ndims,5,7) || RYX(r,s,ndims,5,8) || RYX(r,s,ndims,7,4) || 
            RYX(r,s,ndims,7,5) || RYX(r,s,ndims,7,7) || RYX(r,s,ndims,7,8) || 
            RYX(r,s,ndims,8,4) || RYX(r,s,ndims,8,5) || RYX(r,s,ndims,8,7) || 
            RYX(r,s,ndims,8,8))
        	return (TRUE);
	else
		return (FALSE);
}

Cover1(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Cover(r, s, ndims) || Contain(r, s, ndims) || Equal(r, s, ndims))
		return (TRUE);
	else
		return (FALSE);
}

/*-----------------------------------------------------------------------------
| Contain(r, s, ndims)
|	Decide whether rect s contains rect r
-----------------------------------------------------------------------------*/
Contain(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	register int i, j;
	assert(r && s);

	for (i=0; i<ndims; i++)
	{
		j = i + ndims;  /* index for high sides */
		if (s->boundary[i] >= r->boundary[i] || s->boundary[j] <= r->boundary[j])
			return (FALSE);
	}
	return (TRUE);
}

Contain2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RYX(r,s,ndims,5,5))
        	return (TRUE);
	else
		return (FALSE);
}

Contain1(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Contain(r, s, ndims))
		return (TRUE);
	else
		return (FALSE);
}

/*-----------------------------------------------------------------------------
| Equal(r, s, ndims)
|	Decide whether rect s equals rect r
-----------------------------------------------------------------------------*/
Equal(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	register int i, j;
	assert(r && s);

	for (i=0; i<ndims; i++)
	{
		j = i + ndims;  /* index for high sides */
		if (s->boundary[i] != r->boundary[i] || s->boundary[j] != r->boundary[j])
			return (FALSE);
	}
	return (TRUE);
}

Equal2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RYX(r,s,ndims,7,7))
        	return (TRUE);
	else
		return (FALSE);
}

Equal1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
	if (Cover(r, s, ndims) || Contain(r, s, ndims) || Equal(r, s, ndims))
		return (TRUE);
	else
		return (FALSE);
}

/*-----------------------------------------------------------------------------
| Inside(r, s, ndims)
|	Decide whether rect s is inside rect r
-----------------------------------------------------------------------------*/
Inside(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	return (Contain(s, r, ndims));
}

Inside2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RYX(r,s,ndims,9,9))
        	return (TRUE);
	else
		return (FALSE);
}

Inside1(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Disjoint(r, s, ndims) || Meet(r, s, ndims))
		return (FALSE);
	else
		return (TRUE);
}

/*-----------------------------------------------------------------------------
| Covered_by(r, s, ndims)
|	Decide whether rect s is covered_by rect r
-----------------------------------------------------------------------------*/
Covered_by(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	return (Cover(s, r, ndims));
}

Covered_by2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RYX(r,s,ndims,6,6) || RYX(r,s,ndims,6,7) || RYX(r,s,ndims,6,9) || 
            RYX(r,s,ndims,6,10) || RYX(r,s,ndims,7,6) || RYX(r,s,ndims,7,7) || 
            RYX(r,s,ndims,7,9) || RYX(r,s,ndims,7,10) || RYX(r,s,ndims,9,6) || 
            RYX(r,s,ndims,9,7) || RYX(r,s,ndims,9,9) || RYX(r,s,ndims,9,10) || 
            RYX(r,s,ndims,10,6) || RYX(r,s,ndims,10,7) || RYX(r,s,ndims,10,9) || 
            RYX(r,s,ndims,10,10))
        	return (TRUE);
	else
		return (FALSE);
}

Covered_by1(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Disjoint(r, s, ndims) || Meet(r, s, ndims))
		return (FALSE);
	else
		return (TRUE);
}

/*-----------------------------------------------------------------------------
| Meet(r, s, ndims)
|	Decide whether rect s meets rect r
-----------------------------------------------------------------------------*/
Meet(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	register int i, j;
	assert(r && s);

	if (Overlap(r, s, ndims))
		for (i=0; i<ndims; i++)
		{
			j = i + ndims;  /* index for high sides */
			if (s->boundary[i] == r->boundary[j] || 
		    	r->boundary[i] == s->boundary[j])
				return (TRUE);
		}
	return (FALSE);
}

Meet2(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
        if (RX(r,s,ndims,1) || RX(r,s,ndims,13) || 
            RY(r,s,ndims,1) || RY(r,s,ndims,13) || 
            RYX(r,s,ndims,4,9) || RYX(r,s,ndims,5,6) || RYX(r,s,ndims,5,7) || 
            RYX(r,s,ndims,5,9) || RYX(r,s,ndims,5,10) || RYX(r,s,ndims,6,5) || 
            RYX(r,s,ndims,7,5) || RYX(r,s,ndims,7,9) || RYX(r,s,ndims,8,9) || 
            RYX(r,s,ndims,9,4) || RYX(r,s,ndims,9,5) || RYX(r,s,ndims,9,7) || 
            RYX(r,s,ndims,9,8) || RYX(r,s,ndims,10,5))
        	return (FALSE);
	else
		return (TRUE);
}

Meet1(r, s, ndims)
register struct	Rect	*r, *s;
int			ndims;
{
	if (Disjoint(r, s, ndims))
		return (FALSE);
	else
		return (TRUE);
}

/*-----------------------------------------------------------------------------
| RYX(r, s, ndims, y, x), RY(r, s, ndims, y), RX(r, s, ndims, x)
|       Primitive direction functions . x=1..13, y=1..13
-----------------------------------------------------------------------------*/
RYX(r, s, ndims, y, x)
register struct Rect    *r, *s;
int                     ndims, y, x;
{
	if (RY(r, s, ndims, y) && RX(r, s, ndims, x))
		return (TRUE);
	else
		return (FALSE);
}

RY(r, s, ndims, y)
register struct Rect    *r, *s;
int                     ndims, y;
{
        assert(r && s);

	if (y==1)
        	return (s->boundary[1] > r->boundary[3]);
	if (y==2)
        	return (s->boundary[1] == r->boundary[3]);
	if (y==3)
        	return ((s->boundary[3] > r->boundary[3]) &&
                	(s->boundary[1] > r->boundary[1]) &&
                	(s->boundary[1] < r->boundary[3]));
	if (y==4)
        	return ((s->boundary[3] > r->boundary[3]) &&
                	(s->boundary[1] == r->boundary[1]));
	if (y==5)
        	return ((s->boundary[3] > r->boundary[3]) &&
                	(s->boundary[1] < r->boundary[1]));
	if (y==6)
        	return ((s->boundary[3] == r->boundary[3]) &&
                	(s->boundary[1] > r->boundary[1]));
	if (y==7)
        	return ((s->boundary[3] == r->boundary[3]) &&
                	(s->boundary[1] == r->boundary[1]));
	if (y==8)
        	return ((s->boundary[3] == r->boundary[3]) &&
                	(s->boundary[1] < r->boundary[1]));
	if (y==9)
        	return ((s->boundary[3] < r->boundary[3]) &&
                	(s->boundary[1] > r->boundary[1]));
	if (y==10)
        	return ((s->boundary[3] < r->boundary[3]) &&
                	(s->boundary[1] == r->boundary[1]));
	if (y==11)
        	return ((s->boundary[3] < r->boundary[3]) &&
                	(s->boundary[3] > r->boundary[1]) &&
                	(s->boundary[1] < r->boundary[1]));
	if (y==12)
        	return (s->boundary[3] == r->boundary[1]);
	if (y==13)
        	return (s->boundary[3] < r->boundary[1]);
}

RX(r, s, ndims, x)
register struct Rect    *r, *s;
int                     ndims, x;
{
        assert(r && s);

	if (x==1)
        	return (s->boundary[0] > r->boundary[2]);
	if (x==2)
        	return (s->boundary[0] == r->boundary[2]);
	if (x==3)
        	return ((s->boundary[2] > r->boundary[2]) &&
                	(s->boundary[0] > r->boundary[0]) &&
                	(s->boundary[0] < r->boundary[2]));
	if (x==4)
        	return ((s->boundary[2] > r->boundary[2]) &&
                	(s->boundary[0] == r->boundary[0]));
	if (x==5)
        	return ((s->boundary[2] > r->boundary[2]) &&
                	(s->boundary[0] < r->boundary[0]));
	if (x==6)
        	return ((s->boundary[2] == r->boundary[2]) &&
                	(s->boundary[0] > r->boundary[0]));
	if (x==7)
        	return ((s->boundary[2] == r->boundary[2]) &&
                	(s->boundary[0] == r->boundary[0]));
	if (x==8)
        	return ((s->boundary[2] == r->boundary[2]) &&
                	(s->boundary[0] < r->boundary[0]));
	if (x==9)
        	return ((s->boundary[2] < r->boundary[2]) &&
                	(s->boundary[0] > r->boundary[0]));
	if (x==10)
        	return ((s->boundary[2] < r->boundary[2]) &&
                	(s->boundary[0] == r->boundary[0]));
	if (x==11)
        	return ((s->boundary[2] < r->boundary[2]) &&
                	(s->boundary[2] > r->boundary[0]) &&
                	(s->boundary[0] < r->boundary[0]));
	if (x==12)
        	return (s->boundary[2] == r->boundary[0]);
	if (x==13)
        	return (s->boundary[2] < r->boundary[0]);
}



/******************************************************************************
***********************   DIRECTION RELATIONS *********************************
*******************************************************************************/

/*-----------------------------------------------------------------------------
| Strong_North(r, s, ndims)
|       Decide whether rect s is Strong_North of rect r.
-----------------------------------------------------------------------------*/
Strong_North(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return (s->boundary[1] > r->boundary[3]);
}

Strong_North1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return (s->boundary[3] > r->boundary[3]);
}

/*-----------------------------------------------------------------------------
| Weak_North(r, s, ndims)
|       Decide whether rect s is Weak_North of rect r.
-----------------------------------------------------------------------------*/
Weak_North(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] > r->boundary[1]) &&
                (s->boundary[1] < r->boundary[3]));
}

Weak_North1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] < r->boundary[3]));
}

/*-----------------------------------------------------------------------------
| Strong_Bounded_North(r, s, ndims)
|       Decide whether rect s is Strong_Bounded_North of rect r.
-----------------------------------------------------------------------------*/
Strong_Bounded_North(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[1] > r->boundary[3]) &&
                (s->boundary[1] > r->boundary[1]) &&
                (s->boundary[0] > r->boundary[0]) &&
                (s->boundary[2] < r->boundary[2]) &&
                (s->boundary[3] > r->boundary[3]));
}

Strong_Bounded_North1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[0] < r->boundary[2]) &&
                (s->boundary[2] > r->boundary[0]));
}

/*-----------------------------------------------------------------------------
| Weak_Bounded_North(r, s, ndims)
|       Decide whether rect s is Weak_Bounded_North of rect r.
-----------------------------------------------------------------------------*/
Weak_Bounded_North(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] < r->boundary[3]) &&
                (s->boundary[0] > r->boundary[0]) &&
                (s->boundary[1] > r->boundary[1]) &&
                (s->boundary[2] < r->boundary[2]));
}

Weak_Bounded_North1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] < r->boundary[3]) &&
                (s->boundary[0] < r->boundary[2]) &&
                (s->boundary[2] > r->boundary[0]));
}

/*-----------------------------------------------------------------------------
| Strong_NorthEast(r, s, ndims)
|       Decide whether rect s is Strong_NorthEast of rect r.
-----------------------------------------------------------------------------*/
Strong_NorthEast(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[1] > r->boundary[3]) &&
                (s->boundary[0] > r->boundary[2]));
}

Strong_NorthEast1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[2] > r->boundary[2]));
}

/*-----------------------------------------------------------------------------
| Weak_NorthEast(r, s, ndims)
|       Decide whether rect s is Weak_NorthEast of rect r.
-----------------------------------------------------------------------------*/
Weak_NorthEast(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[2] > r->boundary[2]) &&
                (s->boundary[1] > r->boundary[1]) &&
                (s->boundary[0] > r->boundary[0]) &&
                (s->boundary[1] < r->boundary[3]));
}

Weak_NorthEast1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[2] > r->boundary[2]) &&
                (s->boundary[1] < r->boundary[3]));
}

/*-----------------------------------------------------------------------------
| Same_Level(r, s, ndims)
|       Decide whether rect s is Same_Level of rect r.
-----------------------------------------------------------------------------*/
Same_Level(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[1] >= r->boundary[1]) &&
                (s->boundary[3] <= r->boundary[3]));
}

Same_Level1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] >= r->boundary[1]) &&
                (s->boundary[1] <= r->boundary[3]));
}

/*-----------------------------------------------------------------------------
| Strong_Same_Level(r, s, ndims)
|       Decide whether rect s is Strong_Same_Level of rect r.
-----------------------------------------------------------------------------*/
Strong_Same_Level(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[1] == r->boundary[1]) &&
                (s->boundary[3] == r->boundary[3]));
}

Strong_Same_Level1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] >= r->boundary[1]) &&
                (s->boundary[1] <= r->boundary[3]));
}

/*-----------------------------------------------------------------------------
| Just_North(r, s, ndims)
|       Decide whether rect s is Just_North of rect r.
-----------------------------------------------------------------------------*/
Just_North(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return (s->boundary[1] == r->boundary[3]);
}

Just_North1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] >= r->boundary[3]) &&
                (s->boundary[1] <= r->boundary[3]));
}

/*-----------------------------------------------------------------------------
| North_South(r, s, ndims)
|       Decide whether rect s is North_South of rect r.
-----------------------------------------------------------------------------*/
North_South(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] < r->boundary[1]));
}

North_South1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] < r->boundary[1]));
}

/*-----------------------------------------------------------------------------
| North(r, s, ndims)
|       Decide whether rect s is North of rect r.
-----------------------------------------------------------------------------*/
North(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return ((s->boundary[3] > r->boundary[3]) &&
                (s->boundary[1] > r->boundary[1]));
}

North1(r, s, ndims)
register struct Rect    *r, *s;
int                     ndims;
{
        assert(r && s);
        return (s->boundary[3] > r->boundary[3]);
}

