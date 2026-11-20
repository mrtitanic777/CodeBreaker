extern f01739();

/* the size of a displacement v: 1 when it fits in a byte (or kind 1), else 2 */
f01717(kind, v)
long v;
{
    return kind == 1 || f01739((int)v) ? 1 : 2;
}
