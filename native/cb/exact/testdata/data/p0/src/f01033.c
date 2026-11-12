#include "pool.h"
extern struct pool g_0700[];
extern char *malloc();
extern f00143(), f00ff1();

char *
f01033(n, size)
int n, size;
{
    register struct pool *p;
    register char **q;

    p = &g_0700[n];
    size = (size + 1) & ~1;
    q = p->cur;
    if (size > p->left) {
        if (*q == 0) {
            p->nblocks++;
            if ((*q = malloc(p->size + 4)) == 0)
                f00143("Compiler error (internal): out of heap space");
            *(char **)*q = 0;
        }
        p->ptr = (char *)(p->cur = (char **)*q) + 4;
        f00ff1(p->ptr, p->left = p->size);
    }
    q = (char **)p->ptr;
    p->ptr += size;
    p->left -= size;
    return (char *)q;
}
