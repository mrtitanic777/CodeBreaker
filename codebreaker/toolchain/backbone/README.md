# Toolchain API Backbone (Phase 2)

One uniform interface per tool **role** so the reconstruction pipeline swaps a
compiler / assembler / linker / disassembler by *name* instead of rewriting call
sites. Every concrete tool is an **adapter** registered in a process-wide
`Backbone`; callers ask for a role and get a live adapter with a single method.

```python
from engine.backbone import BACKBONE as bb

bb.compiler().compile(src, flags="/AM /G2 /Gsw /Ox")   # default 16-bit compiler
bb.compiler("msc-6.0a").compile(src)                   # by name
bb.disassembler("capstone-x86-16").disasm(code)        # -> [Insn]
bb.linker(name=None).link(objs, defs=..., libs=...)    # default = chained-reloc LINK 5.20
bb.detect("linker", reloc="chained")                   # pick by hint
bb.catalog()                                           # every adapter + availability
```

## Roles and the one method each exposes

| Role          | Method                                   | Result carries            |
|---------------|------------------------------------------|---------------------------|
| `compiler`    | `compile(source, *, flags=None)`         | `.text` = extracted _TEXT |
| `assembler`   | `assemble(source, *, flags=None)`        | `.output` = OBJ bytes     |
| `linker`      | `link(objs, *, defs, libs, opts, response)` | `.output` = NE/exe bytes |
| `disassembler`| `disasm(code, *, base=0)`                | `list[Insn]`              |

Every call returns a `ToolResult` (`ok`, `output`, `text`, `stdout`, `stderr`,
`meta`); on failure `ok is False` and `stderr` holds the reason — never a
substituted tool, never a guess (same no-fabrication contract as the inverse
engine's oracle).

## Registered adapters

- **compiler** — `msc-6.0a` (wraps `inverse_engine.msc6_oracle`; the compiler that
  built Win95's 16-bit DLLs). MSVC 1.52c / 2.0 present in the manifest, adapters TBD.
- **assembler** — `masm-5.1` (default), `masm-6.13`.
- **linker** — `link-5.20` (default; the shipped 0xFFFF chained relocations),
  `link-5.60` (direct-offset relocs, needs DOSXNT.EXE).
- **disassembler** — `capstone-x86-16` (default), `capstone-x86-32`.

`available()` is honest: an adapter whose tool the manifest marks a gap, or that
needs an unconfigured DOSBox-X, reports `False` and is skipped by auto-select.

## DOS-hosted tools share one runner

`backbone/dosbox.py::run_dos(mounts, bat_lines, artifacts, files=...)` factors out
the mount + BAT + collect-artifacts plumbing that the compiler/assembler/linker all
need. A new DOS-hosted tool is a BAT template, not another copy of the plumbing.

## Adding a tool

Drop a file in `adapters/` with a `register(backbone)` function that instantiates
your `Compiler`/`Assembler`/`Linker`/`Disassembler` subclass, and list its module
in `__init__._ADAPTERS`. No call-site changes. Optional `score(**hints)` opts the
adapter into `detect()`.

## CLI

```
python -m engine.backbone list                 # or: wt backbone list
python -m engine.backbone detect linker reloc=chained
python -m engine.backbone compile file.c /AM /G2 /Gsw /Ox
python -m engine.backbone disasm file.bin 16
```
