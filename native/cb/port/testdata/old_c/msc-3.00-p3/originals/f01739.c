/* 1 when v fits in a signed byte */
f01739(v)
register v;
{
    return v >= -128 && v <= 127;
}
