#include "pool.h"
extern struct pool g_0700[];
extern char *f01033();

f010fa(size, n)
int size, n;
{
    register struct pool *p;

    if (!f01033(n, size))
        return 0;
    p = &g_0700[n];
    p->ptr -= size;
    p->left += size;
    return 1;
}
