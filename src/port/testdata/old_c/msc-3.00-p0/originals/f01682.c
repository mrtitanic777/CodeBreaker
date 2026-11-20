extern int g_08e6;
extern f016b4(), f019b9();

f01682()
{
    register int r;

    r = f016b4();
    while (f019b9(0x130) && !g_08e6)
        r |= f016b4();
    return r;
}
