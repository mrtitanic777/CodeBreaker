# One description per platform

The roadmap (PLATFORMS.md) says: make the platform explicit, so the next platform adds a
description, not branches. This is that design, and the first platform it brings in.

## Where the platform is decided today

Four places, each its own table, each reading the file again:

| Where | What it decides | How |
|---|---|---|
| `verify/reconstruct.py` `kind_name` / `KINDS` | which lane a file goes down: pe, ne, le, dos, powerpc, java | the file header (`corpus.lane`), then PE machine type 0x1F2 for PowerPC |
| `decompile/describe.py` `_ARCH`, `LE_ARCH` | which processor the decompiler reads it as | PE machine type -> SLEIGH language id |
| `emulate/__init__.py` `RUNNERS` | what runs its code | kind -> runner function |
| `base/corpus.py` `_readable` | what a container is opened to | `.xex` -> its basefile |

The lanes themselves are already the right shape: `verify/<kind>.py` gives `KIND`,
`profile`, `prepare`, `build`, `judge`. What is missing is one object that says, for a
platform, everything those four tables say -- so a stage asks it instead of keeping a table.

## The description

`codebreaker/platforms/<name>.py`, one per platform, each a `Platform`:

```python
PLATFORM = Platform(
    name="win32",                    # what results are filed under (today's KIND)
    title="32-bit Windows",
    family="x86",
    recognise=lambda f: f.pe and f.pe.machine == 0x14C,   # given the opened file
    open=pe_image.open,              # container -> the image the stages read
    cpu="x86:LE:32:default:windows", # SLEIGH language id the decompiler uses
    source="c",                      # what its functions come back as: c | asm | java
    lane="codebreaker.verify.win32", # the kind module: profile, prepare, build, judge
    runner=("codebreaker.emulate.emulator", "trace_function"),
    names=[...],                     # where names come from: ordinal tables, SDK prototypes
)
```

`platforms.identify(path)` opens the file once and returns the one platform that recognises
it (or says which near-misses there were). `reconstruct.kind_name`, `describe`'s language
choice, `emulate.runner` and `corpus._readable` all ask it.

| Platform | CPU | Container | Source | Lane | Runner |
|---|---|---|---|---|---|
| win32 | x86:LE:32 | PE | C | verify/win32 | emulator (Unicorn) |
| win16 | x86:LE:16 (real/protected) | NE | C or asm | verify/win16 | windows16 (Unicorn) |
| vxd | x86:LE:32 ring 0 | LE | asm | verify/driver | driver (Unicorn) |
| dos | x86:LE:16 real | MZ, COM, SYS | asm | verify/dos | (byte-exact rebuild) |
| xbox360 | PowerPC:BE:64 | XEX -> PE basefile | C | verify/powerpc | powerpc (own interpreter) |
| java | JVM | source folder, jar | Java -> C++ | verify/java | the JVM |
| **nes** (new) | 6502:LE:16 | iNES (.nes) | asm (C for cc65 programs) | verify/nes | own 6502 interpreter |

## Where it stands (2026-10-21)

Steps 1-5 are done: `codebreaker/platforms/` describes eight platforms (the six below, the NES,
and the original Xbox, whose XBE goes down the 32-bit lane as a PE image: formats/xbox/xbe.py), and `reconstruct.KINDS`, `kind_name`, `emulate.RUNNERS`, `describe`'s languages and
`corpus._readable` all ask it -- `identify()` agrees with the old rule on every one of the 2,574
files of the Windows 95 corpus and the PowerPC samples. The NES: `formats/nes.py` (iNES and
NES 2.0), the 6502 language (L8 in GHIDRA_FORK.md, with its fixes), `decompile/m6502.py` (the
table, disassembler, assembler), `emulate/m6502.py`, `source/nes_writer.py` (ca65 source and an
ld65 configuration, rebuilt by `build/m6502_asm.py` or by cc65), `verify/nes.py`. A cartridge
goes through `decompile`, `measure` and `module` like any other module. Step 6 (homebrew with its
authors' source as ground truth) waits on downloading those programs and the cc65 suite.

## Order of work

1. **The registry, no behaviour change.** `codebreaker/platforms/` with the six existing
   platforms described, `identify()`, and the four tables replaced by asking it. Proven by the
   test gate staying green and `identify()` agreeing with today's `kind_name` on the whole
   Windows 95 corpus.
2. **6502 in the decompiler.** Ghidra's 6502 SLEIGH spec compiled with the core's own SLEIGH
   compiler into `native/decompiler/languages`; a test that decompiles a hand-written routine.
3. **The NES container.** An iNES reader (header, PRG/CHR banks, mapper number); NROM first
   (mapper 0: 16 or 32 KB of code at $8000, no bank switching), the reset/NMI/IRQ vectors as
   entry points.
4. **Running 6502 code.** Unicorn has no 6502, so a small interpreter of our own in the shape
   of `emulate/powerpc.py`: registers, memory with the PPU/APU registers as recorded I/O, the
   same run record every runner returns.
5. **Back to source and rebuild.** NES programs are almost all hand-written assembly: the
   source is ca65 assembly that rebuilds byte-identically with the cc65 suite, judged like the
   DOS lane; a program built with cc65 gets C for its compiled functions.
6. **Ground truth.** Open-source NES homebrew, built from its own source, as the bench
   (as `tools/oss_bench.py` does for C): reconstruct the binary, compare to the authors' source.
