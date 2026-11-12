extern int g_05de;
extern f0018c();

f00167(level, fmt, args)
int level;
char *fmt;
{
    if (level <= g_05de)
        f0018c("warning : ", fmt, &args);
}
