f03710(l)
long l;
{
    return (l &= 0xffff8000L) ? 0x104 : 0x103;
}
