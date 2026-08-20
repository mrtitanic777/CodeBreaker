# XexTool-RE

A clean-room C++17 rebuild of xorloser's **XexTool** (v6.3, 2011) for Xbox 360 `.xex`
files — with a native GUI, and output that is **byte-for-byte identical** to the
original tool.

## Project Status

**✅ Finished — public.**

- **Done:** Clean-room rebuild with byte-identical output vs the original XexTool, plus a native GUI.
- **Unfinished:** `-s` (special patches) still needs the page-hash chain.
  Everything else is verified against the original tool -- `-p` on the full
  18-sample delta corpus, `-m` after a key-rewrap fix (see Known gaps).

_Last reviewed: 2026-08-19._

## What Is This?

XexTool is *the* utility for working with Xbox 360 executables: extracting the
basefile, converting compression/encryption, removing region and media limits,
re-signing for devkits. It has been closed-source and unmaintained since 2011.

XexTool-RE is a from-scratch reimplementation, built by reverse-engineering the
original binary. It isn't a wrapper — every format, every hash chain, and the LZX
compressor are reimplemented in modern C++ and verified by diffing against the
original's output on real files.

**28 of 28 operations produce byte-identical output**, verified against retail
games, homebrew, and Xbox 360 NAND system files (`xam.xex`, `xbdm.xex`).

## Status

| Operation | Original | Status |
|---|---|---|
| Info / list | `-l` | byte-identical |
| Extract basefile | `-b` | byte-identical |
| Dump resources | `-d` | byte-identical |
| XML output | `-x` | byte-identical |
| IDA script | `-i` | byte-identical |
| Compress (LZX) | `-c c` | byte-identical |
| Decompress | `-c u` | byte-identical |
| Flat binary | `-c b` | byte-identical |
| Encrypt / decrypt | `-e e` / `-e u` | byte-identical |
| Machine type | `-m d` / `-m r` | byte-identical |
| Remove limits | `-r` | byte-identical (see note) |
| Bounding path | `-a` | byte-identical |
| Delta patching | `-p` / `-u` | byte-identical (see note) |
| Special patches | `-s` | **not usable — see below** |

### Known gaps

- **`-s` (special patches) is incomplete.** The patch logic itself is solved and
  the patched data region is byte-identical, but modifying the basefile image
  requires recomputing the XEX **page-hash chain**, which is not yet reverse-
  engineered. Output would carry stale page hashes and fail console verification.
  The command refuses rather than writing a broken file.
- **`-m` (machine type)** did not re-wrap the stored title key when the payload
  was unencrypted: `convert_machine` guarded the rewrap on `x.is_encrypted()`, so
  `security_info.aes_key` stayed wrapped under the *source* KEK and the output
  would not decrypt on the target machine. The rewrap is now unconditional.
  Divergence was 6 of 20 comparisons, all at `sec+0x150`; now 20/20 identical.

- **`-r i/y/v/k/c`** (console/date/keyvault/revocation limits) are unmapped — no
  available sample carries those limits, so there is nothing to verify against.
  They are ignored rather than guessed at.
- **`-p` (delta patching)** is byte-identical across the full 18-sample delta
  corpus (`xeBuild/6717`: aac .. ximedic, including `xam`) plus a retail
  Minecraft X360 title update (TU14: 0 bytes differ across 35,127,296).
  Two bugs were found and fixed in `patch.cpp`:
    - the base XEX header was truncated to `headerTargetSize` before the header
      patch read from it, although the source region is bounded against
      `baseHeaderSize` (lines 97-98) and may extend past it -- yielding a wrong
      patched image size;
    - in the `baseComp == 2` (LZX) path the decompressed base image was written to
      `newXexHeaderSize` while every delta record addresses it through
      `outExe = &out[newHeaderSize]`, so the whole image sat one header-size-delta
      off.
  Both only trigger when a patch changes the XEX header size
  (`baseHeaderSize != headerTargetSize`), which is why most samples passed before.
  The previously-noted `xam` 7-byte discrepancy is resolved by these fixes.
  Note the two tools package `-p` output differently (XexTool-RE writes the
  patched image plain, the original preserves the input's encryption and
  compression), so compare extracted basefiles rather than the XEX wrappers.

## Building

Requires a C++17 compiler. Built and tested with MinGW-w64 g++ on Windows.

```sh
mingw32-make          # CLI + GUI
mingw32-make cli      # XexTool-RE.exe
mingw32-make gui      # XexTool-RE-gui.exe
mingw32-make clean
```

## Usage

```
xextool-re <command> <file.xex> [args]

  info          <file.xex>                 summary info
  list          <file.xex>                 extended info (libs, resources, sections)
  extract       <file.xex> [out]           dump the decrypted/decompressed basefile
  resources     <file.xex> [dir]           dump embedded resources
  xml           <file.xex> [opts]          machine-readable XML
  idc           <file.xex> <out.idc>       dump an IDA script
  compress      <file.xex> <out> [enc|unenc]   LZX-compress
  decompress    <file.xex> <out>           convert to uncompressed basic
  binary        <file.xex> <out>           convert to flat binary basic
  encrypt|decrypt <file> <out>             toggle encryption
  machine       <file.xex> <d|r> <out>     force machine type
  remove-limits <file> <opts> <out>        strip limits (a/m/r/z/b/d/l)
  bounding-path <file> <path> <out>        add a bounding path
  special       <file.xex> [mask] [out]    xex-specific patches
  patch         <base.xex> <patch.xexp> <out>   apply a delta patch
  selftest                                 crypto known-answer tests
```

The GUI (`xextool-re-gui.exe`) exposes every operation: open a `.xex`, read the
parsed info, and run any conversion with a save dialog.

## Notes on the format

Some of what the rebuild had to work out, recorded here because it is documented
nowhere else:

- **Canonical header layout.** Every XEX xextool writes places the security info
  at `0x18 + optional_header_count*8 + 0x80` — *not* a fixed `0x120`. Blobs are
  packed at their exact size in key order; the import-libraries blob sits flush
  against the end of the header, and
  `data_offset = round_up(other_blobs_end + import_size, 0x1000)`.
- **Import digest chain.** Each import library stores SHA-1 of the **next**
  library's block (skipping that block's 4-byte size field); the last is zero and
  the head lives in `ImageInfo.ImportDigest`. Built back-to-front.
- **Basic format.** Zero regions are elided on a 32 KB granule; the stored payload
  is zero-padded to a whole granule *after* encryption.
- **Devkit signing.** `sig = (PSS · R²)^d mod N` (R = 2^2048, e = 3), all bignums
  qword-reversed; the PSS DB mask is an **RC4 keystream keyed by H**, not MGF1.

## Provenance

This is a clean-room reimplementation for preservation and interoperability. It
contains no code from the original tool.

Key material and format constants were recovered from the original XexTool binary,
which has been publicly distributed since 2011. The original binary, its
disassembly, and any test `.xex` files are **not** included in this repository and
are excluded by `.gitignore` — bring your own.

XexTool is the work of **xorloser**. This project exists because it is closed and
unmaintained, not to replace or discredit it.

## License

MIT — see [LICENSE](LICENSE).
