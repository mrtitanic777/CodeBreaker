/* a symbol of the third pass */
struct psym {
    struct psym *s0;	/* (the chain's link: NEXT) */
    char kind;                  /* (+2) */
    char s3;
    int s4, s6, s8;
    long place;                 /* (+0ah: its place in its segment) */
    int se;
    int cls;                    /* (+10h: its class, 5 public) */
    int s12, s14;
    long l16;
    long l1a;
    char flags;                 /* (+1eh: 4 far) */
};
