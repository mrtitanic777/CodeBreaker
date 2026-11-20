#include "opnd.h"
#include "ins.h"
#include "lab.h"
#include "recfmt.h"
extern struct ins *g_35f2[];
extern struct recfmt g_0074[], g_1010[];
extern f03f41();

/* whether there is room for a jump to a label: a reference, an instruction, an operand */
f06edb()
{
    if ((g_2fb8 || f03f41(4, 3)) && (g_35f2[0] || f03f41(g_0074[0].n, 2)) && f03f41(g_1010[10].n, 2))
        return 1;
    return 0;
}
