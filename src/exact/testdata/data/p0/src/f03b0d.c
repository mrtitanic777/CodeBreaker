char *
f03b0d(s, c)
register char *s;
char c;
{
    while (*s && c != *s)
        s++;
    return *s ? s : 0;
}
