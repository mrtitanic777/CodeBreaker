extern int g_08e6;
extern f01869(), f019b9();

f01822()
{
    register int r;

    r = f01869();
    while (!g_08e6)
        if (f019b9(0x124))
            r += f01869();
        else if (f019b9(0x125))
            r -= f01869();
        else
            break;
    return r;
}
