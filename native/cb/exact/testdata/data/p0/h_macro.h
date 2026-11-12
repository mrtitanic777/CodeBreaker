/* p0prepro.c: the macros, in 47 chains by the hash of their names */
struct btok {                   /* a piece of a macro's body */
    struct btok *next;
    char *text;
    int kind;                   /* 0x17A: a parameter (argno), 0x108: text */
    char argno;
};
struct mac {
    struct mac *next;
    char *name;
    int hash;
    struct btok *body;
    char nparams;               /* -1: none given */
};
/* a macro's actual argument, in a list (p0expand.c) */
struct arg {
    struct arg *next;
    char *text;
};
