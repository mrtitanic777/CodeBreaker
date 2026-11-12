/* a record the program writes to its own file as bytes, and one it never does */
#ifndef PROG_H
#define PROG_H
#include <stdint.h>
#include <stddef.h>

struct sym;

struct expr {
	int16_t e_op;
	uint16_t e_sym;	/* (a handle: struct sym *) */
	uint16_t e_left;	/* (a handle: struct expr *) */ uint16_t e_right;	/* (a handle: struct expr *) */
	int16_t e_word;	/* (one word, as it was) */
	union {
		long e_v;
		uint16_t e_p;	/* (a handle: struct sym *) */
	} e_u;
};

struct sym {
	int s_num;
	struct sym *s_next;
};

struct plain {
	struct plain *p_next;
	int p_val;
};

#define left_of(e) ((e)->e_left)
extern int write(int, const void *, unsigned);
extern int read(int, void *, unsigned);
extern void dos_use(struct sym *s);
extern struct sym *lookup(int n);
extern struct expr *newexpr(void);
#endif
