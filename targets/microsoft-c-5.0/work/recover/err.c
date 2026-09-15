extern int fprintf();
extern char *_NMSG_TEXT();
extern int exit();
extern int stderr;

err(n)
int n;
{
	fprintf(&stderr, _NMSG_TEXT(n));
	exit(1);
}
