#include "port.h"
#include "s.h"
struct comsym {
	char head[10];
	int16_t count;
};
struct rfld { int16_t a_old; int16_t b_old; };
int16_t bump(struct sym *sym)
{
	return ((struct comsym *)sym)->count;
}
int16_t g(struct usym *us)
{
	struct rfld *r;
	char *fp;
	r = (struct rfld *)&us->u.rfl;
	fp = dos_far(0x40, 0x10);
	return r->x;
}
/* an empty stand-in */
