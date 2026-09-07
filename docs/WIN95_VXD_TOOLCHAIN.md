# The Windows 95 VxD Build Toolchain — Exact Specification

*The precise set of period tools, versions, and invocations that assemble, link, and resource a Windows
95 VxD from source into a loadable image. Companion to [`VXD_FROM_SOURCE.md`](VXD_FROM_SOURCE.md).*

The purpose of this document is to **eliminate toolchain‑version ambiguity**. Every tool below is named
by version, located by path, and invoked verbatim. Substituting a different version of any of these tools
changes the output in ways that range from silent container defects to hard rejection by the loader; the
versions here are the ones proven to produce a booting VxD.

---

## 1. Tools

| Stage | Tool | Version | Host | Location (repo‑relative) |
|---|---|---|---|---|
| Assemble | `ML.EXE` | MASM 6.11 | DOS (DOSBox) | `binaries/toolchains/period/…` (staged by the engine) |
| Link | `LINK.EXE` | **2.60.5046** | Win32 native | `binaries/media/Microsoft Windows 95 DDK/MSVC20/LINK.EXE` |
| Resource compile | `RC.EXE` (+ `RCPP.EXE`) | 3.11 (16‑bit) | DOS (DOSBox) | `binaries/toolchains/period/compilers/msvc-1.52c/BIN/` |
| Resource embed | `ADRC2VXD.EXE` | DDK | DOS (DOSBox) | `binaries/media/Microsoft Windows 95 DDK/BIN/ADRC2VXD.EXE` |
| Resource headers | `version.h`, `common.ver`, `ver.h` | DDK | — | `binaries/media/Microsoft Windows 95 DDK/INC16/` |

The linker's runtime dependencies (`DBI.DLL`, `LINK.ERR`, `IMAGEHLP.DLL`) ship alongside it in `MSVC20/`.

### Why these exact versions

* **LINK 2.60.5046, not LINK 2.50.** LINK 2.50 is the VC++ 2.0 linker. It supports `/VXD` and produces a
  valid LE, but leaves the device id (LE `0xc0`) blank and permutes the page map — both fatal, both
  diagnostic‑free. The DDK shipped LINK 2.60.5046 specifically to fix VxD linking; its `MSVC20\README.TXT`
  states it is for "developers experiencing problems with the MS VC++ 2.0 linker w.r.t. linking VxDs." It
  stamps `0xc0` from the exported DDB and writes an identity page map. It is a native Win32 PE (machine
  i386) and runs directly on 64‑bit Windows.
* **RC 3.11 (16‑bit DOS), not a Win32 RC.** A VxD's version resource is `VOS_DOS_WINDOWS16`; its strings
  are ANSI. The 16‑bit DOS `RC` emits an ANSI `.res` that `ADRC2VXD` accepts. A Win32 `RC` emits a UTF‑16
  `.res` that `ADRC2VXD` rejects ("while finding version info in resource file"). `RC.EXE` is only the
  driver; it spawns `RCPP.EXE` (the preprocessor), which must be present in the same directory.
* **`ADRC2VXD` (DDK), not a hand‑rolled embed.** This is the DDK's own tool for embedding a VxD version
  resource and setting LE `0xb8`/`0xbc`. It does **not** set the device id (`0xc0`); that is the linker's
  job.
* **`LINK386` is not part of this toolchain.** It is the OS/2 linker (OS/2 SDK), targets OS/2 LX, and does
  not produce a Win95 device id. It is not required and not applicable.

---

## 2. Invocations

### 2.1 Assemble (DOSBox)

```
ml.exe -coff -c -DBLD_COFF -DIS_32 -DMASM6 -I<inc32> -Fo<name>.obj <name>.asm
```

`-coff` selects COFF object output (required by LINK 2.60). `<inc32>` is the DDK `INC32` directory,
staged onto the DOS include path.

### 2.2 Link (native, from a space‑free directory)

```
LINK.EXE /VXD /NOD /ALIGN:0x1000 /OUT:<name>.vxd /DEF:<name>.def <name>.obj
```

