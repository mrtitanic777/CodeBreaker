#include "opnd.h"
#include "psym.h"
extern struct opnd *g_264e, *g_2650;
extern char g_264c;
extern f020a8(), f01eb6(), printf();

/* the operand p of the instruction in the listing, its size said when it is not plain */
f01e23(p)
struct opnd *p;
{
    int k;

    k = p->kind;
    if (p == g_264e && f020a8(p) && (!g_2650 || f020a8(g_2650))
        || (k == 3 || k == 5 || k == 7) && ((struct psym *)p->sym)->cls != g_264c)
        printf("%s PTR ", g_264c == 1 ? "BYTE" : g_264c == 2 ? "WORD" : g_264c == 4 ? "DWORD" : "QWORD");
    f01eb6(p);
}
