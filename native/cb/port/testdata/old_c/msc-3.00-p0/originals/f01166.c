f01166(p)
register char **p;
{
    register char **q;

    while (p) {
        q = (char **)*p;
        free(p);
        p = q;
    }
}
