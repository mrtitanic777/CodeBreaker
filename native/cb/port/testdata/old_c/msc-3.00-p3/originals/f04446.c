extern char *g_0c64;            /* (the processor options given) */
extern char g_3a0a[];
extern int g_117e, g_117c, g_117a;      /* (the memory model: large, medium, small) */
extern int g_1180;              /* (its letter) */
extern strcpy();

/* the memory model from the options s: l far code, f or h far data */
f04446(s)
char *s;
{
    int data;
    int code;
    int u, v;                   /* (not used) */

    code = 2;
    data = 2;
    if (g_0c64)
        strcpy(g_3a0a, g_0c64);
    if (s)
        while (*s)
            switch (*s++) {
            case 'l':
                code = 4;
                break;
            case 'f':
            case 'h':
                data = 4;
                break;
            case 'a':
            case 'c':
            case 'm':
            case 's':
            case 'w':
                break;
            }
    if (code == 4)
        if (data == 4) {
            g_117e = 1;
            g_117c = 0;
            g_117a = 0;
            g_1180 = 'L';
        } else {
            g_117e = 0;
            g_117c = 1;
            g_117a = 0;
            g_1180 = 'M';
        }
    else {
        g_117e = 0;
        g_117c = 0;
        g_117a = 1;
        g_1180 = 'S';
    }
}
