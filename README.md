# CodeBreaker

Reconstructs readable, maintainable, compilable source from binaries, and checks that what it
wrote behaves like the original.

A reconstruction is only counted when it has been compiled, run next to the original in an
emulator on the same inputs, and observed to do the same things: the same calls with the same
arguments, the same return value, the same memory left behind, the same stack and registers.
Byte-identical output is not the goal.

## One of each

```
codebreaker/
  base/settings.py            where things are on this machine (settings.json)
  base/corpus.py              the original binaries: find one, identify it
  base/parallel.py            keeping worker processes polite
  formats/                    reading executables: pe.py, ne.py, pe_functions.py, detect.py
  decompile/describe.py       take a file apart and describe it to the decompiler
  decompile/decompiler.py     THE decompiler: run the native program on a description
  decompile/c_text.py         make its output C that compiles and means what the original meant
  build/functions.py          THE builder: compile each function, link a DLL
  build/watcom.py             the compiler it uses
  emulate/emulator.py         THE emulator: run a function, record what it did
  symbols/api_arguments.py    how many bytes of arguments an imported function takes
  verify/compare.py           THE comparer: do two builds of a function behave alike?
  verify/reconstruct.py       THE path: decompile, build, run, compare, record
  verify/win32.py, win16.py   what differs per kind of module (a new kind is one such file)
  verify/sweep.py, measure.py the corpus, and one module
  store/functions.py          THE store: every function, every reconstruction, every verdict
  jobs/pool.py                THE runner: work spread over processes, surviving a crash
  source/module_writer.py     the reconstructed module as a source file, and the scoreboard
native/decompiler/            the native decompiler (C++): decompiler.cc around the Ghidra
                              decompiler core in core/ (Apache-2.0, see core/LICENSE, core/NOTICE)
tests/                        python tests/run_all.py -- run before every commit
tools/                        measuring and keeping the code base itself in order
migrate/                      carries modules over from the previous layout
```

Kinds of code handled: 32-bit Windows modules and 16-bit Windows modules are decompiled,
rebuilt and judged; PowerPC images are decompiled (not yet rebuilt or judged).

## Setup

Copy `settings.example.json` to `settings.json` and fill in where the original binaries and the
OpenWatcom compiler are. Results and caches go to `state/`. Neither is committed.

## Use

```
python -m codebreaker module comctl32.dll      decompile, build, judge, write the module's source
python -m codebreaker measure commdlg.dll      the same without writing to the source tree
python -m codebreaker decompile user.exe       just the C, for any module (32-bit, 16-bit, PowerPC)
python -m codebreaker sweep [--16 | --all]     every module of the corpus, resumably
python -m codebreaker scoreboard               rewrite the scoreboard from stored results
```

## Keeping it one of each

The test gate fails if a module does not import, imports something that is not there, or if
anything in the package is unused. The tools that keep it so:

```
tools/tidy.py --apply      remove unused imports, unused definitions, unreachable modules
tools/carve.py --apply     remove what a real run never calls and nothing live refers to
tools/merge.py A B         fold module A into module B and point every user at B
tools/move.py A B names    move definitions from A to B
tools/sample.py 16 14      a quick reading across several modules
tools/causes.py 16|32      why functions are not verified, across all judged modules
tools/failures.py MODULE   the same for one module
tools/trace16.py M F       one 16-bit function, original and rebuilt, side by side
```

## Where this came from

A restructuring of an earlier checkout that had grown by accretion. `migrate/port.py` carries a
module over under a name that says what it does; `migrate/ported.json` records what came from
where. Once a module is here, it is edited here.
