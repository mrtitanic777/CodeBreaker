extern int g_118a;              /* (the names so far) */
extern f048bd(), f041e1();

/* the name in the LNAMES record: its index */
f04b4f(name)
char *name;
{
    f048bd(0x96, 0);
    f041e1(name);
    return g_118a++;
}
