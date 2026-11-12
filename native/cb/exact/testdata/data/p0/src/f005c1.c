f005c1(s)
register char *s;
{
    register int h;

    h = 0;
    while (*s)
        h += *s++;
    return h;
}
