// ============================ Xbox 360 STFS save-data mount (impl) ============================
// See Save360.reference.h. Implements the "Save:" STFS mount + the XContentFlush durability call.
//
// [360] equivalent: XShowDeviceSelectorUI + XContentCreate(OPENALWAYS) + XContentFlush + XContentClose.
// Auto-detected present in the installed SDK by xdk_scan (XContent* is a 26-fn family in xbox.h; the
// device selector + signin query are in xam.h). This file is what the coverage report means by
// "Save storage: REAL but UNWIRED" -- now wired.
//
// Everything is guarded by _XBOX so the same source compiles on a host build (where it takes the
// game:\ fallback path and Save360_Flush is a no-op).
// =============================================================================================
#include "stdafx.h"   // PCH -- MUST be first (the 360 project builds with precompiled headers)
#include "Save360.h"

#ifdef _XBOX
#include <xtl.h>
#endif

#include "Common.h"     // DbgPrintF -> mirrors into nexia.log (a stalled device-blade is otherwise invisible)
#include <string.h>

static bool             g_mounted = false;     // did the STFS package mount as "Save:"?
static Save360PresentFn g_present = 0;          // guide-UI present pump for the blocking device/signin waits
// The RESOLVED save-root string form ("Save:" vs "Save:\") that actually took a probe write. Set in
// Save360_Init after XContentCreate; Save360_SaveRoot returns it once mounted. [J2X hardening save-writable-probe]
static char             g_saveRoot[16] = "Save:";
#ifdef _XBOX
// J2X-BUGFIX save-mount-race: thread-safe idempotent gate for Save360_Init (0=unmounted, 1=mounting,
// 2=mounted). A racing caller (e.g. a background autosave that reaches boot) which loses the CAS waits out
// the winner instead of double-entering the device selector / XContentCreate. Console-only (host boot is
// single-threaded).
static volatile LONG    g_mountState = 0;
#endif

// The FIRST signed-in profile's controller port (0-3). Saves are per-profile on the 360.
static const int SAVE360_USER = 0;

// STFS root name we mount the package at. The 360 exposes it to CreateFile as "<root>:\...".
static const char* SAVE_ROOT_NAME = "Save";     // -> "Save:" drive
// Fallback app root used when there's no profile/device/STFS support (dev + emulator: game:\ is writable).
static const char* GAME_ROOT      = "game:";

void Save360_SetPresentCallback(Save360PresentFn fn) { g_present = fn; }

#ifdef _XBOX
// Block on an async XOVERLAPPED op while keeping the guide UI (device selector / signin blade) rendering.
// The notes are explicit: without pumping present here, the blade freezes during the wait.
// BOUNDED: an emulator that pops a blade it can never dismiss would hang boot forever (the exact
// "no logging, just hung" failure class this port keeps hitting). If the op hasn't completed after the
// backstop, cancel it and fall back to game:\ -- and DbgPrintF so the stall is visible in nexia.log.
static DWORD Save360_WaitOverlapped(XOVERLAPPED* ov) {
	// ~5 minutes of Sleep(1)+present as a pure anti-infinite-hang net (real single-device consoles auto-confirm
	// in well under a second; a human at a multi-device blade finishes long before this).
	const unsigned long BACKSTOP = 300000UL;
	unsigned long spins = 0;
	while (!XHasOverlappedIoCompleted(ov)) {
		if (g_present) g_present();     // keep drawing so the blade animates
		Sleep(1);
		if (++spins >= BACKSTOP) {
			DbgPrintF("[Save360] device-selector wait exceeded backstop -- cancelling, falling back to game:\\\n");
			XCancelOverlapped(ov);
			return ERROR_CANCELLED;
		}
	}
	DWORD result = ERROR_SUCCESS;
	XGetOverlappedResult(ov, &result, TRUE);
	return result;
}

// J2X-BUGFIX save-writable-probe: write + delete a probe file at "<root>_wtest" to confirm the mounted volume
// actually ACCEPTS a write (not just that the package opened) -- a full/write-protected/oddly-mounted volume
// can open yet reject CreateFile, silently dropping every save. No separator is inserted: the two candidate
// roots ("Save:" and "Save:\") intentionally probe different path forms, and we keep whichever the volume
// accepts. Uses CreateFile/DeleteFile (the same 360 file API the FileIO streams use). Matches the hand-port.
static bool _probeWritable(const char* root) {
	char p[32];
	strcpy(p, root);
	strcat(p, "_wtest");
	HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return false;
	CloseHandle(h);
	DeleteFileA(p);
	return true;
}
#endif

