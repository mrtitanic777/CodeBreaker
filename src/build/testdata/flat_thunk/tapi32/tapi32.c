/* tapi32.c -- 32-bit flat-thunk veneer. DllMain + per-export thunk-through.
 * SIGNATURES are recovered from each stub (stdcall arity via `ret N`; pointer args
 * via SMapLS). Per-function tags: documented/undocumented = published API; behavioral
 * = body reconstructed from observed behaviour (not byte-exact); scaffold = body not
 * yet implemented. Curation fills the marshalling and confirms doc/undoc + accuracy. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tapi.h>

extern BOOL __stdcall tapi32_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);

/* DLL entry -- connects the 32:16 thunk to the 16-bit target TAPI.DLL */
BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)
{
    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h);
        if (!tapi32_ThunkConnect32("TAPI.DLL", "TAPI32.DLL", h, r)) return FALSE; }
    else if (r == DLL_PROCESS_DETACH) tapi32_ThunkConnect32("TAPI.DLL", "TAPI32.DLL", h, r);
    return TRUE;
}

/* @1 lineAccept -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineAccept
DWORD WINAPI lineAccept(void) { return 0; }

/* @2 lineAddProvider -- 3-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineAddProvider
DWORD WINAPI lineAddProvider(LPVOID p1, LPVOID p2, DWORD a3) { return 0; }

/* @3 lineAddToConference -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineAddToConference
DWORD WINAPI lineAddToConference(void) { return 0; }

/* @4 lineAnswer -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineAnswer
DWORD WINAPI lineAnswer(void) { return 0; }

/* @5 lineBlindTransfer -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineBlindTransfer
DWORD WINAPI lineBlindTransfer(void) { return 0; }

/* @6 lineClose -- documented; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineClose
LONG WINAPI lineClose(HLINE hLine) { return 0; }

/* @7 lineCompleteCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineCompleteCall
DWORD WINAPI lineCompleteCall(void) { return 0; }

/* @8 lineCompleteTransfer -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineCompleteTransfer
DWORD WINAPI lineCompleteTransfer(void) { return 0; }

/* @9 lineConfigDialog -- 3-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineConfigDialog
DWORD WINAPI lineConfigDialog(LPVOID p1, DWORD a2, DWORD a3) { return 0; }

/* @10 lineConfigDialogEdit -- 5-arg, 3 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineConfigDialogEdit
DWORD WINAPI lineConfigDialogEdit(LPVOID p1, LPVOID p2, LPVOID p3, DWORD a4, DWORD a5) { return 0; }

/* @11 lineConfigProvider -- 2-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineConfigProvider
DWORD WINAPI lineConfigProvider(DWORD a1, DWORD a2) { return 0; }

/* @12 lineDeallocateCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineDeallocateCall
DWORD WINAPI lineDeallocateCall(void) { return 0; }

/* @13 lineDevSpecific -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineDevSpecific
DWORD WINAPI lineDevSpecific(void) { return 0; }

/* @14 lineDevSpecificFeature -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineDevSpecificFeature
DWORD WINAPI lineDevSpecificFeature(void) { return 0; }

/* @15 lineDial -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineDial
DWORD WINAPI lineDial(void) { return 0; }

/* @16 lineDrop -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineDrop
DWORD WINAPI lineDrop(void) { return 0; }

/* @17 lineForward -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineForward
DWORD WINAPI lineForward(void) { return 0; }

/* @18 lineGatherDigits -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGatherDigits
DWORD WINAPI lineGatherDigits(void) { return 0; }

/* @19 lineGenerateDigits -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGenerateDigits
DWORD WINAPI lineGenerateDigits(void) { return 0; }

/* @20 lineGenerateTone -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGenerateTone
DWORD WINAPI lineGenerateTone(void) { return 0; }

/* @21 lineGetAddressCaps -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetAddressCaps
DWORD WINAPI lineGetAddressCaps(void) { return 0; }

/* @22 lineGetAddressID -- 5-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineGetAddressID
DWORD WINAPI lineGetAddressID(LPVOID p1, LPVOID p2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @23 lineGetAddressStatus -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetAddressStatus
DWORD WINAPI lineGetAddressStatus(void) { return 0; }

/* @24 lineGetAppPriority -- 6-arg, 4 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineGetAppPriority
DWORD WINAPI lineGetAppPriority(LPVOID p1, LPVOID p2, LPVOID p3, LPVOID p4, DWORD a5, DWORD a6) { return 0; }

/* @25 lineGetCallInfo -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetCallInfo
DWORD WINAPI lineGetCallInfo(void) { return 0; }

/* @26 lineGetCallStatus -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetCallStatus
DWORD WINAPI lineGetCallStatus(void) { return 0; }

/* @27 lineGetConfRelatedCalls -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetConfRelatedCalls
DWORD WINAPI lineGetConfRelatedCalls(void) { return 0; }

/* @28 lineGetCountry -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetCountry
DWORD WINAPI lineGetCountry(void) { return 0; }

/* @29 lineGetDevCaps -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetDevCaps
DWORD WINAPI lineGetDevCaps(void) { return 0; }

/* @30 lineGetDevConfig -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetDevConfig
DWORD WINAPI lineGetDevConfig(void) { return 0; }

/* @31 lineGetID -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetID
DWORD WINAPI lineGetID(void) { return 0; }

/* @32 lineGetIcon -- 3-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineGetIcon
DWORD WINAPI lineGetIcon(LPVOID p1, LPVOID p2, DWORD a3) { return 0; }

/* @33 lineGetLineDevStatus -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetLineDevStatus
DWORD WINAPI lineGetLineDevStatus(void) { return 0; }

/* @34 lineGetNewCalls -- 4-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineGetNewCalls
DWORD WINAPI lineGetNewCalls(LPVOID p1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @35 lineGetNumRings -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetNumRings
DWORD WINAPI lineGetNumRings(void) { return 0; }

/* @36 lineGetProviderList -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetProviderList
DWORD WINAPI lineGetProviderList(void) { return 0; }

/* @37 lineGetRequest -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetRequest
DWORD WINAPI lineGetRequest(void) { return 0; }

/* @38 lineGetStatusMessages -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetStatusMessages
DWORD WINAPI lineGetStatusMessages(void) { return 0; }

/* @39 lineGetTranslateCaps -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineGetTranslateCaps
DWORD WINAPI lineGetTranslateCaps(void) { return 0; }

/* @40 lineHandoff -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineHandoff
DWORD WINAPI lineHandoff(void) { return 0; }

/* @41 lineHold -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineHold
DWORD WINAPI lineHold(void) { return 0; }

/* @42 lineInitialize -- 5-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineInitialize
DWORD WINAPI lineInitialize(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @43 lineMakeCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineMakeCall
DWORD WINAPI lineMakeCall(void) { return 0; }

/* @44 lineMonitorDigits -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineMonitorDigits
DWORD WINAPI lineMonitorDigits(void) { return 0; }

/* @45 lineMonitorMedia -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineMonitorMedia
DWORD WINAPI lineMonitorMedia(void) { return 0; }

/* @46 lineMonitorTones -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineMonitorTones
DWORD WINAPI lineMonitorTones(void) { return 0; }

/* @47 lineNegotiateAPIVersion -- 6-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineNegotiateAPIVersion
DWORD WINAPI lineNegotiateAPIVersion(LPVOID p1, LPVOID p2, DWORD a3, DWORD a4, DWORD a5, DWORD a6) { return 0; }

/* @48 lineNegotiateExtVersion -- 6-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineNegotiateExtVersion
DWORD WINAPI lineNegotiateExtVersion(LPVOID p1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6) { return 0; }

/* @49 lineOpen -- 9-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineOpen
DWORD WINAPI lineOpen(LPVOID p1, LPVOID p2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7, DWORD a8, DWORD a9) { return 0; }

/* @50 linePark -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef linePark
DWORD WINAPI linePark(void) { return 0; }

/* @51 linePickup -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef linePickup
DWORD WINAPI linePickup(void) { return 0; }

/* @52 linePrepareAddToConference -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef linePrepareAddToConference
DWORD WINAPI linePrepareAddToConference(void) { return 0; }

/* @53 lineRedirect -- 3-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineRedirect
DWORD WINAPI lineRedirect(LPVOID p1, DWORD a2, DWORD a3) { return 0; }

/* @54 lineRegisterRequestRecipient -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineRegisterRequestRecipient
DWORD WINAPI lineRegisterRequestRecipient(void) { return 0; }

/* @55 lineReleaseUserUserInfo -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineReleaseUserUserInfo
DWORD WINAPI lineReleaseUserUserInfo(void) { return 0; }

/* @56 lineRemoveFromConference -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineRemoveFromConference
DWORD WINAPI lineRemoveFromConference(void) { return 0; }

/* @57 lineRemoveProvider -- 2-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineRemoveProvider
DWORD WINAPI lineRemoveProvider(DWORD a1, DWORD a2) { return 0; }

/* @58 lineSecureCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSecureCall
DWORD WINAPI lineSecureCall(void) { return 0; }

/* @59 lineSendUserUserInfo -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSendUserUserInfo
DWORD WINAPI lineSendUserUserInfo(void) { return 0; }

/* @60 lineSetAppPriority -- 6-arg, 3 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineSetAppPriority
DWORD WINAPI lineSetAppPriority(LPVOID p1, LPVOID p2, LPVOID p3, DWORD a4, DWORD a5, DWORD a6) { return 0; }

/* @61 lineSetAppSpecific -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetAppSpecific
DWORD WINAPI lineSetAppSpecific(void) { return 0; }

/* @62 lineSetCallParams -- 5-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineSetCallParams
DWORD WINAPI lineSetCallParams(LPVOID p1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @63 lineSetCallPrivilege -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetCallPrivilege
DWORD WINAPI lineSetCallPrivilege(void) { return 0; }

/* @64 lineSetCurrentLocation -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetCurrentLocation
DWORD WINAPI lineSetCurrentLocation(void) { return 0; }

/* @65 lineSetDevConfig -- 4-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineSetDevConfig
DWORD WINAPI lineSetDevConfig(LPVOID p1, LPVOID p2, DWORD a3, DWORD a4) { return 0; }

/* @66 lineSetMediaControl -- 12-arg, 4 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineSetMediaControl
DWORD WINAPI lineSetMediaControl(LPVOID p1, LPVOID p2, LPVOID p3, LPVOID p4, DWORD a5, DWORD a6, DWORD a7, DWORD a8, DWORD a9, DWORD a10, DWORD a11, DWORD a12) { return 0; }

/* @67 lineSetMediaMode -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetMediaMode
DWORD WINAPI lineSetMediaMode(void) { return 0; }

/* @68 lineSetNumRings -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetNumRings
DWORD WINAPI lineSetNumRings(void) { return 0; }

/* @69 lineSetStatusMessages -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetStatusMessages
DWORD WINAPI lineSetStatusMessages(void) { return 0; }

/* @70 lineSetTerminal -- 7-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineSetTerminal
DWORD WINAPI lineSetTerminal(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7) { return 0; }

/* @71 lineSetTollList -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetTollList
DWORD WINAPI lineSetTollList(void) { return 0; }

/* @72 lineSetupConference -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetupConference
DWORD WINAPI lineSetupConference(void) { return 0; }

/* @73 lineSetupTransfer -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineSetupTransfer
DWORD WINAPI lineSetupTransfer(void) { return 0; }

/* @74 lineShutdown -- 1-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineShutdown
DWORD WINAPI lineShutdown(DWORD a1) { return 0; }

/* @75 lineSwapHold -- 2-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineSwapHold
DWORD WINAPI lineSwapHold(DWORD a1, DWORD a2) { return 0; }

/* @76 lineTranslateAddress -- 7-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineTranslateAddress
DWORD WINAPI lineTranslateAddress(LPVOID p1, LPVOID p2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7) { return 0; }

/* @77 lineTranslateDialog -- 5-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineTranslateDialog
DWORD WINAPI lineTranslateDialog(LPVOID p1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @78 lineUncompleteCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineUncompleteCall
DWORD WINAPI lineUncompleteCall(void) { return 0; }

/* @79 lineUnhold -- 1-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef lineUnhold
DWORD WINAPI lineUnhold(DWORD a1) { return 0; }

/* @80 lineUnpark -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef lineUnpark
DWORD WINAPI lineUnpark(void) { return 0; }

/* @81 phoneClose -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneClose
DWORD WINAPI phoneClose(void) { return 0; }

/* @82 phoneConfigDialog -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneConfigDialog
DWORD WINAPI phoneConfigDialog(void) { return 0; }

/* @83 phoneDevSpecific -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneDevSpecific
DWORD WINAPI phoneDevSpecific(void) { return 0; }

/* @84 phoneGetButtonInfo -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetButtonInfo
DWORD WINAPI phoneGetButtonInfo(void) { return 0; }

/* @85 phoneGetData -- 4-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneGetData
DWORD WINAPI phoneGetData(LPVOID p1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @86 phoneGetDevCaps -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetDevCaps
DWORD WINAPI phoneGetDevCaps(void) { return 0; }

/* @87 phoneGetDisplay -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetDisplay
DWORD WINAPI phoneGetDisplay(void) { return 0; }

/* @88 phoneGetGain -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetGain
DWORD WINAPI phoneGetGain(void) { return 0; }

/* @89 phoneGetHookSwitch -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetHookSwitch
DWORD WINAPI phoneGetHookSwitch(void) { return 0; }

/* @90 phoneGetID -- 3-arg, 2 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneGetID
DWORD WINAPI phoneGetID(LPVOID p1, LPVOID p2, DWORD a3) { return 0; }

/* @91 phoneGetIcon -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetIcon
DWORD WINAPI phoneGetIcon(void) { return 0; }

/* @92 phoneGetLamp -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetLamp
DWORD WINAPI phoneGetLamp(void) { return 0; }

/* @93 phoneGetRing -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetRing
DWORD WINAPI phoneGetRing(void) { return 0; }

/* @94 phoneGetStatus -- 2-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneGetStatus
DWORD WINAPI phoneGetStatus(LPVOID p1, DWORD a2) { return 0; }

/* @95 phoneGetStatusMessages -- 4-arg, 3 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneGetStatusMessages
DWORD WINAPI phoneGetStatusMessages(LPVOID p1, LPVOID p2, LPVOID p3, DWORD a4) { return 0; }

/* @96 phoneGetVolume -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneGetVolume
DWORD WINAPI phoneGetVolume(void) { return 0; }

/* @97 phoneInitialize -- 5-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneInitialize
DWORD WINAPI phoneInitialize(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @98 phoneNegotiateAPIVersion -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneNegotiateAPIVersion
DWORD WINAPI phoneNegotiateAPIVersion(void) { return 0; }

/* @99 phoneNegotiateExtVersion -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneNegotiateExtVersion
DWORD WINAPI phoneNegotiateExtVersion(void) { return 0; }

/* @100 phoneOpen -- 7-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneOpen
DWORD WINAPI phoneOpen(LPVOID p1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7) { return 0; }

/* @101 phoneSetButtonInfo -- 3-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneSetButtonInfo
DWORD WINAPI phoneSetButtonInfo(LPVOID p1, DWORD a2, DWORD a3) { return 0; }

/* @102 phoneSetData -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneSetData
DWORD WINAPI phoneSetData(void) { return 0; }

/* @103 phoneSetDisplay -- 5-arg, 1 ptr; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneSetDisplay
DWORD WINAPI phoneSetDisplay(LPVOID p1, DWORD a2, DWORD a3, DWORD a4, DWORD a5) { return 0; }

/* @104 phoneSetGain -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneSetGain
DWORD WINAPI phoneSetGain(void) { return 0; }

/* @105 phoneSetHookSwitch -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneSetHookSwitch
DWORD WINAPI phoneSetHookSwitch(void) { return 0; }

/* @106 phoneSetLamp -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneSetLamp
DWORD WINAPI phoneSetLamp(void) { return 0; }

/* @107 phoneSetRing -- 3-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneSetRing
DWORD WINAPI phoneSetRing(DWORD a1, DWORD a2, DWORD a3) { return 0; }

/* @108 phoneSetStatusMessages -- 4-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneSetStatusMessages
DWORD WINAPI phoneSetStatusMessages(DWORD a1, DWORD a2, DWORD a3, DWORD a4) { return 0; }

/* @109 phoneSetVolume -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef phoneSetVolume
DWORD WINAPI phoneSetVolume(void) { return 0; }

/* @110 phoneShutdown -- 1-arg; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef phoneShutdown
DWORD WINAPI phoneShutdown(DWORD a1) { return 0; }

/* @111 tapiGetLocationInfo -- documented; scaffold. TODO: marshal args, thunk to TAPI.DLL, return */
#undef tapiGetLocationInfo
LONG WINAPI tapiGetLocationInfo(LPSTR lpszCountryCode, LPSTR lpszCityCode) { return 0; }

/* @112 tapiRequestDrop -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef tapiRequestDrop
DWORD WINAPI tapiRequestDrop(void) { return 0; }

/* @113 tapiRequestMakeCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef tapiRequestMakeCall
DWORD WINAPI tapiRequestMakeCall(void) { return 0; }

/* @114 tapiRequestMediaCall -- API (mark documented/undocumented); scaffold. TODO: signature + marshal + thunk + return */
#undef tapiRequestMediaCall
DWORD WINAPI tapiRequestMediaCall(void) { return 0; }

