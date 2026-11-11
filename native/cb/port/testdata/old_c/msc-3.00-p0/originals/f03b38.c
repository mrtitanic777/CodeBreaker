char *
f03b38(s, c)
register char *s;
char c;
{
    register char *last;

    last = 0;
    while (*s) {
        if (c == *s)
            last = s;
        s++;
    }
    return last;
}
