#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern struct ins *f06e12();

/* the flags that the item p sets not used after it (through as many as 7 jumps): 1 if so */
f08816(p)
register struct ins *p;
{
    register n;

    p = f06e12(p);
    for (n = 0; p && p->op == 0x3d; n++) {
        if (n >= 7)
            return 0;
        p = f06e12(((struct lab *)p->a->sym)->def);
    }
    if (!p)
        return 1;
    switch (p->op) {
    case 0x12:
    case 0x15:
    case 0x3f:
        return 0;
    }
    return 1;
}
