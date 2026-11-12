#include "port.h"
#include "isym.h"
#define NEXT(s) (*(struct isym **)(s))
int16_t chain(struct isym *p, struct isym *q)
{
	NEXT(p) = q;
	return 0;
}
