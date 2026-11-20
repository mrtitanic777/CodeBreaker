#include "port.h"
#include "mac.h"
struct macro {	/* (as struct mac is: its bytes) */
    struct mac *f0;
    char *f2;
    int16_t f4;
};
int16_t use(void)
{
	struct macro *m;
	m = getmac();
	return m->f0;
}
