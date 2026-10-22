# Xbox 360 executables (.xex)

A `.xex` wraps a PowerPC PE image -- the basefile -- in a header of its own: the image is
compressed (LZX, or in blocks) and usually encrypted with a per-title key, and the libraries its
imports come from are named in the header rather than in the image. CodeBreaker's XEX engine
(`native/xex`, C++17, built with CMake) reads, converts, patches and re-signs it.

## On CodeBreaker's path

Any command takes a `.xex` where it takes a module: `corpus.retail` / `corpus.resolve` hand every
stage the basefile instead (`codebreaker/formats/xex.py`), extracted once under
`state/unpacked/xex/<name>-<hash>.exe` with the header beside it (`.json`). From there it is a
PowerPC image like any other (`verify/powerpc.py`): decompiled, rebuilt, run beside the original.

What the header adds over the basefile alone:

- **Import libraries.** The image numbers its imports by library (`import0_ord_16`); the header
  says which library each number is. `codebreaker module` names the calls from it
  (`source/module_writer.py`, `symbols/console_ordinals.json`) instead of assuming
  kernel-then-system-library.
- **Static libraries.** What the image was built with, with versions (D3D9, LIBCMT, XAPILIB, the
  linker): the SDK and toolchain, for identifying runtime and SDK code.

## Building

```
cmake -S native/xex -B native/xex/build -G "Visual Studio 18 2026"
cmake --build native/xex/build --config Release --target xex
```

`xex.exe` lands in `native/xex/build/bin` (settings.json `xex_bin` to put it elsewhere).
`test_lzxenc` and `test_sign` are hand-run byte-identity harnesses against samples kept outside
the repository (`native/xex/_re/`, gitignored).

## Commands

`codebreaker xex <command> <file.xex> [args]`:

| Command | |
|---|---|
| `info`, `list` | summary; libraries, resources, sections |
| `extract <file> [out]` | the decrypted, decompressed basefile |
| `resources <file> [dir]` | embedded resources |
| `xml <file> [opts]` | machine-readable XML (opts of a/b/d/m/p/r/t/x) |
| `idc <file> <out.idc>` | an IDA script |
| `compress <file> <out> [enc\|unenc]` | LZX-compress |
| `decompress`, `binary` | uncompressed basic, flat binary basic |
| `encrypt`, `decrypt` | toggle encryption |
| `machine <file> <d\|r> <out>` | force devkit or retail |
| `remove-limits <file> <opts> <out>` | a=all m=media r=region z=media id b=bound path d=device id l=min library |
| `bounding-path <file> <path> <out>` | add a bounding path |
| `patch <base> <patch.xexp> <out>` | apply a delta patch |
| `special <file> [mask] [out]` | per-system-file patches -- refuses, see below |
| `selftest` | crypto known-answer tests |

Every write produces the canonical file, checked against retail games, homebrew and console
system files (`xam.xex`, `xbdm.xex`).

### Known gaps

- **`special` is incomplete.** The patch logic is solved and the patched data region is exact,
  but changing the basefile image means recomputing the XEX page-hash chain, which is not worked
  out yet; the command refuses rather than write a file that fails console verification.
- **`remove-limits` i/y/v/k/c** (console, date, keyvault, revocation) are unmapped: no sample
  carries those limits, so there is nothing to check against. They are ignored, not guessed.
- **`patch`** is exact across the 18-sample delta corpus (xeBuild 6717, `xam` included) and a
  retail title update (0 of 35,127,296 bytes differ). It writes the patched image plain, not in
  the input's encryption and compression, so compare basefiles, not the wrappers.

## Notes on the format

Recorded nowhere else:

- **Canonical header layout.** The security info sits at `0x18 + optional_header_count*8 + 0x80`,
  not a fixed `0x120`. Blobs are packed at their exact size in key order; the import-libraries
  blob sits flush against the end of the header, and
  `data_offset = round_up(other_blobs_end + import_size, 0x1000)`.
- **Import digest chain.** Each import library stores the SHA-1 of the next library's block
  (skipping that block's 4-byte size field); the last is zero and the head is in
  `ImageInfo.ImportDigest`. Built back to front.
- **Basic format.** Zero regions are elided on a 32 KB granule; the stored payload is zero-padded
  to a whole granule after encryption.
- **Devkit signing.** `sig = (PSS · R²)^d mod N` (R = 2^2048, e = 3), every bignum qword-reversed;
  the PSS DB mask is an RC4 keystream keyed by H, not MGF1.
- **`machine`** re-wraps the stored title key whether or not the payload is encrypted (guarding
  it on encryption left the key wrapped under the source machine's key, at `sec+0x150`).
- **`patch`** reads the base header up to `baseHeaderSize`, not `headerTargetSize`, and in the
  LZX path writes the base image at the new header size; both only show when a patch changes the
  header size.

Copies of games and system files never enter the repository (`native/xex/.gitignore`).
MIT -- `native/xex/LICENSE`.
