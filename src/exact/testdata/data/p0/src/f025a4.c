#include <ctype.h>
#include "macro.h"
extern int g_0ad2;
extern int (*g_09da)(), (*g_09dc)();
extern char g_3270[];
extern char *f0330d(), *f01135();
extern struct btok *f02817(), *f029b1();
extern f01fd0(), f0010e(), f03a59(), f03a28(), f0391c(), f039e2(), f01ff6();

/* (open: the third pass places the formal list's end and tests "dup" otherwise than the original's) */
/* #define: the name, the formals, the body (the frame's order of the locals follows from their names) */
f025a4()
{
    register int ci;
    struct btok *t;
    register char *to;
    struct btok *flist, *form;
    int k;
    char *ip;
    struct btok *tail;
    int nformals;
    struct btok *bl;
    char *mac;
    struct btok *dup;
    struct btok *new;

    ci = f01fd0();
    if (!isalpha(ci) && ci != '_') {
        f0010e("#define syntax");
        f03a59();
        return 0x16f;
    }
    ip = f0330d(ci);
    mac = f01135(ip, 0);
    tail = flist = 0;
    g_0ad2 = 1;
    ci = (*g_09da)();
    if (!isspace(ci) && ci && ci != '(' && ci != '\\')
        f0010e("`%c' : unexpected in macro definition", ci);
    else if (ci == '\n' || ci == '\\')
        (*g_09dc)(ci);
    nformals = 0;
    if (ci == '(') {
        for (;;) {
            ci = f03a28();
again:
            if (!isalpha(ci) && ci != '_')
                break;
            new = f02817(0x101, 0, f0330d(ci), 0, 3);
            if (!flist) {
                flist = tail = new;
                nformals = 1;
            } else {
                for (dup = flist; dup; dup = dup->next)
                    if (!strcmp(new->text, dup->text))
                        f0010e("reuse of macro formal `%s'", new->text);
                tail = tail->next = new;
                nformals++;
            }
            if ((ci = f03a28()) != ',')
                goto again;
        }
        if (ci != ')')
            f0010e("`%c' : unexpected in formal list", ci);
        else if (!flist)
            nformals = -1;
    }
    ci = f03a28();
    to = g_3270;
    k = 0;
    for (;;) {
        if (ci == '\n')
            break;
        if (!(ci == '\\' && f0391c('\n')) && !f039e2(ci)) {
            if (k == 0xff) {
                f0010e("`%s' : definition too big", mac);
                break;
            }
            *to++ = ci;
            k++;
        }
        ci = (*g_09da)();
    }
    (*g_09dc)(ci);
    *to = 0;
    bl = 0;
    for (t = f029b1(g_3270); t; t = t->next) {
        if (t->kind == 0x101) {
            k = 0;
            for (form = flist; form; form = form->next, k++)
                if (!strcmp(form->text, t->text)) {
                    t->kind = 0x17a;
                    t->argno = k;
                    break;
                }
            if (!form)
                t->kind = 0x108;
        }
        bl = f02817(t->kind, t->argno, t->text, bl, 0);
    }
    f01ff6(mac, bl, nformals);
    g_0ad2 = 0;
    return 1;
}
