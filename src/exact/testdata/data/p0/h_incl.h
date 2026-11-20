/* p0io.c: the stack of files being read (#include) */
struct incl {
    FILE *fp;
    long line;
    char *name;
    char *iname;
};
