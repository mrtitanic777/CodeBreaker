#include "prog.h"

static struct plain pl;
static struct expr *cur;
static intptr_t g_word;

void later();   /* declared with no parameters, defined below: its calls are not checked against it */

/* an old-style function the port kept: its first parameter is a pointer */
void take(s, n)
	struct sym *s;
	int n;
{
	s->s_num = n;
}

int save(struct expr *e, int fd)
{
	char *p = (char *)e;
	struct expr copy;
	(void)p;
	write(fd, (char *)&copy, sizeof(struct expr));
	write(fd, (char *)&e->e_u, sizeof(e->e_u));
	return read(fd, e, sizeof(struct expr));
}

struct sym *find(struct expr *e)
{
	struct sym *r, *t;
	r = e->e_sym;
	e->e_sym = lookup(3);
	e->e_sym = r;
	if ((t = e->e_left->e_sym))
		return t;
	while ((t = e->e_right->e_sym) != 0)
		e = e->e_right;
	e->e_left = e->e_right = 0;
	pl.p_next = 0;
	return e->e_u.e_p;
}

void words(struct expr *e)
{
	struct sym *s = lookup(1);
	e->e_word = (intptr_t)s;
	g_word = e->e_word;
	take(e->e_sym, 4);
	take((struct sym *)e->e_word, 5);
	take(e->e_word, 6);
	later(e->e_word);
	dos_use(e->e_sym);
}

void taken(struct expr *e)
{
	struct sym **pp = &e->e_sym;
	e->e_left++;
	(void)pp;
	cur = left_of(e);
}

/* an old-style function the port kept: its parameter is a pointer */
void later(s)
	struct sym *s;
{
	s->s_num = 0;
}
