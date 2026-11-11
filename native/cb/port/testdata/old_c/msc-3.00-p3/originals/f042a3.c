extern f040fb(), f04127();

/* the length v put in the record: one byte, 81h and two, or 84h and three */
f042a3(v)
long v;
{
    if (v <= 0x7f)
        f040fb((char)v);
    else if (v <= 0xffffL) {
        f040fb(0x81);
        f040fb((char)v);
        f040fb((char)(v >> 8));
    } else {
        f040fb(0x84);
        f040fb((char)v);
        f04127((int)(v >> 8));
    }
}