bool Save360_Init() {
	if (g_mounted) return true;         // fast path: already mounted (idempotent)

#ifdef _XBOX
	// Thread-safe idempotent gate: CAS 0->1 claims the mount; a racing caller that loses waits out the winner
	// rather than double-entering the device selector / XContentCreate. See g_mountState. [J2X hardening]
	if (InterlockedCompareExchange(&g_mountState, 1, 0) != 0) {
		for (int i = 0; i < 3000; ++i) { if (g_mountState != 1) break; Sleep(10); }   // ~30s: wait out the in-progress mount
		return (g_mountState == 2);
	}

	// 1) Require a signed-in profile (local or LIVE). No profile => no per-profile save device => fallback.
	XUSER_SIGNIN_STATE state = XUserGetSigninState(SAVE360_USER);
	if (state == eXUserSigninState_NotSignedIn) {
		DbgPrintF("[Save360] no signed-in profile on port 0 -- saves fall back to game:\\\n");
		g_mounted = false; g_mountState = 0;   // fall back to game:\ (the app root); release the gate
		return false;
	}

	// 2) Pick the storage device via the guide's device selector (async -> pump present while it's up).
	XCONTENTDEVICEID deviceId = XCONTENTDEVICE_ANY;
	{
		// STATIC (not stack): if the backstop abandons this op, a late async completion still writes to valid
		// memory instead of a freed stack frame. Save360_Init is idempotent + boot-single-threaded, so one
		// shared overlapped is fine.
		static XOVERLAPPED ov;
		memset(&ov, 0, sizeof(ov));
		// Ask the device selector for the TRUE on-device footprint, not a raw byte guess: XContentCalculateSize
		// folds in the STFS container overhead (hash tables, block padding) on top of the payload estimate.
		// Payload = a generous headroom for a Beta world's region files; cDirectories = the dir levels the save
		// uses (root + region/ + the two <x36>/<z36> chunk-path levels, with slack).
		const ULONGLONG SAVE_PAYLOAD_BYTES = 16ULL * 1024 * 1024;   // ~16 MB world headroom
		const DWORD     SAVE_DIR_COUNT     = 8;
		ULARGE_INTEGER bytesNeeded;
		bytesNeeded.QuadPart = XContentCalculateSize(SAVE_PAYLOAD_BYTES, SAVE_DIR_COUNT);
		DWORD rc = XShowDeviceSelectorUI(SAVE360_USER, XCONTENTTYPE_SAVEDGAME,
		                                 XCONTENTFLAG_OPENALWAYS, bytesNeeded, &deviceId, &ov);
		if (rc == ERROR_IO_PENDING) rc = Save360_WaitOverlapped(&ov);
		if (rc != ERROR_SUCCESS || deviceId == XCONTENTDEVICE_ANY) {
			DbgPrintF("[Save360] no storage device selected (rc=%lu) -- saves fall back to game:\\\n", (unsigned long)rc);
			g_mounted = false; g_mountState = 0;   // user cancelled / no device -> fallback; release the gate
			return false;
		}
	}

	// 3) Create-or-open the save content package and mount it as "Save:".
	XCONTENT_DATA data;
	memset(&data, 0, sizeof(data));
	data.DeviceID       = deviceId;
	data.dwContentType  = XCONTENTTYPE_SAVEDGAME;
	wcscpy(data.szDisplayName, L"Minecraft");     // shown in the console's memory/save UI
	strcpy(data.szFileName,    "LOGANCRAFT");     // STFS container file name (ASCII, <=42)

	DWORD disposition = 0;
	DWORD rc = XContentCreate(SAVE360_USER, SAVE_ROOT_NAME, &data, XCONTENTFLAG_OPENALWAYS,
	                          &disposition, NULL, NULL /*synchronous*/);
	// OPEN_ALWAYS: ERROR_SUCCESS (created or opened). ERROR_ALREADY_EXISTS is also a success here.
	if (rc == ERROR_SUCCESS || rc == ERROR_ALREADY_EXISTS) {
		// Verify the mounted volume actually ACCEPTS a write before declaring durable saves on. XContentCreate
		// success only means the package opened; a full / write-protected / oddly-mounted volume can open yet
		// reject CreateFile, silently dropping every later save. Probe the "Save:" form first, then the
		// trailing-separator "Save:\" form, and publish only the one that took the write. [J2X hardening]
		if      (_probeWritable("Save:"))   strcpy(g_saveRoot, "Save:");
		else if (_probeWritable("Save:\\")) strcpy(g_saveRoot, "Save:\\");
		else {
			DbgPrintF("[Save360] mount opened but root is not writable -- saves fall back to game:\\\n");
			XContentClose(SAVE_ROOT_NAME, NULL);   // unmount: a retry's XContentCreate needs the name free
			g_mounted = false; g_mountState = 0;
			return false;
		}
		DbgPrintF("[Save360] STFS save package mounted as %s (disposition=%lu) -- durable saves ON\n",
		          g_saveRoot, (unsigned long)disposition);
		g_mounted = true; g_mountState = 2;   // "Save:\..." now resolves for CreateFile/the FileIO streams
		return true;
	}
	DbgPrintF("[Save360] XContentCreate failed (rc=%lu) -- saves fall back to game:\\\n", (unsigned long)rc);
	g_mounted = false; g_mountState = 0;   // create failed -> fallback; release the gate
	return false;
#else
	// Host build: no XDK. Saves go to the writable app root.
	g_mounted = false;
	return false;
#endif
}

const char* Save360_SaveRoot() {
	// The probe-resolved form ("Save:" or "Save:\") once mounted (durable STFS), else the writable app root.
	return g_mounted ? g_saveRoot : GAME_ROOT;
}

void Save360_Flush() {
	if (!g_mounted) return;             // fallback root writes straight through -- nothing to commit
#ifdef _XBOX
	// THE durability step: an STFS container is not persisted until flushed. Synchronous (pOverlapped NULL).
	DWORD rc = XContentFlush(SAVE_ROOT_NAME, NULL);
	DbgPrintF("[Save360] XContentFlush(Save:) rc=%lu -- save batch committed\n", (unsigned long)rc);
#endif
}

void Save360_Shutdown() {
	if (!g_mounted) return;
#ifdef _XBOX
	XContentFlush(SAVE_ROOT_NAME, NULL);
	XContentClose(SAVE_ROOT_NAME, NULL);
	// Release the mount gate too: left at 2, a later Save360_Init (sign-out -> sign-in) would lose the CAS,
	// see g_mountState == 2 and report success WITHOUT remounting -- Save360_SaveRoot() would then return
	// game:\ and every save would silently stop being durable.
	g_mountState = 0;
#endif
	g_mounted = false;
}
