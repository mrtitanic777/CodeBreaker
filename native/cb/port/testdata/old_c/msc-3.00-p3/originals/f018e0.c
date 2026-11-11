#include "seg.h"
extern printf();

/* the listing's end: the last segment ended */
f018e0()
{
    if (g_03d8)
        printf("%s\tENDS\n", g_03d8->name);
    printf("END\n");
}
