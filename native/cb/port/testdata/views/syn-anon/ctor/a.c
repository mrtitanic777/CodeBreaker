#include "rec.h"
struct a_p { int a; int b; };
fa(h)
struct rec_hd h;
{
	return h.kind;
}
fb(p)
register struct a_p *p;
{
	return p->a;
}
