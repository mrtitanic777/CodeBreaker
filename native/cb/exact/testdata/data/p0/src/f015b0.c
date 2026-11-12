extern f015fc(), f019b9();

f015b0()
{
    register int c, a;
    int b;

    c = f015fc();
    if (f019b9(0x133)) {
        a = f015fc();
        if (f019b9(0x134))
            b = f015fc();
        return c ? a : b;
    }
    return c;
}
