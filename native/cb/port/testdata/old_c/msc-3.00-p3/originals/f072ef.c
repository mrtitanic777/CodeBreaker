#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern struct ins *f06df2();

/* whether the labels from the jump p's target to the label after p are jumped to only from between them (a
   part of the function no one enters from outside) */
f072ef(p)
register struct ins *p;
{
    struct ref *r;
    register struct ins *head;
    struct ins *next;
    int k;

    if ((next = f06df2(p))->kind != 1)
        return 0;
    for (p = head = ((struct lab *)p->a->sym)->def; next != p; p = LNEXT(p))
        if (!p)
            return 0;
    for (p = head; ; p = LNEXT(p)) {
        ((struct lab *)p->op)->place = 0;
        if (next == p)
            break;
    }
    for (p = head; next != p; p = p->next)
        if (p->kind == 0 && (p->op == 0x3d || p->op == 0x3f) && p->a->kind == 0xa)
            ((struct lab *)p->a->sym)->place++;
    for (p = LNEXT(head); ; p = LNEXT(p)) {
        k = 0;
        for (r = ((struct lab *)p->op)->refs; r; r = r->next)
            if (r->ins->kind != 7)
                k++;
        if (k != ((struct lab *)p->op)->place && f06df2(p)->kind != 7)
            return 0;
        if (next == p)
            return 1;
    }
}
