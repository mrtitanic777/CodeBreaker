extern int g_08e6;
extern long g_091a;
extern char *g_08e8;
extern f015b0(), f019b9(), f00143();

f01928()
{
    register int r;

    if (f019b9(0x10c)) {
        r = f015b0();
        if (!f019b9(0x10d)) {
            g_08e6 = 1;
            f00143("bad parenthesis nesting");
        }
        return r;
    }
    if (!f019b9(0x103)) {
        g_08e6 = 1;
        g_08e8 = "constant term expected";
    }
    return (int)g_091a;
}
