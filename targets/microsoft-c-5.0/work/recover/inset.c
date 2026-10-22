char set[] = "0123456789";

int inset(c)
char c;
{
	register char *p;
	for (p = set; *p; p++)
		if (*p == c)
			return 1;
	return 0;
}
