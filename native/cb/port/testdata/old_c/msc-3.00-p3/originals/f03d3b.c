#include "psym.h"
extern struct psym *g_0cc4;
extern long g_1e4a;             /* (the procedure's place as the second pass had it) */

/* the label s moved as its procedure was */
f03d3b(s)
struct psym *s;
{
    if (s->kind == 1)
        s->place -= g_0cc4->place - g_1e4a;
}
