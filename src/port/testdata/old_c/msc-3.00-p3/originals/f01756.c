extern f02c73();

f01756(fmt, args)
char *fmt;
{
    f02c73("Compiler error (internal):", fmt, &args);
    exit(1);
}
