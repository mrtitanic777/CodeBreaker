extern f005e4(), f00b85();

f00824(p, n)
register char *p;
int n;
{
    register char *e;

    e = p + n;
    do
        *p = f005e4();
    while (*p++);
    f00b85(p <= e);
}
