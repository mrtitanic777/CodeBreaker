extern int g_0c90, g_0c8a, g_01a4;
extern f01775(), f02cbf(), f02c34();

f017cb(file, line)
char *file;
int line;
{
    g_0c90++;
    f01775(file, line);
    f02cbf("Compiler error (assertion): file %s, line %d source=%d\n", file, line, g_0c8a);
    if (++g_01a4 > 5)
        f02c34("Assertion count exceeds 5; stopping compilation");
}
