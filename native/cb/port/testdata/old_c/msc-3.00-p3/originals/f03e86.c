#include "pool.h"
extern struct pool g_10d0[];
extern char *malloc();
extern f03e47();

/* size bytes from the pool n: 0 when the heap is out (the caller gives the function up) */
char *
f03e86(n, size)
int n, size;
{
    register struct pool *p;
    register char **q;

    p = &g_10d0[n];
    size = (size + 1) & ~1;
    q = p->cur;
    if (size > p->left) {
        if (*q == 0) {
            p->nblocks++;
            if ((*q = malloc(p->size + 4)) == 0)
                return 0;
            *(char **)*q = 0;
        }
        p->ptr = (char *)(p->cur = (char **)*q) + 4;
        f03e47(p->ptr, p->left = p->size);
    }
    q = (char **)p->ptr;
    p->ptr += size;
    p->left -= size;
    return (char *)q;
}
