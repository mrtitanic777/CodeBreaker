/* the first character of s that is in set, 0 if none */
char *
f04529(s, set)
register char *s;
char *set;
{
    register char *p;

    for (; *s; s++)
        for (p = set; *p; p++)
            if (*s == *p)
                return s;
    return 0;
}
