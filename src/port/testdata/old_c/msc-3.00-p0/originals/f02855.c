#include <ctype.h>
extern char g_3270[];
extern f03a28(), f01fd0(), f0010e(), f03a59(), f028ca();

/* (open: the original's first loop tests all three conditions at its foot; release 3.00 splits them) */
/* #include <name> */
f02855()
{
    register char *p;
    char ch;
    int u;                      /* (not used) */

    p = g_3270;
    if ((ch = f03a28()) == '\n') {
        f0010e("missing name following `<'");
        return 0x16f;
    }
    while (!isspace(ch) && ch && ch != '>') {
        *p++ = ch;
        ch = f01fd0();
    }
    *p = 0;
    while (ch != '>') {
        if (ch == '\n') {
            f0010e("missing '>'");
            return 0x16f;
        }
        ch = f01fd0();
    }
    f03a59();
    return f028ca(g_3270);
}
