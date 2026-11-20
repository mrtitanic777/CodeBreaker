extern f02c73();

f02c34(fmt, args)
char *fmt;
{
    f02c73("FATAL : ", fmt, &args);
    exit(255);
}
