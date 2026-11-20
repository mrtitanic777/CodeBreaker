#include <ctype.h>

/* the string s made upper case */
f04805(s)
register char *s;
{
    for (; *s; s++)
        if (islower(*s))
            *s = toupper(*s);
}
