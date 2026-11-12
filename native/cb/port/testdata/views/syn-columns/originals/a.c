struct ent {
	char *name;
	int *val;
};
char *buf;
int cnt;
struct ent tab[] = {
	{ "a", (int *)&buf },
	{ "b", &cnt },
};
