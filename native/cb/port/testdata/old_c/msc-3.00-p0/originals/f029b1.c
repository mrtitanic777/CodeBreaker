#include <ctype.h>
#include "macro.h"
extern char g_2f42[];
extern struct btok *f02817();

/* a macro's body text cut into pieces: identifiers, a quote alone, the text between (the list comes out backwards) */
/* (open: the original's third pass does not turn the outer loop; release 3.00's does) */
struct btok *
f029b1(s)
register char *s;
{
    char pc;
    char *q;
    int x1;                     /* (x1 .. x4 not used) */
    register struct btok *list;
    int x2, x3, x4;

    pc = ' ';
    g_2f42[0] = 0;
    list = 0;
    while (*s)
        if (isalpha(*s) || *s == '_') {
            q = g_2f42;
            while (isalnum(*s) || *s == '_') {
                pc = *s;
                *q++ = *s++;
            }
            *q = 0;
            list = f02817(0x101, 0, g_2f42, list, 3);
        } else if (*s == '"') {
            list = f02817(0x108, 0, "\"", list, 3);
            pc = *s++;
        } else {
            q = g_2f42;
            for (;;) {
                if (!*s)
                    break;
                if (*s == '\\') {
                    pc = *s;
                    *q++ = *s++;
                } else {
                    if (!isalnum(pc) && pc != '_' && (isalpha(*s) || *s == '_'))
                        break;
                    if (*s == '"')
                        break;
                    pc = *s;
                }
                *q++ = *s++;
            }
            *q = 0;
            if (q == g_2f42)
                continue;
            list = f02817(0x108, 0, g_2f42, list, 3);
        }
    return list;
}
