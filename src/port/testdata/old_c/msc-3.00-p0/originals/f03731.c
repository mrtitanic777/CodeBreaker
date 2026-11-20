#include <ctype.h>
extern char *g_0e52;
extern f00081();

/* a digit's value, for the conversion of numbers (anything else is the compiler's error) */
f03731(c)
char c;
{
    if (isdigit(c))
        return c - '0';
    if (isupper(c))
        return c - 'A' + 10;
    if (islower(c))
        return c - 'a' + 10;
    f00081(g_0e52, 736);
}
