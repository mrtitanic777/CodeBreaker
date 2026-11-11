extern int g_1178;              /* (the words of a long the other way) */
extern f04127();

/* the long a:b put in the record */
f0414b(a, b)
{
    if (g_1178) {
        f04127(a);
        f04127(b);
    } else {
        f04127(b);
        f04127(a);
    }
}
