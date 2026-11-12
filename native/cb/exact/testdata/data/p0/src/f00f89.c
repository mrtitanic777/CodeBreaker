#include "pool.h"
extern struct pool g_0700[];
extern f01166(), f00ff1();

f00f89(n)
int n;
{
    register struct pool *p;
    register int size;

    p = &g_0700[n];
    if (p->cur == p->base)
        size = (p->size - p->left + 7) & ~7;
    else {
        size = p->size;
        f01166(*p->base);
        *p->base = 0;
        p->cur = p->base;
        p->nblocks = 1;
    }
    p->ptr = (char *)p->base + 4;
    p->left = p->size;
    f00ff1(p->ptr, size);
}
