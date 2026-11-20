struct stk {
	int top;
	char *v[20];
};
struct stk top_s = { 20, { 0 } };	/* (top, v: one place, as the old linker kept them) */

push(p)
char *p;
{
	char **q;
	q = &top;
	v[0] = p;
	return 0;
}
