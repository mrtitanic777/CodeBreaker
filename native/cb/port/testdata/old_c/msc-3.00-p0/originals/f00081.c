extern int g_09d6, g_09d8, g_00fe;
extern f00026(), f001db(), f00143();

f00081(file, line)
char *file;
int line;
{
    g_09d6++;
    f00026(file, line);
    f001db("Compiler error (assertion): file %s, line %d source=%d\n", file, line, g_09d8);
    if (++g_00fe > 5)
        f00143("Assertion count exceeds 5; stopping compilation");
}
