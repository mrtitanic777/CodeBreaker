extern int g_08e6;
extern f0163f(), f019b9();

f015fc()
{
    register int r;

    r = f0163f();
    if (g_08e6)
        return 0;
    while (f019b9(0x132))
        r = f0163f() || r;
    return r;
}
