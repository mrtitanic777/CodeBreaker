extern f017da(), f019b9();

f01763()
{
    register int r;

    r = f017da();
    if (f019b9(0x129))
        return r < f017da();
    if (f019b9(0x12b))
        return r > f017da();
    if (f019b9(0x12a))
        return r <= f017da();
    if (f019b9(0x12c))
        return r >= f017da();
    return r;
}
