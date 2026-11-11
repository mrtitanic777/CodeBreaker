#include "pool.h"
extern struct pool g_10d0[];
extern f03fa7(), f03e47();

f03de2(n)
int n;
{
    register struct pool *p;
    register int size;

    p = &g_10d0[n];
    if (p->cur == p->base)
        size = (p->size - p->left + 7) & ~7;
    else {
        size = p->size;
        f03fa7(*p->base);
        *p->base = 0;
        p->cur = p->base;
        p->nblocks = 1;
    }
    p->ptr = (char *)p->base + 4;
    p->left = p->size;
    f03e47(p->ptr, size);
}
