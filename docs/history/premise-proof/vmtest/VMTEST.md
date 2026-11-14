# Runtime (integral) verification — LZ matched pair on real Win95

This is the one premise property that can only be shown at runtime, because the
flat-thunk mechanism (`QT_Thunk`/`SMapLS`/`ThunkConnect32`) is implemented by the
**Win95 kernel** — it does not exist on Windows NT/10/11. So this test must run
inside the Win95 VM (`Windows95/design/vm`).

## What's here (all built with our MS-free toolchain)

- `lz32.dll`     — 32-bit PE veneer + our `wthunk` flat thunks (built by `build_lz32.bat`)
- `lzexpand.dll` — 16-bit NE codec (built by `build_lzexpand.bat`)
- `test_lz.exe`  — Win32 console harness (built with `wcl386`); source `test_lz.c`

## Run it

1. Copy `lz32.dll`, `lzexpand.dll`, `test_lz.exe` into one folder in the Win95 VM
   (shared folder, or make an ISO/floppy). Put the two DLLs next to the exe (or in
   `C:\WINDOWS\SYSTEM`).
2. From a DOS box in Win95: `test_lz`
   (Win95 runs Win32 console apps via the built-in Win32 subsystem.)

## Expected output (success)

```
OK  : lz32.dll loaded (DllMain + ThunkConnect32 ran)
OK  : GetProcAddress GetExpandedNameA=... LZStart=... LZDone=...
OK  : LZStart() = 1   (round-tripped 32->16 into the codec)
OK  : LZDone() returned (thunk round-trip, no fault)
OK  : GetExpandedNameA("test.tx_") = ...
DONE: matched pair is behaviorally integral on Win95.
```

Reaching `DONE` without a fault means: `lz32.dll` loaded, `ThunkConnect32`
matched our two halves' `ThunkData` (self-consistent checksum), and calls
marshalled 32->16 through the codec and back — i.e. the pair rebuilt from
recovered source, with no Microsoft tools, is **behaviorally integral** on Win95.

## If it faults / errors

- `LZStart()` returning a negative LZERROR is still a successful thunk round-trip
  (the call reached the 16-bit codec and returned) — only a fault/hang is a real
  failure.
- A load failure usually means `lzexpand.dll` isn't found (put it beside `lz32.dll`
  or in `SYSTEM`), or a `ThunkData` checksum mismatch (both halves must be from the
  same `wthunk`/`lz.thk` — rebuild both).
