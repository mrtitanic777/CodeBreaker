#include "s.h"
struct comsym {
	char head[4];
	int count;
};
struct rfld {
	int x;
	int y;
};
int NEAR bump(sym)
struct sym *sym;
{
	return ((struct comsym *)sym)->count;
}
g(us)
struct usym *us;
{
	struct rfld *r;
	char far *fp;
	r = (struct rfld *)&us->u.rfl;
	FP_SEG(fp) = 0x40;
	FP_OFF(fp) = 0x10;
	return r->x;
}
/* an empty stand-in */
