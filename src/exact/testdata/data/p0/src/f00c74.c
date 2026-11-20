#include "keywords.h"
extern struct sym **g_056c;
extern unsigned g_056e;
extern int g_05e0;
extern f005c1();

f00c74(name)
char *name;
{
    register int h;
    register struct sym *p;

    h = f005c1(name);
    for (p = g_056c[(unsigned)h % g_056e]; p; p = p->next)
        if (p->hash == h && !strcmp(p->name, name)) {
            if (!g_05e0)
                switch (p->val) {
                case 0x11b:
                case 0x11c:
                case 0x11e:
                case 0x11f:
                    return 0x101;
                }
            return p->val;
        }
    return 0x101;
}
