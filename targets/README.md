# Targets

Programs being taken back to source, with their research notes: what was found, signatures, maps of
the original sources, results. The programs themselves are never here -- they are copyrighted --
and live in the gitignored `binaries/<target>/` (see docs/NOT_IN_REPO.md).

| Folder | What |
|---|---|
| `microsoft-c-4.0/` | Microsoft C 4.0 (1986): MSC, CL, C1-C3, LINK, LIB, CodeView |
| `microsoft-c-5.0/` | Microsoft C 5.0 (1987) and QuickC |
| `microsoft-c-6.0a/` | Microsoft C 6.0a (1990): CL and its passes C1, C1L, C2, C3 |

## Doing the work with CodeBreaker

The notes mention scripts each target once carried; their work is CodeBreaker's now:

| The job | CodeBreaker |
|---|---|
| files out of the distribution's floppy images | `codebreaker media extract <image> <folder>` |
| a packed program (EXEPACK, PKLITE ...) unpacked to a plain MZ file | `codebreaker info <program> --unpack <out.exe>`; every command reads packed programs unpacked anyway |
| where the functions are, call graph, strings | `codebreaker dos-functions <program>`; in a project, `codebreaker exact <project> carve` |
| library functions named against the kit's own `.LIB`s | `codebreaker exact <project> library`; `codebreaker identify --learn` for signatures |
| hand-written runtime helpers (`__chkstk` ...) | the toolchain profile and runtime signatures (`codebreaker origin <program>`) |
| functions several programs share (a source module linked into each) | `codebreaker versions --shared <program> <program> ... [--level shape]` |
| the period compiler as an oracle, under DOSBox-X | `codebreaker exact --compile msc500 FILE.c [switches]` (`--toolchains` lists them) |
| a function's object code against the program, masked | `codebreaker exact <project> ledger` |
| functions written back as C by the compiler's inverse model | `codebreaker exact <project> draft`; `codebreaker invert <program>` |

A target becomes an `exact` project with a `project.json` naming its program and toolchain profile
(codebreaker/exact/project.py, docs/EXACT.md).
