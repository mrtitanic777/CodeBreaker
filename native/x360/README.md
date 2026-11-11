# CodeBreaker's Xbox 360 runtime

A statically recompiled Xbox 360 title (its PowerPC code turned into C++ by XenonRecomp,
`native/xenonrecomp`, configured by `codebreaker/recompile/config.py`) runs on this runtime as a
Windows program. The runtime is native code: the Xbox kernel and XAM are reimplemented export by
export (HLE), nothing of the console's system software runs in it unless a module is itself
recompiled and linked in.

- `runtime/src/` — the runtime
  - `kernel/` — xboxkrnl and XAM: threads, synchronisation, APCs and DPCs, memory, files (the
    title's folder is `game:`), XEX loading, content packages and STFS, profiles and sign-in,
    achievements, networking, the system UI's XAM side (`xam_ui.cpp`).
  - `gpu/` — the Xenos GPU on Direct3D 12: the command processor (ring buffer, PM4), the register
    file, shader microcode to HLSL (`shader_translator.cpp`), textures (`texture.h` says which
    formats), EDRAM resolves, the output merger, presenting.
  - `audio/` — the APU and XMA decoding (WMA Pro), XAudio's render driver, host output.
  - `input.cpp` — XInput controllers, keyboard and mouse.
  - `modules.*`, `xex.*`, `memory.*`, `vfs.*` — the guest address space, the one indirect-call table
    every recompiled module shares (`ppc_shared/ppc_lookup.h`), loading recompiled modules.
  - `main.cpp` — the title host: `<title>.exe [title folder]`.
- `system/` — the console's system UI and its formats, independent of the runtime's kernel:
  `xui/` (XUI packages and scenes: the Guide, message boxes, sign-in, the keyboard), `guide/` (the
  Guide's screens), `stfs/` (STFS packages), `live/` (a LIVE replacement: presence, friends,
  messages, parties, against a server of one's own), `pads/` (other controllers), `transition/`.
  Their assets are the user's own (extracted from a system update), never stored here.
- `x360.cmake` — `x360_title(<target> <recompiled dir> <module_info.cpp>)` builds a title's program.
- `tools/gen_import_stubs.py` — a stub that reports the call, for each import the runtime does not
  implement yet.

Switches (environment): `CB_HEADLESS=1` (no window), `CB_LOG=<file>`, `CB_WATCHDOG=<s>` (dump the
threads when no frame comes), `CB_TRACE_IMPORTS=1` or `=<names>`, `CB_CONTINUE_UNIMPLEMENTED=1`
(an unimplemented import returns 0 instead of stopping), `CB_PRESENT_DUMP=<frames>` (BMPs of those
frames), `CB_GPU_TRACE`, `CB_PM4_LOG`, `CB_TEXTURE_DUMP`, `CB_SHADER_CACHE=0`, `CB_D3D12_DEBUG=1`.

Comparing renderers: `CB_REPLAY_TRACE=<file>.xtr` (with `CB_REPLAY_FRAMES=<n>`) draws a GPU trace a
Xenia-based renderer recorded (Xenia's `--trace_gpu_stream`) with this renderer instead of running the
title; it is deterministic, so the switches below can bisect one frame. `CB_SKIP_DRAWS=<frame>:<first>-<last>;...`
leaves out draws of a frame (`CB_GPU_TRACE` numbers them); `CB_PS_PROBE=<hash>:<slot>:<register>` makes a
pixel shader output a register as its colour, and `CB_PS_PROBE=ALL[:<n>]` paints every pixel shader (longer
than n dwords) a flat colour from its hash.
A fault reports the guest address, the guest function and the host stack (offsets into the
program; `llvm-symbolizer --relative-address` turns them into the recompiled functions).

The FFmpeg-derived WMA Pro tables (`runtime/src/audio/wmapro_tables.inc`) are LGPL-2.1+;
`native/third_party/nlohmann` is MIT (NOTICE).
