#include "port.h"
struct opt {	/* (as struct flagdef is: one structure, two names) */
    char *pat;
    int16_t *o2;
};
struct opt o;
int main(void)
{
	use(&o);
	return 0;
}
