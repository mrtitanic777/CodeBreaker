struct stk {
	int top;
	char *v[20];
};
int top = 20;
char *v[20];
push(p)
char *p;
{
	char **q;
	q = &top;
	v[0] = p;
	return 0;
}
