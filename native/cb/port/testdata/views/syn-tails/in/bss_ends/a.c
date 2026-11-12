#include "port.h"
#include "node.h"
int16_t walk(struct pnode *p, int16_t i)
{
	return p->kid[i] != 0;
}
