#include "cell.h"
extern struct cell *g_11b2;     /* (the free cells) */
extern struct cell *g_11ae, *g_11b0;    /* (the list being made, its first cell) */
extern f03dad();

/* a cell taken from the free ones and put at the head of the list */
struct cell *
f03fec()
{
    int x;                      /* (not used) */
    register struct cell *p;

    if (g_11b2) {
        p = g_11b2;
        g_11b2 = p->next;
    } else
        f03dad();
    if (!g_11b0)
        g_11b0 = p;
    p->next = g_11ae;
    g_11ae = p;
    return p;
}
