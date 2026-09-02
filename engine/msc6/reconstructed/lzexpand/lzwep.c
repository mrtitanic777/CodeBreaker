/*
**  lzwep.c  (segment 3)  -  DLL lifecycle: the load-time entry (LibMain, called
**  by the runtime once the data segment is in place) and the exit procedure (WEP).
**  Neither has per-instance work to do, so both simply succeed.
*/
#include "lzexp.h"

int far pascal LibMain(HANDLE hInst, WORD wDataSeg, WORD wHeapSize, LPSTR lpCmdLine)
{
    return 1;
}

int far pascal WEP(int nParam)
{
    return 1;
}
