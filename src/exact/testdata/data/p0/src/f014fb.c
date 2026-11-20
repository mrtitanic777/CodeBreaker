extern char *g_082c;
extern int g_082a;
extern int (*g_09da)(), (*g_09dc)();
extern f01cd2(), f01d42(), f00f89();

extern char g_3974[];
#define EXEND   (g_3974 + 0x200)

f014fb()
{
    if (g_082c < EXEND)
        return *g_082c++;
    g_09da = f01cd2;
    g_09dc = f01d42;
    g_082a = 0;
    f00f89(4);
    return (*g_09da)();
}
