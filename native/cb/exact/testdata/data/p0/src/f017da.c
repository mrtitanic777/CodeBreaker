extern f01822(), f019b9();

f017da()
{
    register int r;

    r = f01822();
    if (f019b9(0x128))
        return r >> f01822();
    if (f019b9(0x127))
        return r << f01822();
    return r;
}
