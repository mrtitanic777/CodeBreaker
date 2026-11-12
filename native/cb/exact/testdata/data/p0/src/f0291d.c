extern char g_2e42[];

/* the file name n in the directory d: d, a '/' if it has none at its end, n (in g_2e42) */
char *
f0291d(d, n)
register char *d, *n;
{
    char *p;

    if (!*d)
        return n;
    p = g_2e42;
    if (!strcmp(d, "./"))
        return n;
    while (*p++ = *d++)
        ;
    if (d[-2] != ':' && d[-2] != '/')
        p[-1] = '/';
    else
        p--;
    while (*p++ = *n++)
        ;
    return g_2e42;
}
