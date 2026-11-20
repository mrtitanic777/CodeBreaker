extern f0018c();

f00002(fmt, args)
char *fmt;
{
    f0018c("Compiler error (internal):", fmt, &args);
    exit(1);
}
