/* a label or segment an operand refers to */
struct targ {
    char t0, t1;
    char kind;                  /* (0, 1, 2) */
    char t3;
    int t4, t6, t8;
    int off;                    /* (+0ah: its place) */
    int tc;
    unsigned flags;             /* (+0eh: its class in bits 4-7, its frame in 8-11) */
    int t10;
    int t12;                    /* (+12h) */
    char t14, t15;
};
