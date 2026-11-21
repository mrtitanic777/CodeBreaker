# x86link — CodeBreaker's own x86 linker

CodeBreaker links the objects its builds make with an engine of its own, `src/build/x86link/`, compiled into
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

- **The capture corpus.** With `CB_WLINK_CAPTURE=<folder>` set (`src/core/wlink_capture`), every wlink run
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

## The route (Logan, 2026-11-20): OpenWatcom's own source first, then refactor over time

**First, wlink's own C source is compiled into src's one build** as a library (from the fork,
`toolchains/openwatcom`, `bld/wl`, branch `codebreaker`, and the OpenWatcom libraries it needs) and called in-process
through the entry point OpenWatcom already has for its IDE DLL (`wlink.dll`'s `IDEDLL` interface: a link run inside another
program, its state set up and torn down per run). That makes x86link byte-identical to wlink by construction; the
corpus replay proves it (and keeps proving it).

**Then it is refactored over time** into the CodeBreaker modules below, one module at a time, each step re-proven
byte-identical on the corpus before it lands: C into idiomatic typed C++, globals into the `Link` of a run, files into
callbacks. Nothing is rewritten from scratch first. The same route is taken for OpenWatcom's C compilers (wcc, wcc386:
docs/BUILTIN_ENGINES_PLAN.md item 5; their IDE DLLs are `wccd.dll` / `wccd386.dll`).

## Architecture (where the refactoring goes)

wlink's seams are kept, as C++ modules (idiomatic typed C++, docs/CPP rules); each ports wlink's file of the same job
from the fork (`toolchains/openwatcom`, `bld/wl`, branch `codebreaker`), with wlink's behaviour.

| Module (`src/build/x86link/`) | wlink source it came from | Job |
|---|---|---|
| `link` | `global.c`, `globals.h`, `linkstd.h`, `link.h` (now `flags.h`), `formats.h`, `objstruc.h`, `syms.h`, `permdata.c` | `Link` (`link.hpp`): all state of one link; the run's state (`run_state`); the types of objects, symbols, segments and formats |
| `base` | `wlmem.c`, `carve.c`, `virtmem.c`, `spillio.c`, `hash.c`, `strtab.c`, `ring.c`, `ring2.c`, `linkutil.c`, `ntio.c` (now `fileio.cpp`), `mixcache.c`, `debug.c` | memory, carvers, virtual memory, hash and string tables, rings, the file layer |
| `messages` | `msg.c`, `wlnkmsg.c`, `lnkerror.msg`, `wlink.msg` | the messages, compiled in (`msgtable.cpp`) |
| `directives` | `cmdline.c`, `cmdall.c`, `cmddos.c`, `cmdnt.c`, `cmdos2.c`, `cmd16m.c` ... and `wlsystem.lnk` | the directive language, `system` blocks |
| `objects` | `objomf.c`, `objorl.c`, `objio.c`, `omfreloc.c`, `objnode.c`, `objfree.c`, `objstrip.c`, `procfile.c` | reading OMF and ORL (COFF, ELF) objects and libraries |
| `symbols` | `symtrace.c`, `objpass1.c`, `objpass2.c`, `libr.c`, `libsupp.c`, `lsymtab.c`, `symmem.c`, `wcomdef.c`, `impexp.c` | the symbol table, the two passes, library search, communal data, imports and exports |
| `layout` | `distrib.c`, `objcalc.c`, `autogrp.c`, `overlays.c`, `ovlsupp.c`, `salloc.c` | segment and group placement, classes and their order, overlays, addresses |
| `fixups` | `obj2supp.c`, `reloc.c`, `toc.c` | applying fixups, the relocations each format keeps, the TOC |
| `debuginfo` | `dbgall.c`, `dbgcv.c`, `dbghll.c`, `dbgdwarf.c`, `dbginfo.c` | Watcom, CodeView, HLL and DWARF debugging information |
| `formats` | `loadfile.c` (now `output.cpp`), `dosstub.c`, and `load<format>.c` (now `<format>.cpp`): `dos`, `dos16m`, `os2` (NE, LX), `flat` (LE/VxD), `pe`, `novell`, `phar`, `qnx`, `elf`, `raw`, `rdos` | the output file and each executable format |
| `map` | `mapio.c` (now `map.cpp`) | `option map` |
| `driver` | `wlink.c`, `ideentry.c` | the passes, the IDE entry the program calls (`x86link.cpp`) |
| `support` | OpenWatcom's `orl`, `wres`, `dwarf/dw`, `rc`, `watcom/c`, the demangler | the libraries the linker uses |

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

