#include <ctype.h>
extern int (*g_09dc)();
extern char *f0330d();
extern f03a28(), f0010e(), f03a59(), f021ac();

f031f7()
{
    register int c;
    char *id;

    c = f03a28();
    if (!isalpha(c)) {
        if (c == '\n')
            (*g_09dc)(c);
        else {
            f0010e("expected preprocessor command, found `%c'", c);
            f03a59();
        }
        return 0x16f;
    }
    id = f0330d(c);
    f021ac(id);
    return 0x16b;
}
