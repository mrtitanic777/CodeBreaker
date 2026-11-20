struct sym {
	char *name;
	int16_t kind;
	char *link;
};
struct usym {
	int16_t kind;
	union {
		struct s_rfl { int16_t a_old; int16_t b_old; } rfl;
		char c;
	} u;
};
