extern char *g_082c;
extern f00143();

extern char g_3974[];          /* the expansion buffer (512): text is pushed back from its end */
#define EXBUF   g_3974

f014bb(s)
register char *s;
{
    register char *p;

    p = g_082c - strlen(s);
    if (p < EXBUF)
        f00143("compiler limit : macro expansion too big");
    g_082c = p;
    while (*s)
        *p++ = *s++;
}
