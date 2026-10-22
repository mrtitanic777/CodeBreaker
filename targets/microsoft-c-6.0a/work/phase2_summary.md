# Phase 2 - inventory / call graph / ASM floor (summary)

| Pass | Functions | Image | Code bytes | Call edges | Indirect br |
|---|---:|---:|---:|---:|---:|
| CL | 209 | 0x7160 | 0x5ce1 (81%) | 681 | 18 |
| C1 | 1016 | 0x24f70 | 0x1fa0e (85%) | 1669 | 53 |
| C1L | 982 | 0x316f0 | 0x2d49e (91%) | 1336 | 52 |
| C2 | 991 | 0x480e0 | 0x3ac07 (81%) | 1352 | 62 |
| C3 | 951 | 0x25160 | 0x1ec39 (82%) | 1498 | 105 |

Artifacts per pass in `work/<pass>/`: `functions.json` (starts, sizes, calls, edges, relocs,
indirect sites) and `floor.asm` (faithful Capstone byte-exact decode; `db` = reloc/data byte).

Caveats: leaders come from a linear sweep (direct call targets + `55 8B EC` prologues), so the
function count is an upper bound (some false positives in data). Byte-exact **round-trip**
verification (re-assembly) is not yet wired - needs MASM/nasm-masm config.
