/* A label that is jumped to from before it and from after it does not nest as Java's labelled break and continue: the
   function is written as a signature and a body that throws, listed in the notes; the rest of the program is written as
   usual. */
static int tangle(int n) {
    int r = 0;
    if (n > 3) goto mid;
    r = 1;
mid:
    r++;
    if (r < n) goto mid;
    return r;
}

static int plain(int a) { return a + 1; }

int main(void) { return plain(2) + (tangle(0) > 100 ? 1 : 0); }
