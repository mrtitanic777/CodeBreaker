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
| Visual C++ 4.0's C runtime libraries (LIBC, LIBCMT, MSVCRT: only to learn runtime signatures from, `identify --learn`) | `toolchains/period/compilers/msvc-4.0/LIB` | (named on the command line) |
| The Windows 3.1 DLL kit (lean MDLLCEW.LIB, LIBW.LIB, LIBENTRY.OBJ) | `toolchains/period/libs/win31-kit` | (found there) |
| Microsoft C 2.03 (MC1, MC2) | `binaries/latticec/oracle16` | `kits`: `msc203` |
| MASM (BIN, INC, LIB) | `toolchains/masmroot` | `masm` |
| A MASM-compatible NASM | built from `native/nasm` (default `native/nasm/build/bin/nasm.exe`) | `nasm` |
| The XENIX C compiler of Microsoft's 1984 tools (its files; the LINK/LIB reconstruction it is compared with: `xcc.py`'s folder) | | `kits`: `xenix86`; `xenix_c` |
| PowerPC compiler, linker and runtime | | `powerpc_tools`, `powerpc_lib` |
| A C++ compiler for judging transpiled Java (g++) | `C:/msys64/mingw64/bin/g++.exe` (or g++ on PATH) | `host_cxx` |
| Java (a JDK 8+: java and javac), to decompile a jar and to run classes beside their translation | `JAVA_HOME`, or `java` on PATH | `java` |
| RetroMCP CLI jar (RetroMCP-Java + Vineflower; MIT / Apache-2.0) | | `retromcp` |
| Visual Studio with clang-cl (builds XenonRecomp and recompiled Xbox 360 titles) | the newest install vswhere finds | `visual_studio` |
| PowerPC binutils that know VMX128 (the recompiler's instruction tests) | `toolchains/binutils-ppc` | `ppc_binutils` |
| The Xbox 360 SDK (XDK), for the Java transpiler's console target | `C:/Program Files (x86)/Microsoft Xbox 360 SDK` | `xbox360_sdk` |
| MSBuild, to build a transpiled game's Visual Studio project | | `msbuild` |
| Per-project hand fixes laid over transpiled Java | `state/java/overlays` | `java_overlays` |
| The 12 Xbox 360 PowerPC images the store has judged (`*.basefile`) | `../powerpc-samples/images` (moved out of the old `src/xbox/samples` on 2026-10-17; its README says what else was kept) | (the store's module paths) |
| Real Xbox 360 files the format tests read when present (an STFS theme, a dashboard XZP, a Minecraft PCK) | `../xbox-samples` | (tests/test_xbox_formats.py) |
| The Xbox 360 key store the `crypto` tools read (1BL_pub.bin, SB_prv.bin, ...) | `Keys/` in the current folder | `xbox360_keys` |
| The `crypto` tools' binaries: LZX libraries, the Free60 xenon-as / xenon-objcopy, hypervisor images | | `xbox360_crypto_bin` |
| ILSpy's command line (the `xna` lane's decompiler) | `dotnet tool install -g ilspycmd` | `ilspycmd` |
| MonoGame's effect compiler (the `xna` port compiles each recovered effect's HLSL) | `dotnet tool install -g dotnet-mgfxc --version 3.8.1.303` | `mgfxc` on PATH |
| The Xbox 360 SDK's fxc (tests only: tests/test_xbox_effect.py recompiles its .fxo fixtures with it when it is there) | the XDK's `bin/win32/fxc.exe` | `XEDK` |
| de4dot (the `xna` lane's deobfuscator: an obfuscated assembly is cleaned before it is decompiled) | built from its source (github.com/de4dot/de4dot) for net8, beside the checkout at `../_tools/de4dot/Release/net8.0/de4dot.exe` | `de4dot` (or PATH, or that build) |
| XNA Game Studio 4.0 (its GamerServices, Net, Storage and Avatar assemblies: `xna port` generates its stubs from their metadata) | the Windows GAC | |
| MonoGame DesktopGL 3.8.1 and the .NET 8 SDK (`xna port` builds against them) | NuGet's cache | |
| ffmpeg (XMA / XMA2 audio decoded to WAV) | PATH | `ffmpeg` |
| Per-game hand work for `xna port` (a hand port's shaders and fixes) | `state/xna/overlays/<name>/` | `xna_overlays` |

None of these is redistributable, and none is needed to read the code: each command says which
setting it is missing.

## What CodeBreaker makes

What CodeBreaker makes of a binary goes into that binary's project folder under `Converted/`
(setting `converted`), one folder per binary, the same layout every time:

    Converted/<name>/
      README.txt     what the binary is, what was done to it (each command, when), how to build or run the result
      project.json   the same for tools: the input, its SHA-1, its kind, every output and its command
      original/      the binary as it came in
      src/           source: decompiled C, recompiled C++, assembly, build files
      assets/        resources and data: extracted resources, a title's files
      build/         programs and libraries built from src/
      reports/       analysis, listings, disassembly, logs

`decompile`, `recompile --all`, `recompile-project`,
`resources` and `build` write there unless given `--out` / `-o`. A binary's project is found again
by its SHA-1, so every command run on one file adds to one folder.

The rest -- the function store, caches, scratch builds, scoreboards -- goes under `state/`
(setting `state`). Delete it to start over; nothing in it is source. The reconstructed Windows 95
source is written into the Windows 95 tree (`../Windows95`), where it is kept and curated.

The native decompiler is built from `native/decompiler` into `native/decompiler/build/bin`
(setting `decompiler_bin`); the build output is not tracked.
