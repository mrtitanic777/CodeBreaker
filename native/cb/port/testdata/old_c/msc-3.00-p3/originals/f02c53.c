extern int g_0c98;
extern f02c73();

f02c53(level, fmt, args)
int level;
char *fmt;
{
    if (level <= g_0c98)
        f02c73("warning : ", fmt, &args);
}
