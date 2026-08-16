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
#endif

bool Save360_Init() {
	if (g_mounted) return true;         // idempotent

#ifdef _XBOX
	// 1) Require a signed-in profile (local or LIVE). No profile => no per-profile save device => fallback.
	XUSER_SIGNIN_STATE state = XUserGetSigninState(SAVE360_USER);
	if (state == eXUserSigninState_NotSignedIn) {
		DbgPrintF("[Save360] no signed-in profile on port 0 -- saves fall back to game:\\\n");
		g_mounted = false;              // fall back to game:\ (the app root)
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
			g_mounted = false;          // user cancelled / no device -> fallback
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
		DbgPrintF("[Save360] STFS save package mounted as Save:\\ (disposition=%lu) -- durable saves ON\n", (unsigned long)disposition);
		g_mounted = true;               // "Save:\..." now resolves for CreateFile/the FileIO streams
		return true;
	}
	DbgPrintF("[Save360] XContentCreate failed (rc=%lu) -- saves fall back to game:\\\n", (unsigned long)rc);
	g_mounted = false;                  // create failed -> fallback
	return false;
#else
	// Host build: no XDK. Saves go to the writable app root.
	g_mounted = false;
	return false;
#endif
}

const char* Save360_SaveRoot() {
	// "Save:" once mounted (durable STFS), else the writable app root as a fallback.
	return g_mounted ? "Save:" : GAME_ROOT;
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
#endif
	g_mounted = false;
}
