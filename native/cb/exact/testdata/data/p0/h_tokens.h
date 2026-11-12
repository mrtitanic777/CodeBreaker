/* tokens.c: a token's name (for the messages), its number, its kind */
struct tokname {
    char *name;
    int tok;
    int kind;
};
extern struct tokname g_123a[];        /* (from token 0x101 on) */
