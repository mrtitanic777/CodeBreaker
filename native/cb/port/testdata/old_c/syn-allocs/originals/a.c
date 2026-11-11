#include "n.h"
char *pool_alloc()
{
    return malloc(36);
}
void g()
{
    struct node *p;
    p = (struct node *)pool_alloc();
}
