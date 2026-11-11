#include "pool.h"
extern struct pool g_10d0[];
extern char *f03e86();

f03f41(size, n)
int size, n;
{
    register struct pool *p;

    if (!f03e86(n, size))
        return 0;
    p = &g_10d0[n];
    p->ptr -= size;
    p->left += size;
    return 1;
}
