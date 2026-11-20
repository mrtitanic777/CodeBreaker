#include <ctype.h>

/* a digit's value (hexadecimal digits too) */
f03ada(c)
register int c;
{
    if (isdigit(c))
        return c - '0';
    if (islower(c))
        return c - 'a' + 10;
    if (isupper(c))
        return c - 'A' + 10;
}
