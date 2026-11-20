extern int g_3a20[64];          /* (the global symbols, hashed) */

/* the global symbol table emptied */
f06797()
{
    register i;

    for (i = 0; i < 64; i++)
        g_3a20[i] = 0;
}
