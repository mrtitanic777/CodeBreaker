struct targ {
	int kind;
	char *name;
	int flags;
};
struct sym {
	struct targ *type;
	int cls;
};
