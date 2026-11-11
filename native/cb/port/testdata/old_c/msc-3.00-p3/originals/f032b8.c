extern f0308c(), f035eb();

f032b8(p, n)
register char *p;
int n;
{
    register char *e;

    e = p + n;
    do
        *p = f0308c();
    while (*p++);
    f035eb(p <= e);
}
