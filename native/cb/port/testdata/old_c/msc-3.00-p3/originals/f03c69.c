extern fgetc();

/* the next character of the source file f (a new line at its end) */
f03c69(f)
{
    register c;

    c = fgetc(f);
    return c <= 0 ? '\n' : c;
}
