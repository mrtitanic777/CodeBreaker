#include <stdio.h>
extern f00143();

f00c14(fp)
FILE *fp;
{
    char tmp[512];
    long nw;

    fflush(fp);
    if (fp->_flag & 0x20) {
        *(int *)&nw = write(fp->_file, tmp, 512);     /* (the bytes: only the low word is stored) */
        if (nw != 512) {
            perror("error on compiler intermediate file");
            exit(255);
        } else
            f00143("error on compiler intermediate file");
    }
}
