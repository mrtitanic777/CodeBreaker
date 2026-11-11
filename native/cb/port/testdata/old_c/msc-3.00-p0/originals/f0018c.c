extern char *g_09d4;
extern int g_09d8;
extern f001db(), f0022c();

f0018c(prefix, fmt, args)
char *prefix, *fmt;
int *args;
{
    char buf[100];

    f001db("%s(%d) : ", g_09d4, g_09d8);
    if (prefix)
        f001db(prefix);
    f0022c(buf, fmt, args);
    f001db(buf);
    f001db("\n");
}
