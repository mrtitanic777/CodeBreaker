extern int g_08e6;
extern f018d1(), f019b9();

f01869()
{
    register int r;

    r = f018d1();
    while (!g_08e6)
        if (f019b9(0x11a))
            r *= f018d1();
        else if (f019b9(0x122))
            r /= f018d1();
        else if (f019b9(0x123))
            r %= f018d1();
        else
            break;
    return r;
}
