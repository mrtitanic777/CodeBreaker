/* the string s made upper case */
f046bf(s)
register char *s;
{
    for (; *s; s++)
        if (*s >= 'a' && *s <= 'z')
            *s = (char)(*s - 0x20);
}
