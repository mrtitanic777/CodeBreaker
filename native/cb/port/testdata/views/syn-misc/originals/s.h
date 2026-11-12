struct sym {
	char *name;
	int kind;
	char *link;
};
struct usym {
	int kind;
	union {
		struct { int a_old; int b_old; } rfl;
		char c;
	} u;
};
#define NEAR near pascal
