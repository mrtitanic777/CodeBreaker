#include <windows.h>

/* Minimal Win16 program: proves the OpenWatcom v2 toolchain we built from
   source compiles 16-bit segmented C and links it to a Win16 NE executable. */
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{
    (void)hInst; (void)hPrev; (void)lpCmd; (void)nShow;
    MessageBox(NULL, "premise proof", "OW16 NE", MB_OK);
    return 0;
}
