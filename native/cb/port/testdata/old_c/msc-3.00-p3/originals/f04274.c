extern f040fb();

/* the index n put in the record: one byte below 80h, else two (the first with 80h) */
f04274(n)
register n;
{
    if (n < 0x80)
        f040fb(n);
    else {
        f040fb(n >> 8 | 0x80);
        f040fb((unsigned char)n);
    }
}
