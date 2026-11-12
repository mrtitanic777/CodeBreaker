#include "port.h"
#include "t.h"
int16_t relay(struct b *p)
{
	return use(p);
}
int16_t top(void)
{
	struct b *q;
	return relay(q);
}
