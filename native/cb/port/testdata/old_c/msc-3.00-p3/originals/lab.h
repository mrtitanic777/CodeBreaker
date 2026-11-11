/* a reference to a label: the next, the item that jumps */
struct ref {
    struct ref *next;
    struct ins *ins;
};
/* a label (its symbol): the item that places it, the jumps to it */
struct lab {
    int l0, l2, l4, l6, l8;
    long place;                 /* (+0ah) */
    int le;
    struct ref *refs;           /* (+10h) */
    struct ins *def;            /* (+12h) */
};
#define LNEXT(p)    (*(struct ins **)&(p)->size)        /* (a label item's next label) */
extern struct ref *g_2fb8;      /* (the free references) */
extern struct ins *g_3160;      /* (the function's first label) */
