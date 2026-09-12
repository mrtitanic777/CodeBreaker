# SDK/DDK Ingestion & Byte-Exact Assist (Phase 3)

Point CodeBreaker at an SDK/DDK (headers, libs, binaries, docs, toolchains) and it
(1) inventories the tree, (2) auto-detects the toolchain and maps it to backbone
adapters, and (3) parses headers/`.def`s into **typed signatures** that feed the
MSC-6.0a inverse — the direct accuracy multiplier for byte-exact reconstruction.

## Pipeline

```
ingest.scan(root)  ─►  Inventory{headers, defs, libs, binaries, tools, sources, docs}
        │
        ├─ detect.identify_tools / detect.plan ─► toolchain -> backbone adapter names
        │
        └─ apidb.ApiDb.from_paths(headers, defs) ─► {NAME: {ret, params, ordinal, module}}
                    │
                    └─ assist.invert(db, name, code) ─► msc6dec with the REAL signature
                                                        ─► byte-exact C
```

## Why it raises accuracy

`msc6dec` recovers byte-exact C **when it has the right parameter layout** — far
params take two stack words, longs take two, ints one, and pascal order decides
which slot is which. Guessed from `retf n` alone the layout is coarse (all-int).
The SDK header carries the truth: `LZSeek(HFILE, long, int)` → `[('hf','int'),
('lOffset','long'),('iOrigin','int')]`, `LZOpenFile(LPSTR, OFSTRUCT far*, WORD)` →
`[far, far, int]`. `assist.invert` hands msc6dec that signature, and the recovered
C recompiles to the shipped bytes. Verified end-to-end in `assist._selftest`.

## No fabrication

- `apidb.classify` returns `'unknown'` for a type it can't map; `apidb.unknowns()`
  lists every one for hand review — it never force-fits a kind.
- `detect.identify_binary` returns `adapter=None` when a tool's version banner can't
  be read (e.g. MSC 6.0's `CL.EXE` is a stub — the banner lives in `C1/C2.EXE`);
  `detect.plan` surfaces those under `_unidentified` rather than guessing a version.
- `detect.plan` cross-checks each choice against the backbone: a detected tool with
  no registered adapter (e.g. `msvc-1.52c`) is reported `available=False`, not silently
  swapped for a different compiler.

## CLI

```
python -m codebreaker.toolchain.sdk scan   <root>                 # or: wt sdk scan <root>
python -m codebreaker.toolchain.sdk detect <root>                 # tool -> adapter identification
python -m codebreaker.toolchain.sdk plan   <root>                 # chosen toolchain vs. backbone
python -m codebreaker.toolchain.sdk api    hdr.h --def mod.def     # prototypes -> typed signatures
```