* `/VXD` — produce a VxD (LE); read the DDB via the `.def` export and stamp the device id.
* `/NOD` — no default libraries.
* `/ALIGN:0x1000` — 4096‑byte LE pages (VKD's page size). Omitting `/ALIGN` yields LINK 2.60's default of
  512‑byte pages, which is also valid and ships in real VxDs; use `/ALIGN:0x1000` to match VKD.

**The `.def`** declares the module a VxD, lists every segment `PRELOAD NONDISCARDABLE` (for the identity
page map, §4.4 of the companion doc), and exports the DDB as ordinal 1:

```
VXD <NAME>
DESCRIPTION '<text>'

SEGMENTS
    _LTEXT   CLASS 'LCODE'  PRELOAD NONDISCARDABLE
    _LDATA   CLASS 'LCODE'  PRELOAD NONDISCARDABLE
    <CUST>   CLASS '<CUST>CODE' PRELOAD NONDISCARDABLE
    …

EXPORTS
    <NAME>_DDB @1
```

> **Critical: run the linker from a directory whose path has no spaces.** LINK 2.60's `/VXD` mode
> re‑executes the linker to build the exports file and does not quote its own module path. The DDK's
> install path contains spaces (`…\Microsoft Windows 95 DDK\…`), so the self‑spawn splits on them and
> fails (`cannot open input file "Windows.obj"`, LNK1181 → LNK1141). Stage `LINK.EXE`, `DBI.DLL`,
> `LINK.ERR`, and `IMAGEHLP.DLL` into a space‑free directory and run from there, with that directory on
> `PATH` so the self‑spawn resolves.

### 2.3 Resource compile + embed (DOSBox)

Per the DDK's `BASE\VXDRES\MAKEFILE`:

```
set INCLUDE=<inc16 dir>
rc -r <name>.rc                    # -> <name>.res  (16-bit ANSI resource)
adrc2vxd <name>.vxd <name>.res     # embeds the resource; sets LE 0xb8/0xbc
```

`RC.EXE` and `RCPP.EXE` must be in the same directory. The DDK `INC16` headers must be reachable via
`INCLUDE` (they contain transitive includes: `version.h` → `ver.h`). Both the DDK path (spaces) and
DOSBox's mount limitations mean the tools and headers should be staged into space‑free directories.

The `.rc` is reconstructed from recovered metadata (see `version_rc` in `vxd_from_source.py`):

```rc
#include <version.h>
#define VER_FILETYPE             VFT_VXD
#define VER_FILESUBTYPE          0x0000000D          /* device id, from the DDB */
#define VER_FILEDESCRIPTION_STR  "VKD Virtual Device     (Version 4.0)"
#define VER_INTERNALNAME_STR     "VKD"               /* from the DDB */
#define VER_LEGALCOPYRIGHT_YEARS "1988-1995"
#define VER_ORIGINALFILENAME_STR "VKD.VXD"
#include <common.ver>
```

---

## 3. Environment notes

* **DOSBox‑X** hosts the DOS tools (`ML`, `RC`/`RCPP`, `ADRC2VXD`). The engine's `run_dos` helper mounts a
  tool drive and a work drive, writes a `.BAT`, runs it, and reads back artifacts. It **deletes named
  artifacts before running**, so a file that is both input and output must be staged under a different
  name and copied to the output name inside the batch. DOSBox **cannot mount a path containing spaces**,
  so DDK tools and headers are copied to space‑free staging directories first. DOS has no `2>&1`; redirect
  with `> file` only.
* **LINK 2.60** runs natively (it is a Win32 PE). It is the only stage that does not use DOSBox.
* **Determinism.** Given the same source and `.def`, the toolchain is deterministic: the same `.obj`, the
  same linked container, the same compiled resource, the same final bytes.

---

## 4. Provenance and licensing

These are Microsoft tools from the Windows 95 DDK and period compiler packages. They are **run**, not
redistributed; none of these binaries are committed to the repository. The engine references them at the
paths above on the local machine. The reconstructed source and the engine that produces it are the
committed artifacts; the toolchain is an external dependency documented here so the build is reproducible
by anyone with the same period DDK.
