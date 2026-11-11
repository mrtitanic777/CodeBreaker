extern int (*g_09dc)();
extern f01fd0(), f039e2();

/* the rest of the line skipped, the newline put back */
f03a59()
{
    register int c;

    do
        c = f01fd0();
    while (f039e2(c) || c != '\n');
    (*g_09dc)(c);
}
