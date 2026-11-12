#include "port.h"
#include "t.h"
struct lab {
	int16_t a;
	char *b;
	int16_t c;
};
int16_t f(struct sym *s)
{
	struct lab *p;
	p = (struct lab *)s->type;
	return p->a + p->c;
}
