#include "isym.h"
#define NEXT(s) (*(struct isym **)(s))
chain(p, q)
struct isym *p, *q;
{
	NEXT(p) = q;
	return 0;
}
