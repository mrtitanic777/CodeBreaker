extern int g_09d6, g_09d8;
extern f00026(), f001db();

f000c6(expr, file, line)
char *expr, *file;
int line;
{
    g_09d6++;
    f00026(file, line);
    f001db("Compiler error (assertion : %s): file %s, line %d source=%d\n", expr, file, line, g_09d8);
}
