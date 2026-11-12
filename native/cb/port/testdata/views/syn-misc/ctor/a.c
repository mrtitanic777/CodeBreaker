#include "s.h"
struct comsym {
	char head[10];
	int count;
};
struct rfld { int a_old; int b_old; };
int bump(sym)
struct sym *sym;
{
	return ((struct comsym *)sym)->count;
}
g(us)
struct usym *us;
{
	struct rfld *r;
	char *fp;
	r = (struct rfld *)&us->u.rfl;
	fp = dos_far(0x40, 0x10);
	return r->x;
}
/* an empty stand-in */
