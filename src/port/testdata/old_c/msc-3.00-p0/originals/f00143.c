extern f0018c();

f00143(fmt, args)
char *fmt;
{
    f0018c("FATAL : ", fmt, &args);
    exit(255);
}
