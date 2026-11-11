/* a segment of the listing */
struct seg {
    struct seg *next;
    char *name;
    char *cls;                  /* its class */
    int *grp;                   /* (+6: its group) */
    int num;                    /* its number */
    int size;                   /* (+0ah: its size so far) */
    int align;                  /* (+0ch) */
    int combine;                /* (+0eh) */
    long len;                   /* (+10h: its length) */
    int nameidx;                /* (+14h: its name's index, its class's) */
    int clsidx;
};
extern struct seg *g_11b8;      /* (the segments) */
extern struct seg *g_03d8;      /* (the segment the listing is in) */
