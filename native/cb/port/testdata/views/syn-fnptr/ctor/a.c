/* a */
char *mk(n)
int n;
{
	return 0;
}
run(fn, k)
char *(*fn)();
int k;
{
	return (*fn)(k) != 0;
}
main()
{
	run(mk, 3);
	return 0;
}
