# What is not in the repo

CodeBreaker tracks its own code only. The binaries it reconstructs, the period compilers and
kits it builds with, and everything it produces live on disk beside it and are named in
`settings.json` at the top of the checkout (copy `settings.example.json`). A path in
`settings.json` may be relative to the checkout. An environment variable `CB_<KEY>` overrides a
setting for one run.

## Inputs and tools, and the setting that names each

| What | Where it usually is | Setting |
|---|---|---|
| The original binaries | `../Windows95/design/corpus/files` | `corpus`, `corpora` |
| The SDK and DDK of the time (headers, debug symbols, MASM 6.11c, the thunk compiler) | `../Windows95/design/reference` | `reference` (default: the corpus's design folder) |
| OpenWatcom (builds and judges reconstructions) | `experiments/premise-proof/open-watcom-v2/rel` | `openwatcom` |
| DOSBox-X (runs the DOS compilers, assemblers and linkers) | `toolchains/dosbox-x/bin/x64/Release/dosbox-x.exe` | `dosbox_x` |
| Microsoft C 6.0a | `toolchains/period/compilers/msc-6.0a` | `msc6` |
| Visual C++ 1.52c (Microsoft C/C++ 8, LINK 5.60, RC) | `toolchains/period/compilers/msvc-1.52c` | `msvc152` |
| Visual C++ 2.0 (the 32-bit compiler of 1994) | `toolchains/period/compilers/msvc-2.0/BIN` | `period_compiler` |
| The Windows 3.1 DLL kit (lean MDLLCEW.LIB, LIBW.LIB, LIBENTRY.OBJ) | `toolchains/period/libs/win31-kit` | (found there) |
| Microsoft C 2.03 (MC1, MC2) | `binaries/latticec/oracle16` | `msc203` |
| MASM (BIN, INC, LIB) | `toolchains/masmroot` | `masm` |
| A MASM-compatible NASM | `../nasm-masm/nasm.exe` (its own repository) | `nasm` |
| The XENIX C compiler of Microsoft's 1984 tools | `xcc.py`'s folder | `xenix_c` |
| PowerPC compiler, linker and runtime | | `powerpc_tools`, `powerpc_lib` |

None of these is redistributable, and none is needed to read the code: each command says which
setting it is missing.

## What CodeBreaker makes

Everything it produces -- the function store, caches, build folders, reports -- goes under
`state/` (setting `state`). Delete it to start over; nothing in it is source. The reconstructed
source itself is written into the Windows 95 tree (`../Windows95`), where it is kept and curated.

The native decompiler is built from `native/decompiler` into `native/decompiler/build/bin`
(setting `decompiler_bin`); the build output is not tracked.
