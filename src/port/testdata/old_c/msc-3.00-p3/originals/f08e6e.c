extern f08dfc();

/* the register (of the width w, not a segment register) known to hold the operand o: -1 if none */
f08e6e(o, w)
{
    register r;

    r = f08dfc(o, w);
    if (r >= 8 && r <= 11)
        return -1;
    return r;
}
