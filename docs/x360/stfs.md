# STFS packages: format and the shared library (`runtime/stfs/`)

Status (2026-10-18)
- **Done**: library in `runtime/stfs/` (read, verify, write CON, sign with a caller-supplied key), standalone tests
  (`runtime/stfs/test/build.bat`: 450 checks pass; the 5 system update packages read and verify, and match
  `tools/scripts/stfs_extract.py` entry for entry and byte for byte).
- **Not verified**: (1) a package written by the library on a real console (not yet tried), (2) a signature made by
  the library on a stock console (no console key was used; signing was tested with a generated key only), (3) the
  read-write (two tables) format against a console-made package (none was on this PC; the format follows Xenia /
  Velocity / LCEStudio), (4) LIVE / PIRS signatures (Microsoft's key is not carried; reported "not checked").
- **Next**: the dashboard's device layer and Copy / Move (`storage_devices.md`, pieces 2-4) use it.

Tags: **SHOWS** = seen in real packages (the system update's PIRS packages su20076000_00000000, FFFE07DF00000001 /
02 / 06 / 08, read 2026-10-18) or produced and checked by the tests; **INFER** = from the references (Xenia's
StfsContainerDevice, BSD-3; `../XboxTranspiler/docs/STFS-CON-Resigning.md`; `../LCEStudio/lce/core/stfs.py`;
`../NexGen/nexgen/tools/stfs2.py`, whose block addressing and parent field disagree with the real packages) without
a real package showing it.

## Format

### File
`[header, 0 .. headerSize rounded up to 4 KiB][backing blocks of 4 KiB: hash tables and data blocks]`. Everything in
the header is big-endian except where noted. (SHOWS)

### XContent header
| Offset | Size | Field |
|---|---|---|
| 0x000 | 4 | magic: `CON ` (console-signed), `LIVE`, `PIRS` (Microsoft-signed). SHOWS (PIRS) |
| 0x004 | 0x1A8 | CON: console certificate (cert size 0x1A8, console ID, part number, type, date, public exponent at 0x028, 1024-bit modulus at 0x02C in BnQw form, Microsoft's signature over it). INFER |
| 0x1AC | 0x80 | CON: console signature (below). INFER |
| 0x004 | 0x100 | LIVE / PIRS: Microsoft's 2048-bit signature (0x104..0x22C padding). INFER |
| 0x22C | 0x100 | 16 license entries (u64 ID, u32 bits, u32 flags); the signed region starts here. The system packages have one entry, ID FF x 8 (unrestricted). SHOWS |
| 0x32C | 0x14 | header hash = SHA-1 of [0x344, headerSize rounded up to 4 KiB). **SHOWS** (all 5 match the rounded end, none the unrounded) |
| 0x340 | 4 | header size (0xAD0E in all 5 system packages; 0x971A in console content, INFER) |
| 0x344 | 4 | content type (0xB0000 installer = su*, 0x8000 storage pack = FFFE07DF*; 1 saved game, 0x10000 profile, 2 marketplace). SHOWS for the two seen |
| 0x348 | 4 | metadata version (0 for su*, 2 for FFFE07DF*). SHOWS |
| 0x34C | 8 | content size = bytes of all backing blocks (file size - rounded header). **SHOWS** (all 5) |
| 0x354 | 0x18 | media ID, version, base version, title ID (0x360), platform, executable type, disc number, discs in set, save game ID |
| 0x36C | 5 | console ID |
| 0x371 | 8 | profile ID (owner XUID) |
| 0x379 | 0x24 | volume descriptor (below) |
| 0x39D | 4 + 8 | data file count, combined size (`.data\` folders; 0 here) |
| 0x3A9 | 4 | volume type: 0 STFS, 1 SVOD (the library refuses SVOD) |
| 0x3AD | 8 + 4 | online creator, category |
| 0x3D9 | 0x24 | v2: series ID, season ID, season / episode numbers (kept as raw bytes) |
| 0x3FD | 0x14 | device ID |
| 0x411 | 9 x 0x100 | display names (UTF-16BE, 128 chars, one per language) SHOWS ("System Update", "ParityPack", ...) |
| 0xD11 | 9 x 0x100 | descriptions |
| 0x1611 / 0x1691 | 0x80 each | publisher ("Microsoft" SHOWS), title name |
| 0x1711 | 1 | transfer flags (0 here; meaning of the bits not needed yet) |
| 0x1712 / 0x1716 | 4 each | thumbnail / title thumbnail sizes |
| 0x171A / 0x571A | 0x4000 each | thumbnails (PNG); v2: 0x3D00 each, then 3 more display names at 0x541A and 3 more descriptions at 0x941A |
| 0x971A | | end of the metadata; what follows up to headerSize (to 0xAD0E in the system packages) is kept as raw bytes |

The offsets not tagged are INFER (Xenia's XContentMetadata), and `ParseMetadata` -> `WriteMetadata` reproduces each
real header byte for byte (SHOWS, test).

### Volume descriptor (0x379)
| +Off | Size | Field |
|---|---|---|
| 0 | 1 | length 0x24 (SHOWS) |
| 1 | 1 | version 0 (SHOWS) |
| 2 | 1 | flags: bit 0 read-only format (one copy of each hash table), bit 1 root active index (which copy of the top table is current, read-write format) |
| 3 | 2 LE | file table block count |
| 5 | 3 LE | file table first block |
| 8 | 0x14 | top hash: SHA-1 of the top-level hash table (all 4 KiB) **SHOWS** |
| 0x1C | 4 | total (allocated) data blocks |
| 0x20 | 4 | free data blocks |

The system packages are read-only format (flags 1) SHOWS. Console-made saves are read-write format (flags 0) per
LCEStudio, which reads and writes them (INFER here).

### Blocks and hash tables
Data blocks are numbered without the hash tables between them. A hash table is one 4 KiB block of 170 entries of 24
bytes: SHA-1 of the block (or table) below, then a u32 "info". Level 0 covers 170 data blocks, level 1 170 level-0
tables (28900 blocks), level 2 170 level-1 tables (4913000 blocks, the maximum). The top level is the lowest one that
covers all data blocks (<= 170: level 0, <= 28900: level 1, else 2); the volume descriptor holds its hash. SHOWS for
levels 0 and 1 (FFFE07DF00000008 has 8000 blocks); level 2 only from the library's own packages (test).

Order of backing blocks (`Layout` in `stfs/format.h`, after Xenia): L0 table 0, data 0..169, **L1 table 0**, L0
table 1, data 170..339, L0 table 2, ... ; after the first 28900 data blocks: L2 table, L1 table 1, L0 table 170, data
28900.... Read-write packages have two copies of every table, side by side (each "table" above is two blocks).
SHOWS for the read-only order (all 5); the read-write order is INFER (and the library's own packages read back).

Level-0 info: bit 31 allocated (0x80 in the status byte SHOWS; Velocity also names 0x40 "previously allocated", 0xC0
"newly allocated"), low 24 bits the next data block of the file (0xFFFFFF end of chain) SHOWS. Level 1/2 info: 0 in
the read-only packages SHOWS; in read-write ones bit 30 selects which copy of the table below is current, and the low
15 bits count free blocks (INFER).

### File table
A chain of data blocks (followed through the level-0 next-block fields), 64 entries of 0x40 bytes each:

| +Off | Size | Field |
|---|---|---|
| 0x00 | 0x28 | name (ASCII / CP-1252, not terminated when 40 chars) |
| 0x28 | 1 | low 6 bits name length; 0x40 contiguous blocks; 0x80 directory |
| 0x29 / 0x2C | 3 LE each | valid / allocated data blocks |
| 0x2F | 3 LE | first data block |
| 0x32 | 2 **BE** | parent: the entry's slot in the file table, 0xFFFF = root (SHOWS: BE gives the right tree; stfs2.py reads it LE) |
| 0x34 | 4 | file size |
| 0x38 / 0x3C | 4 each | update / access time (FAT date in the high 16 bits, time in the low) |

A name length of 0 is an empty slot. A contiguous file's blocks are consecutive data block numbers (the next-block
fields agree, SHOWS); others follow the chain.

### Signatures
- **CON** (INFER, from STFS-CON-Resigning.md, which checked it byte for byte against real console-signed saves):
  PKCS#1 v1.5 RSA-SHA1 over [0x22C, 0x344) with the console's 1024-bit key; the 0x80-byte signature is stored
  **byte-reversed** at 0x1AC; the certificate at 0x004 holds the public key (exponent 0x028, modulus 0x02C as BnQw:
  big-endian qwords, least significant qword first). A console's keyvault holds the certificate (KV 0x9C8, 0x1A8
  bytes) and the private key (KV 0x298, XECRYPT_RSAPRV_1024: cqw, e, reserved, n, p, q, dp, dq, cr). Since the header
  hash is inside the signed region, any change to the metadata or the files invalidates the signature.
- **LIVE / PIRS**: Microsoft's key; a changed package can never carry a valid one again, so tools convert such a
  package to CON and sign it with a console key. Retail consoles check signatures; modded consoles and the PC port do
  not need them.

## The library

`runtime/stfs/stfs.cmake` exports `X360_STFS_SOURCES` / `X360_STFS_INCLUDE_DIR` (link `bcrypt`); headers
`"stfs/<name>.h"`, logging through the runtime's `"log.h"` (`Log`). No third-party code; SHA-1 and RSA through
Windows bcrypt.

- `stfs/format.h`: constants and offsets, `Metadata` / `VolumeDescriptor` / `DirEntry` with parse / write, `Layout`
  (data block and hash table positions), `Sha1`.
- `stfs/package.h`: `Package::Open(path)` -> header (`HeaderBytes()`, the rounded header incl. certificate /
  signature), `GetMetadata()`, `Entries()`, `Find(path)` (case-insensitive, `/` or `\`), `List(dir)`,
  `Read(entry, offset, buffer, size)` (any range), `ReadAll`, `Verify()` -> `VerifyReport` (header hash, top hash,
  every allocated data block, every table, signature status; mismatches listed; reading never depends on it).
  Thread-safe reads. `CopyPackage(from, to)`: the byte copy for moving packages between devices.
- `stfs/builder.h`: `Builder(Metadata)` (new CON) or `Builder::FromPackage(package)`; `AddDirectory`, `AddFile` (bytes)
  / `AddFileFromDisk`, replace = add again, `Remove` (file or directory tree), `GetMetadata()` / `HeaderTemplate()`
  to change fields; `Save(out, {readOnlyFormat, key})` writes a complete new package (file table first, each file
  contiguous, every hash level, top hash, content size, header hash; `<out>.tmp` renamed over `<out>`, so a package
  can be saved over its own file while open). It **refuses to write LIVE / PIRS**; setting the magic to CON converts
  one (Microsoft's signature area is zeroed, the license set to unrestricted). Without a key the CON signature area
  stays as it was (new packages: zero = unsigned; an edited signed package then reports "invalid").
- `stfs/sign.h`: `ConsoleKey` (certificate + n, p, q, e; `FromXeCrypt` decodes the keyvault's struct the caller
  read), `SignCon(header, key)` (writes the certificate and signature, then checks them; a key that does not match
  its certificate is refused), `VerifySignature(header)`. The library carries and looks for no key material.

Not done: SVOD (Games on Demand), PEC (profile-embedded content, other offsets), incremental in-place writes (every
Save rewrites the package; fine for saves and profiles, slow for multi-GB content), keeping a console's block order
when editing (a saved package is laid out afresh).

## Tests (`runtime/stfs/test/`)
`build.bat` (clang-cl from VS 2026 as the dashboard's build.bat; output in `%TEMP%\stfs_test_build`, test packages
in `%TEMP%\stfs_test_work`) builds `stfs_test.exe` and runs:
- `selftest`: create packages in both formats (empty / 1-byte / 1-block / 4097-byte files, nested and empty
  directories, 70 files = two file table blocks, a 173-block file = two hash levels, metadata incl. v2 names and a
  thumbnail), reopen, compare contents and ranged reads across block / table boundaries, verify; edit in place
  (replace, add, remove a directory, change the display name) and re-verify; a 28951-block package (three levels) in both
  formats; corruption (one data byte, one metadata byte) reported exactly; PIRS refused, converted to CON; byte copy
  identical; signing with a bcrypt-generated RSA-1024 key in keyvault form (valid, tampered region / signature
  rejected, mismatched certificate refused, edit without key -> invalid); each real package given: opens, hash chain
  verifies, metadata round-trips.
- `compare.py`: the library's entry list and per-file SHA-1 against `tools/scripts/stfs_extract.py`'s.

Also checked by hand (2026-10-18): `compare.py` on the library's own read-write, read-only, signed and converted
packages (stfs_extract.py reads them the same), and the generated-key signature checked with plain Python big-int
math (the PKCS#1 block decodes with the byte-reversed storage and BnQw modulus), so the CON signing does not only
agree with bcrypt's own verify.
