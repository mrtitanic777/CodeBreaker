/* switches: -Ox */
/* HINT: 1, 2 */
#include <stdio.h>
extern int g;
extern int h; /* the other one */
int
f01000(a)
int a;
{
	return f01010(a) + f01020(a, a);
}

/* SAMEFILE */
int f01020(a, b)
{
}
