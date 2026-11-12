#include "t.h"
struct lab {
	int a;
	char *b;
	int c;
};
f(s)
struct sym *s;
{
	struct lab *p;
	p = (struct lab *)s->type;
	return p->a + p->c;
}
