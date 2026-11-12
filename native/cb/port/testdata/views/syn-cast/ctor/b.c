struct opt {	/* (as struct flagdef is: one structure, two names) */
    char *pat;
    int *o2;
};
struct opt o;
main()
{
	use(&o);
	return 0;
}
