/* mpr.c -- 32-bit flat-thunk veneer. DllMain + per-export thunk-through.
 * SIGNATURES are recovered from each stub (stdcall arity via `ret N`; pointer args
 * via SMapLS). Per-function tags: documented/undocumented = published API; behavioral
 * = body reconstructed from observed behaviour (not byte-exact); scaffold = body not
 * yet implemented. Curation fills the marshalling and confirms doc/undoc + accuracy. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winnetwk.h>
#include <mmsystem.h>

extern BOOL __stdcall mpr_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);

/* DLL entry -- connects the 32:16 thunk to the 16-bit target MPR.DLL */
BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)
{
    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h);
        if (!mpr_ThunkConnect32("MPR.DLL", "MPR.DLL", h, r)) return FALSE; }
    else if (r == DLL_PROCESS_DETACH) mpr_ThunkConnect32("MPR.DLL", "MPR.DLL", h, r);
    return TRUE;
}

/* @10 DllCanUnloadNow -- 0-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef DllCanUnloadNow
DWORD WINAPI DllCanUnloadNow(void) { return 0; }

/* @11 DllGetClassObject -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef DllGetClassObject
DWORD WINAPI DllGetClassObject(DWORD a1) { return 0; }

/* @26 MultinetGetConnectionPerformanceA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef MultinetGetConnectionPerformanceA
DWORD WINAPI MultinetGetConnectionPerformanceA(DWORD a1, DWORD a2) { return 0; }

/* @27 MultinetGetConnectionPerformanceW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef MultinetGetConnectionPerformanceW
DWORD WINAPI MultinetGetConnectionPerformanceW(DWORD a1, DWORD a2) { return 0; }

/* @28 MultinetGetErrorTextA -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef MultinetGetErrorTextA
DWORD WINAPI MultinetGetErrorTextA(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @29 MultinetGetErrorTextW -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef MultinetGetErrorTextW
DWORD WINAPI MultinetGetErrorTextW(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @30 NPSAuthenticationDialogA -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSAuthenticationDialogA
DWORD WINAPI NPSAuthenticationDialogA(DWORD a1) { return 0; }

/* @31 NPSCopyStringA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSCopyStringA
DWORD WINAPI NPSCopyStringA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @32 NPSDeviceGetNumberA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSDeviceGetNumberA
DWORD WINAPI NPSDeviceGetNumberA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @33 NPSDeviceGetStringA -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSDeviceGetStringA
DWORD WINAPI NPSDeviceGetStringA(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @34 NPSGetProviderHandleA -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSGetProviderHandleA
DWORD WINAPI NPSGetProviderHandleA(DWORD a1) { return 0; }

/* @35 NPSGetProviderNameA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSGetProviderNameA
DWORD WINAPI NPSGetProviderNameA(DWORD a1, DWORD a2) { return 0; }

/* @36 NPSGetSectionNameA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSGetSectionNameA
DWORD WINAPI NPSGetSectionNameA(DWORD a1, DWORD a2) { return 0; }

/* @37 NPSNotifyGetContextA -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSNotifyGetContextA
DWORD WINAPI NPSNotifyGetContextA(DWORD a1) { return 0; }

/* @38 NPSNotifyRegisterA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSNotifyRegisterA
DWORD WINAPI NPSNotifyRegisterA(DWORD a1, DWORD a2) { return 0; }

/* @39 NPSSetCustomTextA -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSSetCustomTextA
DWORD WINAPI NPSSetCustomTextA(DWORD a1) { return 0; }

/* @40 NPSSetExtendedErrorA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef NPSSetExtendedErrorA
DWORD WINAPI NPSSetExtendedErrorA(DWORD a1, DWORD a2) { return 0; }

/* @41 PwdChangePasswordA -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef PwdChangePasswordA
DWORD WINAPI PwdChangePasswordA(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @42 PwdChangePasswordW -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef PwdChangePasswordW
DWORD WINAPI PwdChangePasswordW(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @43 PwdGetPasswordStatusA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef PwdGetPasswordStatusA
DWORD WINAPI PwdGetPasswordStatusA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @44 PwdGetPasswordStatusW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef PwdGetPasswordStatusW
DWORD WINAPI PwdGetPasswordStatusW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @45 PwdSetPasswordStatusA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef PwdSetPasswordStatusA
DWORD WINAPI PwdSetPasswordStatusA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @46 PwdSetPasswordStatusW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef PwdSetPasswordStatusW
DWORD WINAPI PwdSetPasswordStatusW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @47 WNetAddConnection2A -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetAddConnection2A
DWORD WINAPI WNetAddConnection2A(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @48 WNetAddConnection2W -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetAddConnection2W
DWORD WINAPI WNetAddConnection2W(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @49 WNetAddConnection3A -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetAddConnection3A
DWORD WINAPI WNetAddConnection3A(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @50 WNetAddConnection3W -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetAddConnection3W
DWORD WINAPI WNetAddConnection3W(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @51 WNetAddConnectionA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetAddConnectionA
DWORD WINAPI WNetAddConnectionA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @52 WNetAddConnectionW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetAddConnectionW
DWORD WINAPI WNetAddConnectionW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @53 WNetCachePassword -- 6-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetCachePassword
DWORD WINAPI WNetCachePassword(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6) { return 0; }

/* @54 WNetCancelConnection2A -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetCancelConnection2A
DWORD WINAPI WNetCancelConnection2A(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @55 WNetCancelConnection2W -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetCancelConnection2W
DWORD WINAPI WNetCancelConnection2W(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @56 WNetCancelConnectionA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetCancelConnectionA
DWORD WINAPI WNetCancelConnectionA(DWORD a1, DWORD a2) { return 0; }

/* @57 WNetCancelConnectionW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetCancelConnectionW
DWORD WINAPI WNetCancelConnectionW(DWORD a1, DWORD a2) { return 0; }

/* @58 WNetCloseEnum -- documented; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetCloseEnum
void WINAPI WNetCloseEnum(HANDLE a1) { }

/* @59 WNetConnectionDialog1A -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetConnectionDialog1A
DWORD WINAPI WNetConnectionDialog1A(DWORD a1) { return 0; }

/* @60 WNetConnectionDialog1W -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetConnectionDialog1W
DWORD WINAPI WNetConnectionDialog1W(DWORD a1) { return 0; }

/* @61 WNetConnectionDialog -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetConnectionDialog
DWORD WINAPI WNetConnectionDialog(DWORD a1, DWORD a2) { return 0; }

/* @62 WNetDisconnectDialog1A -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetDisconnectDialog1A
DWORD WINAPI WNetDisconnectDialog1A(DWORD a1) { return 0; }

/* @63 WNetDisconnectDialog1W -- 1-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetDisconnectDialog1W
DWORD WINAPI WNetDisconnectDialog1W(DWORD a1) { return 0; }

/* @64 WNetDisconnectDialog -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetDisconnectDialog
DWORD WINAPI WNetDisconnectDialog(DWORD a1, DWORD a2) { return 0; }

/* @65 WNetEnumCachedPasswords -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetEnumCachedPasswords
DWORD WINAPI WNetEnumCachedPasswords(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @66 WNetEnumResourceA -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetEnumResourceA
DWORD WINAPI WNetEnumResourceA(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @67 WNetEnumResourceW -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetEnumResourceW
DWORD WINAPI WNetEnumResourceW(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @68 WNetFormatNetworkNameA -- 6-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetFormatNetworkNameA
DWORD WINAPI WNetFormatNetworkNameA(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6) { return 0; }

/* @69 WNetFormatNetworkNameW -- 6-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetFormatNetworkNameW
DWORD WINAPI WNetFormatNetworkNameW(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6) { return 0; }

/* @70 WNetGetCachedPassword -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetCachedPassword
DWORD WINAPI WNetGetCachedPassword(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @71 WNetGetConnectionA -- documented; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetConnectionA
DWORD WINAPI WNetGetConnectionA(LPCSTR a1, LPSTR a2, LPDWORD a3) { return 0; }

/* @72 WNetGetConnectionW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetConnectionW
DWORD WINAPI WNetGetConnectionW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @73 WNetGetHomeDirectoryA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetHomeDirectoryA
DWORD WINAPI WNetGetHomeDirectoryA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @74 WNetGetHomeDirectoryW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetHomeDirectoryW
DWORD WINAPI WNetGetHomeDirectoryW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @75 WNetGetLastErrorA -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetLastErrorA
DWORD WINAPI WNetGetLastErrorA(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @76 WNetGetLastErrorW -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetLastErrorW
DWORD WINAPI WNetGetLastErrorW(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @77 WNetGetNetworkInformationA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetNetworkInformationA
DWORD WINAPI WNetGetNetworkInformationA(DWORD a1, DWORD a2) { return 0; }

/* @78 WNetGetNetworkInformationW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetNetworkInformationW
DWORD WINAPI WNetGetNetworkInformationW(DWORD a1, DWORD a2) { return 0; }

/* @79 WNetGetProviderNameA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetProviderNameA
DWORD WINAPI WNetGetProviderNameA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @80 WNetGetProviderNameW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetProviderNameW
DWORD WINAPI WNetGetProviderNameW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @81 WNetGetResourceInformationA -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetResourceInformationA
DWORD WINAPI WNetGetResourceInformationA(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @82 WNetGetResourceInformationW -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetResourceInformationW
DWORD WINAPI WNetGetResourceInformationW(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @83 WNetGetResourceParentA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetResourceParentA
DWORD WINAPI WNetGetResourceParentA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @84 WNetGetResourceParentW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetResourceParentW
DWORD WINAPI WNetGetResourceParentW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @85 WNetGetUniversalNameA -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetUniversalNameA
DWORD WINAPI WNetGetUniversalNameA(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @86 WNetGetUniversalNameW -- 4-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetUniversalNameW
DWORD WINAPI WNetGetUniversalNameW(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @87 WNetGetUserA -- documented; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetUserA
DWORD WNETAPI WNetGetUserA(LPCSTR lpName, LPSTR lpUserName, LPDWORD lpnLength) { return 0; }

/* @88 WNetGetUserW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetGetUserW
DWORD WINAPI WNetGetUserW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @89 WNetLogoffA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetLogoffA
DWORD WINAPI WNetLogoffA(DWORD a1, DWORD a2) { return 0; }

/* @90 WNetLogoffW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetLogoffW
DWORD WINAPI WNetLogoffW(DWORD a1, DWORD a2) { return 0; }

/* @91 WNetLogonA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetLogonA
DWORD WINAPI WNetLogonA(DWORD a1, DWORD a2) { return 0; }

/* @92 WNetLogonW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetLogonW
DWORD WINAPI WNetLogonW(DWORD a1, DWORD a2) { return 0; }

/* @93 WNetOpenEnumA -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetOpenEnumA
DWORD WINAPI WNetOpenEnumA(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @94 WNetOpenEnumW -- 5-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetOpenEnumW
DWORD WINAPI WNetOpenEnumW(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @95 WNetRemoveCachedPassword -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetRemoveCachedPassword
DWORD WINAPI WNetRemoveCachedPassword(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @96 WNetRestoreConnectionA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetRestoreConnectionA
DWORD WINAPI WNetRestoreConnectionA(DWORD a1, DWORD a2) { return 0; }

/* @97 WNetRestoreConnectionW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetRestoreConnectionW
DWORD WINAPI WNetRestoreConnectionW(DWORD a1, DWORD a2) { return 0; }

/* @98 WNetSetConnectionA -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetSetConnectionA
DWORD WINAPI WNetSetConnectionA(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @99 WNetSetConnectionW -- 3-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetSetConnectionW
DWORD WINAPI WNetSetConnectionW(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @100 WNetUseConnectionA -- 8-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetUseConnectionA
DWORD WINAPI WNetUseConnectionA(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7, DWORD a8) { return 0; }

/* @101 WNetUseConnectionW -- 8-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetUseConnectionW
DWORD WINAPI WNetUseConnectionW(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7, DWORD a8) { return 0; }

/* @102 WNetVerifyPasswordA -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetVerifyPasswordA
DWORD WINAPI WNetVerifyPasswordA(DWORD a1, DWORD a2) { return 0; }

/* @103 WNetVerifyPasswordW -- 2-arg; scaffold. TODO: marshal args, thunk to MPR.DLL, return */
#undef WNetVerifyPasswordW
DWORD WINAPI WNetVerifyPasswordW(DWORD a1, DWORD a2) { return 0; }