1. The capture hook (done: `core/wlink_capture`), a corpus from a gate run (49 links:
   `test-inputs/x86link/gate`), and the replay harness.
2. wlink's C source as a library in the one build, run in-process through its IDE entry point; every case byte-identical.
   **Done (2026-11-20):** `src/build/x86link/wlink` (cb_wlink: the objects of wlinkd.dll, copied by work/x86link/vendor.py;
   messages compiled in, cb/msgtable.c), `x86link.cpp` (the host: the IDE callbacks as wlink.exe's driver has them;
   WLINK_LNK names <WATCOM>/binnt64/wlink.lnk), `replay.cpp` + `x86link_replay`: 47 of 47 replayable captured links
   identical (files byte for byte but the PE link time; messages; exit code), 2 not replayable (their captures lack an
   `option resource` file; captures now keep it). One wlink fix: the reset between links now clears the stack address.
3. Every caller on the in-process linker; `wlink.exe` is no longer started.
   **Done (2026-11-20):** process::run asks an in-process hook first (core/process: set_in_process); x86link::install(),
   called at codebreaker.exe's start, takes `wlink`/`wlink.exe`, so every caller (build driver, lanes, writers, owbuild)
   links in-process with no change of its own (CB_WLINK=exe starts wlink.exe again). The gate: 6636/6636, 168/168.
