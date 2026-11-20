/* what a token carries (P0's scanner) */
union tokval {
    int i;
    char *s;
    long l;
    double d;
    struct tokval_str {
        char *s;
        int n;
        int f4;
    } str;
    struct tokval_sl {                    /* the same, its length as a long (the scanner's view) */
        char *s;
        long len;
    } sl;
};
