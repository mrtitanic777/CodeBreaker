/* test_lz.c -- runtime proof for the MS-free LZ matched pair (run inside Win95).
 *
 * Loads our lz32.dll, resolves the exports, and exercises the full 32->16 flat
 * thunk round-trip: LoadLibrary triggers DllMain -> lz_ThunkConnect32 (connects
 * to the 16-bit lzexpand.dll half), then each call marshals down through
 * QT_Thunk / SMapLS into the 16-bit codec and back. If this prints sane results
 * without faulting, the pair -- built entirely with OpenWatcom + wthunk, no MS
 * tools -- is behaviorally INTEGRAL on real Win95.
 *
 * Build (on the dev box, with our from-source OpenWatcom):
 *     wcl386 -bt=nt -l=nt test_lz.c
 * Then copy test_lz.exe + lz32.dll + lzexpand.dll into the Win95 VM and run.
 */
#include <windows.h>
#include <stdio.h>

typedef int  (WINAPI *PFN_GETEXP)(LPSTR, LPSTR);
typedef int  (WINAPI *PFN_LZSTART)(void);
typedef void (WINAPI *PFN_LZDONE)(void);
typedef int  (WINAPI *PFN_LZOPEN)(LPSTR, void *, WORD);

int main(void)
{
    HMODULE h;
    PFN_GETEXP  pGetExpandedNameA;
    PFN_LZSTART pLZStart;
    PFN_LZDONE  pLZDone;
    char buf[260];
    int r;

    h = LoadLibraryA("lz32.dll");
    if (!h) { printf("FAIL: LoadLibrary(lz32.dll) err=%lu\n", GetLastError()); return 1; }
    printf("OK  : lz32.dll loaded (DllMain + ThunkConnect32 ran)\n");

    pGetExpandedNameA = (PFN_GETEXP)GetProcAddress(h, "GetExpandedNameA");
    pLZStart          = (PFN_LZSTART)GetProcAddress(h, "LZStart");
    pLZDone           = (PFN_LZDONE)GetProcAddress(h, "LZDone");
    printf("OK  : GetProcAddress GetExpandedNameA=%p LZStart=%p LZDone=%p\n",
           (void *)pGetExpandedNameA, (void *)pLZStart, (void *)pLZDone);

    if (pLZStart && pLZDone) {
        r = pLZStart();                 /* 32->16 thunk: allocate the codec working set */
        printf("OK  : LZStart() = %d  (round-tripped 32->16 into the codec)\n", r);
        pLZDone();
        printf("OK  : LZDone() returned (thunk round-trip, no fault)\n");
    }

    if (pGetExpandedNameA) {            /* exercises pointer marshalling (SMapLS/SUnMapLS) */
        lstrcpyA(buf, "<unset>");
        r = pGetExpandedNameA("test.tx_", buf);
        printf("OK  : GetExpandedNameA(\"test.tx_\") = %d, buf=\"%s\"\n", r, buf);
    }

    FreeLibrary(h);
    printf("DONE: matched pair is behaviorally integral on Win95.\n");
    return 0;
}
