extern char *g_0b3e[];          /* (the formats of the kinds of name) */
extern char g_0308;             /* (what a leading '.' becomes) */
extern f02cf4(), strlen();

/* the name of kind kind for a made in buf, sep after it (none: its last character dropped) */
char *
f036d0(buf, a, kind, sep)
char *buf;
char kind;
char *sep;
{
    char *p;

    f02cf4(buf, g_0b3e[kind], a, sep ? sep : "");
    p = buf + strlen(buf) - 1;
    if (!sep)
        *p = 0;
    if (*buf == '.')
        *buf = g_0308;
    return buf;
}
