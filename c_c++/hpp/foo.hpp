#ifndef _FOO_H_
#define _FOO_H_

#if (!defined STATIC) && (!defined INLINE)
int add(int i, int j)
{
    return i + j;
}
#elif defined STATIC 
static int add(int i, int j)
{
    return i + j;
}
#elif defined INLINE
inline int add(int i, int j)
{
    return i + j;
}
#endif

#endif
