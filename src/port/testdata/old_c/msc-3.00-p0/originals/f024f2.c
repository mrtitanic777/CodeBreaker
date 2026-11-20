extern int (*g_09da)(), (*g_09dc)();
extern char *f0330d();
extern f03a59(), f03a28(), f00c74(), f00143();

/* skip the lines of a false #if part: 2 at an #elif, 3 at an #else, 4 at its #endif; how 6: an #else is wrong here */
f024f2(how)
int how;
{
    register int c;
    register char *id;
    int kw;
    int level;

    level = 0;
    do {
        for (;;) {
            f03a59();
            (*g_09da)();
            if ((c = f03a28()) == '#')
                break;
            if (c == '\n')
                (*g_09dc)(c);
        }
        c = f03a28();
        id = f0330d(c);
        *id = '#';
        kw = f00c74(id);
        if (kw == 0x175 || kw == 0x176 || kw == 0x158)
            level++;
        else if (level == 0 && (kw == 0x153 || kw == 0x177)) {
            if (how == 6)
                f00143("expected `#endif'");
            else
                return kw == 0x177 ? 2 : 3;
        } else if (kw == 0x178)
            level--;
    } while (level >= 0);
    return 4;
}
