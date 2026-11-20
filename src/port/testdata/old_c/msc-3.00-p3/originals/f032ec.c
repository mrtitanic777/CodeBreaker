extern f0308c(), f0310f(), f035eb();

f032ec(p, n)
register char *p;
int n;
{
    register int len;
    int i;

    len = f0310f();
    f035eb(n >= len);
    for (i = 0; i < len; i++)
        *p++ = f0308c();
    return len;
}
