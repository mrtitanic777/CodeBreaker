#include "recfmt.h"
extern struct recfmt g_0074[], g_2096[];
extern f03f41();

/* whether there is room for an instruction and a label */
f06ea9()
{
    if (f03f41(g_0074[1].n, 2) && f03f41(g_2096[1].n, 1))
        return 1;
    return 0;
}
