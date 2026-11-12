#include "mac.h"
struct macro {	/* (as struct mac is: its bytes) */
    struct mac *f0;
    char *f2;
    int f4;
};
struct macro *getmac();
use()
{
	struct macro *m;
	m = getmac();
	return m->f0;
}
