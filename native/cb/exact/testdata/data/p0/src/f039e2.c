extern f0391c(), f01fd0();

/* a comment begins at c: skipped */
f039e2(c)
register int c;
{
    register int d;

    if (c != '/' || !f0391c('*'))
        return 0;
    while (!((d = f01fd0()) == '*' && f0391c('/')))
        ;
    return 1;
}
