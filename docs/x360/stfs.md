# STFS packages (CON / LIVE / PIRS)

The Xbox 360's package format: saves, profiles, themes, gamer pictures, Xbox LIVE downloads (Arcade games, add-ons,
title updates) and the system update's packages. `native/x360/system/stfs` reads, verifies and writes them:
`stfs/format.h` (the layout, as arithmetic on bytes), `stfs/package.h` (reading), `stfs/builder.h` (writing),
`stfs/sign.h` (signatures). The dashboard mounts a package as `game:` (kernel/stfs_mount.cpp) and lists dropped
packages in My Games (kernel/game_import.cpp). Block layout as in Xenia's StfsContainerDevice (BSD-3).

## The three kinds

| magic | signed by | what it is |
|---|---|---|
| `CON ` | a console (its own 1024-bit key) | saves, profiles, content a console made or copied |
| `LIVE` | Microsoft (2048-bit) | Xbox LIVE downloads: Arcade games, add-ons, title updates |
| `PIRS` | Microsoft (2048-bit) | system and offline content (system update packages, demos on discs) |

Only CON packages are ever written. A LIVE or PIRS package moves between devices as a byte copy
(`stfs::CopyPackage`); turning one into CON (the builder's metadata magic set to CON) clears its Microsoft
signature area and makes its license unrestricted.

## Header

Big-endian unless noted. Offsets from the start of the file (`stfs::offsets`):

| offset | size | field |
|---|---|---|
| 0x000 | 4 | magic |
| 0x004 | 0x1A8 | CON: the console's certificate (it holds the public key) |
| 0x1AC | 0x80 | CON: the console's signature, byte-reversed |
| 0x004 | 0x100 | LIVE / PIRS: Microsoft's signature |
| 0x22C | 16 x 0x10 | licenses; **the signed region [0x22C, 0x344) starts here** |
| 0x32C | 20 | header hash: SHA-1 of [0x344, the first hash table) |
| 0x340 | 4 | header size |
| 0x344 | 4 | content type (0x1 saved game, 0x2 marketplace, 0x4000 installed game, 0x8000 storage pack, 0x9000 avatar item, 0x10000 profile, 0x20000 gamer picture, 0x30000 theme, 0x40000 cache file, 0xA0000 game title, 0xB0000 installer) |
| 0x348 | 4 | metadata version (1 or 2) |
| 0x34C | 8 | content size |
| 0x354 / 0x358 / 0x35C / 0x360 | 4 each | media ID, version, base version, title ID |
| 0x364..0x367 | 1 each | platform, executable type, disc number, discs in set |
| 0x368 | 4 | save game ID |
| 0x36C | 5 | console ID |
| 0x371 | 8 | profile ID |
| 0x379 | 0x24 | volume descriptor (below) |
| 0x39D / 0x3A1 | 4 / 8 | data file count / size |
| 0x3A9 | 4 | volume type: 0 STFS, 1 SVOD |
| 0x3AD | 8 | online creator |
| 0x3B5 | 4 | category |
| 0x3D9 | | metadata v2: series ID (0x10), season ID (0x10), season / episode (u16 each) |
| 0x3FD | 0x14 | device ID |
| 0x411 | 9 x 0x100 | display names (UTF-16BE, 128 characters each, by language) |
| 0xD11 | 9 x 0x100 | descriptions |
| 0x1611 / 0x1691 | 0x80 each | publisher, title name |
| 0x1711 | 1 | transfer flags |
| 0x1712 / 0x1716 | 4 each | thumbnail size, title thumbnail size |
| 0x171A | 0x4000 | thumbnail (PNG; v2: 0x3D00, then 3 more display names at 0x541A) |
| 0x571A | 0x4000 | title thumbnail (v2: 0x3D00, then 3 more descriptions at 0x941A) |
| 0x971A | | end of the metadata (the library's own header size; the first hash table is at 0xA000) |

Metadata version 2 has 12 languages (the 9 of v1 and 3 more, in the thumbnails' last 0x300 bytes each).

## Volume descriptor (0x379, 0x24 bytes)

| offset | field |
|---|---|
| 0 | length (0x24) |
| 1 | version |
| 2 | flags: bit 0 read-only format (one copy of each hash table), bit 1 root active index (which copy is current) |
| 3 | file table blocks (LE16) |
| 5 | file table's first block (LE24) |
| 8 | top hash (SHA-1 of the top hash table) |
| 0x1C | total data blocks allocated |
| 0x20 | free data blocks |

## Blocks and hash tables

Everything after the header is 4 KiB blocks. A *backing block* is counted from the first hash table (the header size
rounded up to 4 KiB); *data blocks* are numbered without the hash tables between them.

A hash table is 170 entries of 24 bytes: the SHA-1 of a block (20 bytes) and an info word (BE u32: its status and,
for a data block, the next block of its file, 0xFFFFFF ending the chain). Level 0 covers 170 data blocks, level 1
covers 170 level-0 tables (28 900 blocks), level 2 covers 170 level-1 tables (4 913 000 blocks, the most a package
has). The layout interleaves them:

```
level-0 table 0, data 0..169,
level-1 table 0, level-0 table 1, data 170..339, level-0 table 2, data 340..509, ...
(after the first 28 900 data blocks) level-2 table, level-1 table 1, ...
```

A read-write package (flags bit 0 clear) keeps two copies of every table side by side; the root active index (and
each table entry's own bits) says which copy is current. System packages use the read-only format. The top hash in
the volume descriptor covers the highest table present (`stfs::TopHashLevel`).

## File table

The file table is a chain of data blocks starting at the descriptor's block; each holds 64 entries of 0x40 bytes:

| offset | field |
|---|---|
| 0x00 | name (0x28 bytes, ASCII / CP-1252) |
| 0x28 | name length (low 6 bits) and flags: 0x40 contiguous, 0x80 directory |
| 0x29 / 0x2C / 0x2F | valid blocks, allocated blocks, first block (LE24 each) |
| 0x32 | parent entry (BE16; 0xFFFF the root) |
| 0x34 | size (BE32) |
| 0x38 / 0x3C | updated / accessed (FAT date in the high 16 bits, time in the low) |

An entry with no name is an empty slot. A contiguous file's blocks follow one another; any other file's blocks follow its
chain through the level-0 hash entries.

## Verifying

`Package::Verify` reports, and reading never depends on it:
- each data block's SHA-1 against its level-0 entry, each table against the entry above it, the top table against the
  descriptor's top hash;
- the header hash (0x32C) against [0x344, the first hash table);
- the signature (below).

## Signatures

A console signs [0x22C, 0x344) (licenses, header hash, header size) with its 1024-bit RSA key: PKCS#1 v1.5 with SHA-1,
the 0x80-byte result stored byte-reversed at 0x1AC; its certificate (0x1A8 bytes, from its keyvault) at 0x004 holds
the public key that verifies it (`stfs::VerifySignature`: Valid, Invalid, Unsigned when the area is zero, NotChecked
for LIVE / PIRS, which are Microsoft's and which the library cannot check).

The library carries no key material and never looks for any. A key is the caller's: `ConsoleKey::FromXeCrypt` takes
the XECRYPT_RSAPRV_1024 (0x1D0 bytes) and the certificate from the user's own decrypted keyvault;
`GenerateConsoleKey` makes a stand-in console of the library's own (a certificate for its public key, retail type, no
Microsoft signature) for a runtime run without one.

## Writing

`stfs::Builder` makes a new CON package from metadata, or takes over an existing package's header and files and
changes them (add, replace, delete; paths case-insensitive, names up to 40 characters, parents created as needed).
`Save` writes the whole file again -- data blocks, every hash level, the top hash, the header hash -- to
`<out>.tmp` and renames it over `<out>`, so a package can be saved over itself. With `SaveOptions::key` the header is
signed; without, the signature area keeps what it had (a new package: zero, unsigned). `readOnlyFormat` writes one
copy of each table (as system packages are); the default is the read-write format consoles write for saves.

## The library's work and tests (2026-10-18)

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
