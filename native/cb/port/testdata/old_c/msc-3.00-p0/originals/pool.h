/* memallo.c: the passes' storage pools - blocks chained by their first word */
struct pool {
    char **base;                /* the first block */
    int size;                   /* a block's size, its link aside */
    char *ptr;                  /* where the next piece goes */
    char **cur;                 /* the block it is in */
    int left;                   /* what is left of it */
    int nblocks;
};
