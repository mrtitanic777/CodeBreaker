extern int g_0c90, g_0c8a;
extern f01775(), f02cbf();

f0180b(expr, file, line)
char *expr, *file;
int line;
{
    g_0c90++;
    f01775(file, line);
    f02cbf("Compiler error (assertion : %s): file %s, line %d source=%d\n", expr, file, line, g_0c8a);
}
