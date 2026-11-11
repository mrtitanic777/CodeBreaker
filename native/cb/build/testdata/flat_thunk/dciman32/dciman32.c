/* dciman32.c -- 32-bit flat-thunk veneer. DllMain + per-export thunk-through.
 * SIGNATURES are recovered from each stub (stdcall arity via `ret N`; pointer args
 * via SMapLS). Per-function tags: documented/undocumented = published API; behavioral
 * = body reconstructed from observed behaviour (not byte-exact); scaffold = body not
 * yet implemented. Curation fills the marshalling and confirms doc/undoc + accuracy. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dciman.h>

extern BOOL __stdcall dciman32_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);

/* DLL entry -- connects the 32:16 thunk to the 16-bit target DCIMAN.DLL */
BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)
{
    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h);
        if (!dciman32_ThunkConnect32("DCIMAN.DLL", "DCIMAN32.DLL", h, r)) return FALSE; }
    else if (r == DLL_PROCESS_DETACH) dciman32_ThunkConnect32("DCIMAN.DLL", "DCIMAN32.DLL", h, r);
    return TRUE;
}

/* @1 DCIBeginAccess -- 5-arg, 2 ptr; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCIBeginAccess
DWORD WINAPI DCIBeginAccess(LPVOID p1, LPVOID p2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @2 DCICloseProvider -- 1-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCICloseProvider
DWORD WINAPI DCICloseProvider(DWORD a1) { return 0; }

/* @3 DCICreateOffscreen -- 10-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCICreateOffscreen
DWORD WINAPI DCICreateOffscreen(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7, DWORD a8, DWORD a9, DWORD a10) { return 0; }

/* @4 DCICreateOverlay -- 3-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCICreateOverlay
DWORD WINAPI DCICreateOverlay(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @5 DCICreatePrimary -- documented; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCICreatePrimary
int WINAPI DCICreatePrimary(HDC a1, LPDCISURFACEINFO FAR * a2) { return 0; }

/* @6 DCIDestroy -- 1-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCIDestroy
DWORD WINAPI DCIDestroy(DWORD a1) { return 0; }

/* @7 DCIDraw -- 1-arg, 2 ptr; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCIDraw
DWORD WINAPI DCIDraw(LPVOID p1) { return 0; }

/* @8 DCIEndAccess -- 1-arg, 2 ptr; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCIEndAccess
DWORD WINAPI DCIEndAccess(LPVOID p1) { return 0; }

/* @9 DCIEnum -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef DCIEnum
DWORD WINAPI DCIEnum(void) { return 0; }

/* @10 DCIOpenProvider -- 0-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCIOpenProvider
DWORD WINAPI DCIOpenProvider(void) { return 0; }

/* @11 DCISetClipList -- 2-arg, 4 ptr; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DCISetClipList
DWORD WINAPI DCISetClipList(LPVOID p1, LPVOID p2) { return 0; }

/* @12 DCISetDestination -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef DCISetDestination
DWORD WINAPI DCISetDestination(void) { return 0; }

/* @13 DCISetSrcDestClip -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef DCISetSrcDestClip
DWORD WINAPI DCISetSrcDestClip(void) { return 0; }

/* @14 DllEntryPoint -- 3-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef DllEntryPoint
DWORD WINAPI DllEntryPoint(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @15 GetDCRegionData -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef GetDCRegionData
DWORD WINAPI GetDCRegionData(void) { return 0; }

/* @16 GetWindowRegionData -- 3-arg, 1 ptr; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef GetWindowRegionData
DWORD WINAPI GetWindowRegionData(LPVOID p1, DWORD a2, DWORD a3) { return 0; }

/* @17 WinWatchClose -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef WinWatchClose
DWORD WINAPI WinWatchClose(void) { return 0; }

/* @18 WinWatchDidStatusChange -- 1-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef WinWatchDidStatusChange
DWORD WINAPI WinWatchDidStatusChange(DWORD a1) { return 0; }

/* @19 WinWatchGetClipList -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef WinWatchGetClipList
DWORD WINAPI WinWatchGetClipList(void) { return 0; }

/* @20 WinWatchNotify -- 3-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef WinWatchNotify
DWORD WINAPI WinWatchNotify(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @21 WinWatchOpen -- 1-arg; scaffold. TODO: marshal args, thunk to DCIMAN.DLL, return */
#undef WinWatchOpen
DWORD WINAPI WinWatchOpen(DWORD a1) { return 0; }

/* @22 dcithk_ThunkData32 -- thunk-data table (generated by the thunk compiler from dciman32.thk;
 * exported by name in the .def, no C definition here) */

