#include "port.h"
struct ent {
	char *name;
	intptr_t *val;	/* (a word's place - a pointer's in some rows) */
};
char *buf;
intptr_t cnt;
struct ent tab[] = {
	{ "a", (intptr_t *)&buf },
	{ "b", &cnt },
};
