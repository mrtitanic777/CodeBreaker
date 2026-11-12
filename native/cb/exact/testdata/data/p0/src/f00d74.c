extern int g_3770;
extern char **g_3b74;

char *
f00d74()
{
    if (--g_3770 > 0)
        return *++g_3b74;
    return 0;
}
