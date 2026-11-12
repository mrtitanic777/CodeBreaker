#include "mac.h"
struct macro {
	int f0;
	int f2;
	int f4;
};
struct macro *getmac();
use()
{
	struct macro *m;
	m = getmac();
	return m->f0;
}
