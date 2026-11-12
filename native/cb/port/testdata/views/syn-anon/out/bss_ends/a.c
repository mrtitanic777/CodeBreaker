#include "port.h"
#include "rec.h"
struct a_p { int16_t a; int16_t b; };
int16_t fa(struct rec_hd h)
{
	return h.kind;
}
int16_t fb(register struct a_p *p)
{
	return p->a;
}
