extern char g_2eac[];
extern char g_3af2[];           /* (the suffix of a segment's name) */
extern char *f0475f();
extern strcpy(), strcat();

/* the name of the symbol s with the suffix */
char *
f04316(s)
register s;
{
    strcpy(g_2eac, f0475f(s));
    strcat(g_2eac, g_3af2);
    return g_2eac;
}
