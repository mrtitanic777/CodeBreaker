#include "opnd.h"
extern int g_1ff8, g_3aa0;

/* the operand o made the register r */
f08e99(o, r)
register struct opnd *o;
char r;
{
    o->kind = 1;
    o->size = r;
    o->o2 = 0;
    g_1ff8++;
    g_3aa0 = 1;
}
