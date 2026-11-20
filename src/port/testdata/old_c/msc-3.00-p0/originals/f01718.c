extern f01763(), f019b9();

f01718()
{
    register int r;

    r = f01763();
    if (f019b9(0x12d))
        return r == f01763();
    if (f019b9(0x12e))
        return r != f01763();
    return r;
}
