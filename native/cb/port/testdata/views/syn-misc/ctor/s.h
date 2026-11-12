struct sym {
	char *name;
	int kind;
	char *link;
};
struct usym {
	int kind;
	union {
		struct s_rfl { int a_old; int b_old; } rfl;
		char c;
	} u;
};
