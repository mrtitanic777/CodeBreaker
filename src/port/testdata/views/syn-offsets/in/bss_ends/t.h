struct targ {
	int16_t kind;
	char *name;
	int16_t flags;
};
struct sym {
	struct targ *type;
	int16_t cls;
};
