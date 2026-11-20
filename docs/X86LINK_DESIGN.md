# x86link — CodeBreaker's own x86 linker

CodeBreaker links the objects its builds make with an engine of its own, `native/cb/build/x86link/`, compiled into
`codebreaker.exe`. It replaces OpenWatcom's `wlink`, which CodeBreaker starts as a child process today (about fifteen
callers: `build/driver`, `build/functions`, `build/module_build16`/`32`, `build/flat_thunk`, the lane kit's Win16, DOS and
machine lanes, `source/assembly`, `source/vxd/owbuild`). When x86link is complete those callers call it in memory and
`wlink` is no longer run. (Plan: docs/BUILTIN_ENGINES_PLAN.md, item 3. Its sibling is x86asm, docs/X86ASM_DESIGN.md.)

## What "done" means

**Functionally identical to wlink as CodeBreaker runs it**: for every link, the same bytes out — the same executable, the
same errors refused. Not "a working program": the builds are compared byte for byte with the originals they reconstruct,
so the engine must make every choice wlink makes (segment order and alignment, the group and class rules, the DOS
header's fields, the relocation tables and their order, NE's entry table and resource layout, PE's section layout and
import tables, LE's object pages and fixup records).

The proof is differential, never by eye:

- **The capture corpus.** With `CB_WLINK_CAPTURE=<folder>` set (`native/cb/core/wlink_capture`), every wlink run
  CodeBreaker makes saves its arguments, its link scripts, every object and library it reads (found as wlink finds
  them: as named, then the working folder, each `libpath`, `LIB`, the libpaths of `wlsystem.lnk`), the environment it
  reads, its messages, its exit code and what it wrote. Inputs are kept once, by content (`blobs/<sha1>`), so
  OpenWatcom's libraries cost their size once. A gate run and a corpus sweep give the real links.
- `cb_tests` area `x86link` replays every case through x86link and compares bytes with the recorded output. A case that
  differs is a bug in x86link; nothing is ever "close enough".

## What CodeBreaker links (the scope)

| Output | `system` / `format` | Inputs | Callers |
|---|---|---|---|
| DOS MZ `.exe`, `.com` | `format dos`, `format dos com` | OMF from wcc (`-ml`, `-ms`) and wasm, OpenWatcom's 16-bit C library | DOS writer, DOS lanes |
| Win16 NE `.exe`, `.dll` | `system windows`, `system windows dll` | OMF, import libraries, `export`/`import` directives, resources (wrc) | module_build16, Win16 lane |
| Win32 PE `.exe`, `.dll` | `system nt`, `system nt_dll`, `format windows nt` | OMF from wcc386, import libraries | module_build32, functions, flat thunks |
| VxD LE | `format windows vxd [virtdevice]` | OMF from wasm (MASM dialect) | VxD writer, owbuild |

The directives that occur (the corpus lists the rest): `file`, `library`, `libpath`, `name`, `system`, `format`,
`option` (`map`, `quiet`, `stack`, `offset`, `alignment`, `nodefaultlibs`, `packcode`/`packdata`, `start`, `heapsize`,
`modname`, `description`, `version`), `export`, `import`, `segment`, `order clname`, `alias`.

## Architecture

wlink's seams are kept, as C++ modules (idiomatic typed C++, docs/CPP rules); each ports wlink's file of the same job
from the fork (`experiments/premise-proof/open-watcom-v2`, `bld/wl`, branch `codebreaker`), with wlink's behaviour.

| Module (`native/cb/build/x86link/`) | wlink source it ports | Job |
|---|---|---|
| `link.hpp/.cpp` | the globals of `wlink.c`, `linkstd.h` | `Link`: all state of one link (options, symbols, segments, groups, the output) |
| `directives` | `cmdline.c`, `cmdall.c`, `cmddos.c`, `cmdnt.c`, `cmdos2.c`, `cmd16m.c` and `wlsystem.lnk` | the directive language, `system` blocks |
| `objects` | `objomf.c`, `objorl.c` (OMF; COFF later if a caller needs it) | reading OMF objects and libraries (cb/formats/omf reads the records) |
| `symbols` | `symtrace.c`, `objpass1.c`, `objpass2.c`, `libr.c` | symbol resolution, library search, the two passes |
| `layout` | `mapio.c`'s order, `ring.c`, `distrib.c`, `objcalc.c` | segment and group placement, classes and their order |
| `fixups` | `fixup.c`, `reloc.c` | applying fixups, the relocations each format keeps |
| `format_dos` | `loaddos.c` | MZ and COM |
| `format_ne` | `load16m.c`, `loados2.c` (NE), `wres` for resources | NE |
| `format_pe` | `loadpe.c` | PE |
| `format_le` | `loadflat.c` (LE/VxD) | LE |
| `map` | `mapio.c` | `option map` |
| `driver` | `wlink.c` | the passes, the command line (`codebreaker wlink <wlink's arguments>`) |

### Rules

1. **No global or static mutable state.** Everything wlink keeps in globals lives in the `Link` of the run; two links run
   at once in one process without touching each other.
2. **No `exit()`.** A link that fails is an error the caller gets (`LinkError` with wlink's message text and number).
3. **No files unless asked.** `link(Request{directives, objects, libraries}) -> Result{image, map, messages}`; inputs are
   read through a callback (files, or a map of names to bytes), so the corpus replays from memory.
4. **The command line too.** `codebreaker wlink <wlink's arguments>` behaves as wlink did, for people and tests.
5. **Speed.** Faster than starting wlink.
6. **Tests.** Each module has `*_test.cpp` in area `x86link`; the corpus replay is the acceptance.

## Phases

1. The capture hook (done: `core/wlink_capture`), a corpus from a gate run and a sweep, and the replay harness (every case
   fails until the engine exists).
2. `link`, `directives`, `objects` (over cb/formats/omf), `symbols`, `layout`, `fixups` and `format_dos`: the DOS
   lanes switch to x86link.
3. `format_le`: the VxD writer and owbuild switch.
4. `format_ne` with resources: Win16.
5. `format_pe`: Win32.
6. Every caller on x86link; `wlink` is no longer run. OpenWatcom stays only for wcc/wcc386 until the compiler port
   (BUILTIN_ENGINES_PLAN item 5) replaces them too.
