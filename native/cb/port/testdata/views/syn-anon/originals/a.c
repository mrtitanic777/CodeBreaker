#include "rec.h"
fa(h)
struct { char kind, flags; } h;
{
	return h.kind;
}
fb(p)
register struct { int a; int b; } *p;
{
	return p->a;
}
