extern int g_08e6;
extern f016e6(), f019b9();

f016b4()
{
    register int r;

    r = f016e6();
    while (f019b9(0x12f) && !g_08e6)
        r ^= f016e6();
    return r;
}
