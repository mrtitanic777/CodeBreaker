# CodeBreaker

Reconstructs readable, maintainable, compilable source from binaries, and checks that what it
wrote behaves like the original.

A reconstruction is only counted when it has been compiled, run next to the original in an
emulator on the same inputs, and observed to do the same things: the same calls with the same
arguments, the same return value, the same memory left behind, the same stack and registers.
Byte-identical output is not the goal.

## Layout

```
codebreaker/
  base/        settings, locations, the active target, spreading work over cores, progress
  formats/     reading executables: PE, NE, LE, CAB
  disasm/      instructions and function boundaries
  emulate/     running original and rebuilt code and recording what it does
  decompile/   getting C out of a function (our decompiler; the embedded Ghidra core)
  recover/     what behaviour says about a function: arguments, structures, globals
  symbols/     names and prototypes from headers, exports and symbol files
  verify/      building a reconstruction and judging it against the original
  source/      writing the reconstructed module and the scoreboard
  targets/     what is specific to one thing being reconstructed (win95)
  ui/          the progress window
native/
  decompiler/  our decompiler (C++)
  ghidra_core/ the Ghidra decompiler core, embedded (Apache-2.0; see its LICENSE and NOTICE)
tests/         python tests/run_all.py -- run before every commit
tools/         checks on the code base itself
migrate/       carries modules over from the previous layout (see below)
```

Dependencies point one way: `targets` and `source` sit on top; `base` and `formats` at the
bottom import nothing above them.

## Setup

Copy `settings.example.json` to `settings.json` and fill in where the original binaries and the
OpenWatcom compiler are. Results and caches go to `state/`. Neither is committed.

## Use

```
python -m codebreaker module comctl32.dll      decompile, build, judge, write the module's source
python -m codebreaker measure comctl32.dll     judge one decompiler on one module; writes nothing
python -m codebreaker sweep pe                 judge every function of every 32-bit module
python -m codebreaker scoreboard               rewrite the scoreboard from stored results
```

## Where this came from

This is a restructuring of an earlier checkout that had grown by accretion: one tool directory
per Windows version, each with its own copies of shared code, and several hundred scripts found
by scanning folders. Modules are carried over by `migrate/port.py`, which gives each one a
single home under a name that says what it does and rewrites its imports;
`migrate/ported.json` records what came from where. Once a module is here, it is edited here.
What has not been carried over yet is still in the old checkout.
