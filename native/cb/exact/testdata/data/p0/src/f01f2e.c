#include <stdio.h>
#include "incl.h"
extern int g_0a42;
extern char g_3270[];
extern struct incl g_09de[];
extern char *f03b38();
extern f00213(), f01d6e();

f01f2e(name)
char *name;
{
    register int i;
    char *n;
    register char *p;
    char *iname;
    char *to;

    for (i = g_0a42; i >= 0; i--) {
        iname = g_09de[i].iname;
        if ((p = f03b38(iname, '/')) == 0)
            f00213(g_3270, "%s", name);
        else {
            for (to = g_3270; iname <= p; )
                *to++ = *iname++;
            n = name;
            while (*to++ = *n++)
                ;
        }
        if (f01d6e(g_3270, 0))
            return 1;
    }
    return 0;
}
