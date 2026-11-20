#include "n.h"
char *two_alloc()
{
    char *p;
    p = calloc(0x28);
    return p;
}
char *third_alloc()
{
    return malloc(10);
}
void h()
{
    struct node *a;
    struct cell *b;
    a = (struct node *)two_alloc();
    b = (struct cell *)two_alloc();
    a = (struct node *)third_alloc();
}
