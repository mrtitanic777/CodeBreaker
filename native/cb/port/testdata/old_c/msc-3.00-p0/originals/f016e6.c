extern int g_08e6;
extern f01718(), f019b9();

f016e6()
{
    register int r;

    r = f01718();
    while (f019b9(0x121) && !g_08e6)
        r &= f01718();
    return r;
}
