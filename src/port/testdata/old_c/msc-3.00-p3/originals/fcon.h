/* a floating point constant loaded into a variable at the function's head: the variable (its symbol), the
   constant (its symbol) */
struct fcon {
    struct fcon *next;
    int var;
    int con;
};
extern struct fcon *g_2008;     /* (the constants loaded) */
extern struct fcon *g_200a;     /* (the same constants' other variables) */
