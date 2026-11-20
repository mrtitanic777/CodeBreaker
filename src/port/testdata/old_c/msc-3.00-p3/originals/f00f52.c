/* the size of the displacement the ModR/M byte m takes */
f00f52(m)
unsigned char m;
{
    switch (m & 0xc0) {
    case 0:
        if ((m & 7) != 6)
            return 0;
    case 0x80:
        return 2;
    case 0xc0:
        return 0;
    case 0x40:
        return 1;
    }
}
