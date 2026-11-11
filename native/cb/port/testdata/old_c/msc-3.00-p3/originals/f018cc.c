extern printf();

/* a static name's alias in the listing */
f018cc(a, b)
char *a, *b;
{
    printf(";\t%s\tEQU\t%s\n", a, b);
}
