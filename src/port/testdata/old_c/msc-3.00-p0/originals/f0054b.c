f0054b(s, c)
register char *s;
char c;
{
    if (s)
        while (*s)
            if (*s++ == c)
                return 1;
    return 0;
}
