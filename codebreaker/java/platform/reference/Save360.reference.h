#ifndef SAVE360_H
#define SAVE360_H
// ============================ Xbox 360 STFS save-data mount ===================================
// World saves on the 360 must live in an STFS content package (durable per-profile storage), NOT a raw
// file write to the read-only game:\ mount. This layer creates/opens the package, mounts it as the
// "Save:" drive so the existing FileIO streams (CreateFile/WriteFile) "just work", and -- the whole point
// of this file -- FLUSHES it (XContentFlush) after a save batch so the bytes actually persist to the
// device. An STFS container is NOT durable until flushed; a missing flush silently loses everything.
//
// [360] equivalent: XContentCreate(OPENALWAYS) + XContentFlush + XContentClose (xam.lib / xbox.h).
// [360] requirement (auto-detected by xdk_scan): saves go through XContent*, device via XShowDeviceSelectorUI.
//
// GRACEFUL FALLBACK: if there is no signed-in profile / storage device / STFS support (e.g. under an
// emulator that doesn't implement XContentCreate), Save360_Init leaves the package UNMOUNTED and
// Save360_SaveRoot() returns the writable app root ("game:") instead of "Save:", and Save360_Flush() is a
// no-op. So saving still works where game:\ is writable (dev/emulator), and is durable on real hardware.
// =============================================================================================

typedef void (*Save360PresentFn)();   // called during signin/device waits so the guide UI keeps rendering

// Mount the save package (idempotent). Picks the storage device, XContentCreate(SAVEDGAME, OPENALWAYS),
// mounts as "Save:". Returns true if the STFS package mounted, false if it fell back to game:.
bool Save360_Init();

// The root path the game should build save file paths under -- "Save:" when the STFS package is mounted,
// else the writable app root (e.g. "game:") as a fallback. Callers do: Save360_SaveRoot() + "\\<world>\\...".
const char* Save360_SaveRoot();

// Persist the STFS container to the device. Call AFTER a save batch (world save / on pause). No-op when
// running on the fallback root.
void Save360_Flush();

// Flush + XContentClose (unmount). Call on clean shutdown / world unload.
void Save360_Shutdown();

// The guide UI must keep rendering during the (blocking) signin/device waits -- give it the game's present fn.
void Save360_SetPresentCallback(Save360PresentFn fn);

#endif // SAVE360_H
