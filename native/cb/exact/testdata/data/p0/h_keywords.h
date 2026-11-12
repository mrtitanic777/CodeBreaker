/* keywords.c: a keyword (or a # word) of the table P0 looks names up in: its chain, the name, its hash, its token */
struct sym {
    struct sym *next;
    char *name;
    int hash;
    int val;
};
