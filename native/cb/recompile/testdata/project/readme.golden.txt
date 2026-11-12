# game - recompiled by CodeBreaker

Static-recompiled from the Xbox 360 title to native C++ (the XenonRecomp method, as the
rexglue SDK carries it out; codebreaker/recompile). Laid out as a real source tree.

## Run it
Double-click **`Play-game.bat`** - it builds the project on first run (needs
CMake + a C++ compiler on PATH) and then launches it.

Or build by hand:
```
cmake -S . -B build
cmake --build build --config Release
```

## Source tree
```
include/
  game.h            declarations of every recompiled function + the mapping table
  game_config.h     image base 0x82000000, entry 0x82001000
  rex_ppc_min.h       PowerPC runtime header (PPCContext, byte-swap memory, helpers)
src/
  game_0000.cpp ..  the 12,345 recompiled functions, split across 7 files
  game_init.cpp     the guest-address -> function-pointer table
  rex_runtime.cpp     guest memory, indirect-call table, host-shim registry, boot main()
assets/
  game.bin          the decoded guest image, loaded flat at the image base
Play-game.bat       double-click launcher
```

Coverage: 1,999/2,000 instructions translated
(100.0%).

## Honest status
This **builds and links** as scaffolded. To actually **run** the whole title the
host runtime still needs filling in (the scaffold gives the exact place for each):
- **Kernel/XAM/CRT host shims** - the imports currently log + no-op.
- **GPU bridge** - the Xenos command processor / shader translation.
- **setjmp/longjmp & thread/heap setup** - scaffolded, needs wiring.
- **What is left untranslated** - `PPC_UNIMPLEMENTED` marks it (padding, and any instruction no
  builder covers); it traps if reached.

See CodeBreaker's `docs/RECOMPILE.md` for how it works and the runtime work that remains.
