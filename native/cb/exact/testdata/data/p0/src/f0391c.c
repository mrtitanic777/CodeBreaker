extern int (*g_09da)(), (*g_09dc)();

/* the next character is want: taken; otherwise put back */
f0391c(want)
register int want;
{
    register int c;

    if ((c = (*g_09da)()) == want)
        return 1;
    (*g_09dc)(c);
    return 0;
}