4. The refactoring, module by module (the table above), each re-proven on the corpus: the directives, the OMF reader (onto
   cb/formats/omf), the symbol passes, the layout and fixups, then each format (DOS, LE, NE, PE).
   **4a done (2026-11-20): wlink is C++.** Every file of `src/build/x86link/wlink` is `.cpp`, compiled as C++20 under
   `/permissive-`, and the replay is 47 of 47 the same. What C++ asked to be written (scripts in work/x86link4): C++
   keywords used as names renamed `<word>_` (`class_`, `this_`, `export_`, `new_` ...; 729); an anonymous struct named
   by a typedef that is also a tag given that tag, and `typedef struct x *x` tags made `x_s`; every conversion C made
   silently (`void *` to a typed pointer, an int to an enumeration) a cast where it happens or in its macro; string
   constants pointed at by `const char *`; the command tables' empty flags `CF_NONE`. `cb/cbcxx.h`, force-included,
   gives enumerations C's arithmetic (`|=`, `&`, `++` ...). The source is now the starting point of each module's
   refactoring; `work/x86link/vendor.py` copies OpenWatcom's C and is no longer how these files are made.

   **4b: real typed C++ (Logan, 2026-11-20: "wlink should be REAL typed C++").** C compiled as C++ is the starting
   point only. Each module becomes idiomatic typed C++ (the rules above, docs/CPP rules): `std::string` and
   `std::string_view` for text, containers for lists and tables, `enum class` with flag operators where C had int flags,
   RAII for files and memory, errors returned to the caller instead of `longjmp`/`exit`, and no mutable globals.

   *The proof, widened first.* The gate's 47 captured links use few directives, so a directive corpus was made
   (`test-inputs/x86link/directives`, git-ignored; made by work/x86link4/dircorpus.py): every `system` of wlink.lnk, all
   26 formats, and every option and directive of the grammar `wlink ?` prints, each linked once by OpenWatcom's own
   wlink.exe: 1,930 cases. The in-process linker is the same on 1,748; the other 182 are listed in
   `build/x86link/testdata/directives_known.txt`, with why: state an earlier link in the same process left (ELF and QNX
   headers, PE fields; each case is the same alone) and memory not cleared (CodeView padding, a map line). These are bugs
   of wlink's globals, and the typed rewrite is to empty the list. `x86link.replay.the_directive_corpus_is_the_same_but_
   the_known_cases` fails on any case not listed that differs, and names a listed case that has become the same. Two
   wlink bugs found on the way: `debug watcom` (every format) and `option incremental` (11 formats) crash wlink.exe
   itself (work/x86link4/dircorpus/crashes.txt); the typed linker is to report them as errors instead. Fixed on the way:
   each link in the process now prints its banner (wlink kept `BannerPrinted` across links).

   *The order.*
   1. **The state of a link** (`link.hpp`): every global wlink keeps (LinkFlags, LinkState, FmtData, the object and
      library lists, the symbol table's roots, the message state, the directive parser's) moves into one `Link`, one
      per run, reached by the C code through `current()` while it is still C-shaped. The known list should shrink
      most here: what leaked between links has nowhere to stay.
   2. **The directive reader** (`directives/reader`): the command sources (the command line, @files, environment
      variables, system blocks) as a stack of owned buffers, a `Token` of `std::string_view` text with its source and
      line, backup and restore. It replaces cmdutils' `Token`, `GetToken*`, `NewCommandSource`, `RestoreParser`.
   3. **The directive language** (`directives/`): the 344 table rows as typed tables (`std::string_view` keyword, its
      handler, the formats it is valid for), the matching (`ProcOne`, `MatchOne`) and the handlers of cmdall, cmdline and
      each format's file, writing typed `Options` in the `Link` rather than FmtData's union.
   4. Then the object reader, symbols, layout, fixups and each format, as the table above, each against both corpora.

   **The directive language is typed C++ (2026-11-20; Logan: "All at once").** wlink's cmd*.c (13 files, 9,616 lines with
   their headers) are `src/build/x86link/directives/` (6,466 lines), compiled into cb_wlink:
   - `reader` (Source, the token, the sources' stack: positions into each source's own std::string, `@file`, `%ENV%`,
     `^`, escapes, double-byte; every source read whole) and `language` (typed keyword tables of `Entry`, matching,
     lists with wildcards and `{...}`, numbers, versions; the flags as `enum class Flag`);
   - `state` (what reading a link's directives keeps, one `State` per run of the linker: the reader, the flags, the
     file list, the executable's name, the system blocks, the link commands, what the common directives carry);
   - `common` (cmdall.c), `driver` (cmdline.c), and one file per format: `dos`, `os2` (OS/2, Windows, NT, VxD),
     `novell`, `phar`, `dos16m`, `qnx`, `elf`, `rdos`, `raw`, and `help`.
   The rest of wlink calls the same 18 functions as before (cmdline.h, cmdall.h, cmdnov.h, cmdelf.h); msg.cpp reads the
   message location and loadphar.cpp `runtime realbreak` through `state.hpp`. wlink's headers gained `#pragma once`
   (but globals.h and specials.h, which global.cpp includes twice to define the globals). Both corpora are as before:
   the gate 47 of 47, the directive corpus 1,749 of 1,929 with nothing newly different. One wlink bug fixed on the way:
   the map's "Stub file: none" read the OS/2 family's `no_stub` for every format (FmtData.u is a union), so a Netware
   link with `option messages` had it or not as the heap fell. Two behaviours changed on purpose: every source is
   read whole (the line-by-line reading of command files over 64K is gone), and a link handed no command reads an
   empty one (wlink read the host process's command line: in codebreaker.exe, CodeBreaker's own).

   **All of wlink is typed C++ (2026-11-20; Logan: "Start it. Do it all.").** Seven waves after the directive layer
   (004207493 ... 5b6248071), each proven on both corpora before it landed; work/x86link4/WLINK_PLAN.md has the waves.
   - Waves 1-6: every file of wl/c (64) and of the support libraries (ORL's OMF, COFF and ELF readers, wres, DWARF,
     watcom/c, rc, the demangler) rewritten in place: constexpr tables, enum class flags, std::string / std::array /
     std::vector for local text and buffers, typed casts, no alloca; each file's statics in the state of the run
     (cb/run_state: `cb_wlink::RunState::part<T>()`, made new at each run's start).
   - Wave 7: the 82 globals of a link's state are members of one `cb_wlink::Link` (cb/link.hpp), made new at each
     run's start and reached as `wlink()` (one pointer load: the same speed); the names wlink only reads (the special
     class and group names, the COFF section names, OMF's alignment table, the newline) are constants.
   - Bugs fixed on the way, each where the in-process linker differed from wlink.exe or could fail: the map's
     "Stub file" read through the wrong member of FmtData's union; the import library's temporary file was left when a
     link failed (26 such and every `option implib` failed); CVDebugDirEntryPos and ELF's program-header count kept
     from an earlier link; RDOS's sizes grew from link to link; CodeView's block was not cleared; the TOC's hash key
     read stack bytes; each link printed its banner only once per process.
   - Proof: every link of both corpora is the same in-process as wlink.exe, the gate's 47 and the directive corpus's
     1,930 (directives_known.txt is empty), at wlink's speed. Next (phase 5): the modules into the folders of the table
     above, and `Link&` passed instead of wlink() as each module's functions are reworked.
5. The modules into their folders, and the `Link` passed.
   **5a done (2026-11-20): the folders.** wlink's files are in the module folders of the table above (src/build/x86link:
   link, base, messages, directives, objects, symbols, layout, fixups, debuginfo, formats, map, driver; OpenWatcom's
   libraries under support/), the cryptic names made plain (`loadpe.c` is `formats/pe.cpp`, `mapio.c` `map/map.cpp`,
   `ntio.c` `base/fileio.cpp`, `link.h` `link/flags.h`), every header of the linker included by its path under src/
   (`build/x86link/<module>/<file>`), the build in src/build/x86link/CMakeLists.txt (work/x86link5/folders.py moved
   them). Both corpora as before: the gate's 47 and the directive corpus's 1,930.
   **5b done (2026-11-20): the Link passed.** Every function of the linker that reads or changes the link's state is
   given it, `Link& link` first, and every call passes it on (1,177 functions; the directive handlers too: `Handler` is
   `bool (*)(Link&)`); a run's `Link` is made by `InitSubSystems` (RunState::begin_run returns it). Found from clang's AST
   of every file and rewritten by it (work/x86link5: astdump.py, analyze.py, rewrite.py). What still reaches it through
   `wlink()` is one line, `Link& link = wlink();`, first in each of the 158 functions whose signature is fixed for now:
   callbacks given to walkers by address (RingWalk, CarveWalkAll, WalkAllSects, OrderGroups ...), and the few the support
   libraries call (LnkMsg, MemAlloc, the resource callbacks); next, the walkers take callables and those functions the
   Link. Both corpora as before, at the same speed (CPU, the binaries alternated).
   **5c (2026-11-20): the walkers.** A walker takes its routine as `FunctionRef<F>` (link/callback.hpp: a reference to
   anything callable as F) where wlink took `F *`, and a routine that needs the link's state takes `Link&` first and is
   handed as `bind_link(link, f)` (its arguments converted to the routine's types, as wlink's casts had them): the
   rings', carvers', hash tables', sections', areas', modules' and leaders' walks, the DBI scans, OrderGroups, the
   relocation and string-table walks (31 walkers, 108 routines; work/x86link5/slotify.py, hand5c.py); the tables of routines take it too: the
   directive formats' set-up and free, the object formats' skip and pass 1, the debugging formats' pass-1 routines, the
   virtual function lists', permdata's block preparation (hand5c3.py). What is left of wlink() is the boundary, 26
   functions that bind `Link& link = wlink();` first because something outside the linker calls them with a
   signature of its own: the IDE entry points (the host), the callbacks OpenWatcom's libraries make (ORL's reader and
   scans, wres's file routines, the rc merger's messages and copies, the memory tracker's allocations, LnkMsg), and
   qsort's comparison (its order is wlink's). Both corpora as before, at the same speed.
   **5d (2026-11-21): the boundary.** `wlink()` and the pointer behind it are gone: nothing holds the Link globally.
   The host's DLL handle is the Link; ORL's note callbacks get it as their cookie, its reader's io handle holds it and its
   allocator is a `FunctionRef` holding it; wres's file and allocation routines and the rc merger take it; `LnkMsg` and the
   allocators take it first (194 more functions); the symbol sort picks its comparison by the link's flag at the call.
   Both corpora as before, at the same speed.
