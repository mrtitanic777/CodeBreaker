#include <ctype.h>
extern f01fd0(), f039e2();

/* the next character that is not a blank or in a comment (a newline is not skipped) */
f03a28()
{
    register int c;

    do
        c = f01fd0();
    while (f039e2(c) || (isspace(c) || !c) && c != '\n');
    return c;
}
