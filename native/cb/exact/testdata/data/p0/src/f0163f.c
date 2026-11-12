extern int g_08e6;
extern f01682(), f019b9();

f0163f()
{
    register int r;

    r = f01682();
    if (g_08e6)
        return 0;
    while (f019b9(0x131))
        r = f01682() && r;
    return r;
}
