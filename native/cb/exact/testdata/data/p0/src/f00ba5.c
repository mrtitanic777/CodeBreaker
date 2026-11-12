#include <stdio.h>
extern char *g_0230;
extern f00081();

FILE *
f00ba5(dir, name, mode)
char *dir, *name, *mode;
{
    char path[64];
    FILE *fp;

    strcpy(path, dir);
    strcat(path, name);
    if (strlen(path) > 64)
        f00081(g_0230, 382);
    if ((fp = fopen(path, mode)) == 0) {
        perror("error on compiler intermediate file");
        exit(1);
    }
    return fp;
}
