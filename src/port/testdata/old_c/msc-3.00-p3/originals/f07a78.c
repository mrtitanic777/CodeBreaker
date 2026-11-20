#include "opnd.h"
#include "ins.h"
#include "optab.h"
extern struct ins *g_0c60;
extern f06d9a();

/* the byte registers made the registers again; the function placed */
f07a78()
{
    register struct ins *p;

    for (p = g_0c60; p; p = p->next)
        if (p->kind == 0 && p->size == 1)
            switch (g_17b2[p->op].nops) {
            case 2:
                if (p->b->size >= 12)
                    p->b->size -= 12;
            case 1:
                if (p->a->size >= 12)
                    p->a->size -= 12;
            }
    f06d9a();
}
