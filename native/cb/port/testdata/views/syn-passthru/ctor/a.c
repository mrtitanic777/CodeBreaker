#include "t.h"
relay(p)
struct a *p;
{
	return use(p);
}
top()
{
	struct b *q;
	return relay(q);
}
