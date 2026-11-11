/* getflags.c: one switch of a table of switches (flags.c) */
struct flagdef {
    char *pat;                  /* the switch's text: # an argument, * a pattern, - either - or / */
    int *var;
    char val;                   /* what the match returns */
    unsigned char flags;        /* 0: the table's end */
};
/* one letter of a switch of letters (kind C) */
struct sub {
    int c;
    int type;
    int *var;
};
/* a list of names a switch gathers: its count of free places, then the places (filled from the end) */
struct slots {
    int top;
    char *v[20];
};
