#include <stdio.h>
extern char *g_0950;
extern f017cb();

FILE *
f03606(dir, name, mode)
char *dir, *name, *mode;
{
    char path[64];
    FILE *fp;

    strcpy(path, dir);
    strcat(path, name);
    if (strlen(path) > 64)
        f017cb(g_0950, 382);
    if ((fp = fopen(path, mode)) == 0) {
        perror("error on compiler intermediate file");
        exit(1);
    }
    return fp;
}
