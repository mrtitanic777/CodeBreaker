#include "node.h"
walk(p, i)
struct pnode *p;
int i;
{
	return p->kid[i] != 0;
}
