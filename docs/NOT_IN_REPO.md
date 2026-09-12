# What is *not* in the repo

CodeBreaker tracks **tool code only.** Input corpora, period Microsoft toolchains, VM images,
build artifacts, machine-specific config, third-party binaries, and any secrets are **never
committed** — they are attached locally and stay out of version control. This is deliberate:
the repo must ship clean as a *standard of measure*, carrying no proprietary or generated
payload. This document is the authoritative inventory of everything that lives on disk but is
not pushed, why, and how to re-attach it on a fresh clone.

## The ignore layers

Three `.gitignore` files enforce this, innermost-wins:

| File | Scope |
|---|---|
| `.gitignore` | repo root — toolchains, data caches, build output, local config, user inputs |
| `codebreaker/xbox/.gitignore` | the Xbox 360 subsystem — samples, console secrets, vendored/third-party tools |
| `tools/nasm-masm/.gitignore` | the vendored assembler — built `nasm`/`*.exe`/`*.o` output |

A fresh clone plus `pip install -r requirements.txt` plus a filled-in `config.json` is a
complete, working tool. Everything below is additive.

## 1. Period toolchains — `toolchains/` (≈ 1.6 GB, git-ignored in full)

The period Microsoft build tools live at the repo root under `toolchains/`. License and
provenance keep them out of the repo — install/attach your own and point `config.json` at them.

| Path | What it is | Config key |
|---|---|---|
| `toolchains/period/` | DOS-era MS toolchains: MS C / MASM / LINK, `msvc-1.52c` (RC 3.11, DOSXNT), `msvc-2.0` (LINK 2.50) | `msc6_toolchain` |
| `toolchains/dosbox-x/` | DOSBox-X, to run the DOS-era tools | `dosbox_x` |
| `toolchains/vc71sp1/` | the MSVC 7.1 SP1 compiler used by the recompile/backbone build | `msvc_cl`, `msvc_env_path` |

## 2. Win95 data — lives in the external `Windows95` project repo

None of the Win95 corpus/media/VMs live under CodeBreaker. They sit in the sibling
`GitHub/Windows95/` repo (their natural home beside the Win95 design docs), and the tools
resolve them through `WT_DATA_ROOT` / the `paths` resolver — no junction, no in-tool copy.

| What | Where | How tools resolve it |
|---|---|---|
| Win95 input corpus (`files/WINDOWS/SYSTEM/*`, `disc/src`, `reports/`) | `Windows95/design/corpus/` | `WT_DATA_ROOT` → `paths.data_root()`; `config.json:msc6_data` |
| Golden oracle disc (`Win95 OSR2.ISO`) | `Windows95/design/oracle/` | `paths.design_root()` → `oracle._golden_iso()` |
| Byte-exact ASM keepsake (`win95re.iso`) | `Windows95/design/legacy/` | — (kept for reference) |
| Period reference SDK/DDK (`WIN32SDK`, `Microsoft Windows 95 DDK`) | `Windows95/design/reference/` | `paths.reference(...)` |
| VMware VM images (boot VMDKs, experiment snapshots) | `Windows95/vm/` | supply your own; boot in VMware |

To attach: set `WT_DATA_ROOT=<path>\Windows95\design\corpus` (see `env.sh`) and, if you use the
MS C 6 path, `config.json:msc6_data` to the same. `paths.design_root()` derives the reference /
oracle / legacy siblings from the corpus parent automatically (override with `WT_REFERENCE_ROOT`).

## 3. Local configuration (git-ignored)

| Path | What it is | Re-attach |
|---|---|---|
| `config.json` | machine-specific external-tool paths | `cp config.json.example config.json`, fill in |

`config.json.example` **is** tracked and documents every key: `dosbox_x`, `msc6_toolchain`,
`msc6_data`, `msc203_oracle`, `nasm`, `masm`, `msvc152c`, `sevenzip`, `ghidra`, `msvc_cl`,
`msvc_include`, `msvc_lib`, `msvc_env_path`. Paths may also be supplied through the environment
(`env.sh`: `WT_DATA_ROOT`, `WT_REFERENCE_ROOT`, `WT_<ERA>_DATA`).

> The MS C 2.03 inverse oracle (`msc203_oracle`, the MC1/MC2 gate for `wt inverse` byte-exact
> verify) is **not attached** in this checkout — that subsystem reports "oracle not configured"
> and skips byte-exact verify until you point `config.json:msc203_oracle` at an MC1.EXE/MC2.EXE
> pair. Everything else runs without it.

## 4. Third-party / generated tool binaries (never distributed)

| Path | Size | What it is |
|---|---:|---|
| `toolchains/…` (DOSBox-X, DOS-era MS C/MASM/LINK, vc71sp1) | ~1.6 GB (§1) | period build toolchain — attach your own |
| `binaries/ghidra_gdt/*.gdt` | ~1.3 MB | **generated** Ghidra type archives (Win32/Win16/VxD), built by `codebreaker/toolchain/ghidra/BuildGDT.py` from the reference SDK/DDK headers; regenerable |
| `codebreaker/xbox/xbt/crypto/bin/*.exe` | ~2.6 MB | PowerPC toolchain (`xenon-as.exe`, `xenon-objcopy.exe`) for the Xbox path |
| `codebreaker/xbox/xbt/vendor/` | ~31 MB | third-party libraries vendored on disk for the Xbox subsystem |
| `tools/nasm-masm/nasm.exe` (+ `*.o`) | ~11 MB | the **built** nasm-masm assembler — *source is tracked, binary is not* |

> `tools/nasm-masm/` ships as **source**. Build it (`sh autogen.sh && ./configure && make`, or
> `nmake -f Mkfiles/msvc.mak`) and point `config.json:nasm` at the resulting executable to
> enable the native assembler backbone.

## 5. Console secrets & copyrighted samples (excluded by policy)

**Never committed under any circumstance.** Secrets are excluded on principle; samples are
excluded on both regenerability and copyright grounds.

| Path | Size | What it is |
|---|---:|---|
| `codebreaker/xbox/NexGen_Review/` | ~21 MB | Xbox console research material incl. device secrets — **kept strictly local** |
| `codebreaker/xbox/samples/` | — | copyrighted game assets / regenerable sample data (`samples/README.md` is tracked) |
| `codebreaker/xbox/files/` | — | user-dropped title binaries (copyrighted) |

## 6. Build artifacts, caches & generated data (regenerable)

| Pattern | What it is |
|---|---|
| `__pycache__/`, `*.pyc` | Python bytecode caches |
| `build/`, `dist/`, `*.manifest` | PyInstaller packaging output |
| `reports/` | generated coverage ledgers, conduct/*.json, logs |
| `*.o`, `*.obj` | object files (recompile / assembler build output) |
| `codebreaker/engines/diff/*.db`, `*.db-wal`, `*.db-shm` | the Transpiler-Diff corpus database (rebuild via `wt diff ingest`) |
| `reference/`, `files/`, `*.iso` | user-supplied reference trees / inputs / disc images |

## Re-attaching on a fresh clone — checklist

1. `pip install -r requirements.txt`
2. `cp config.json.example config.json` and fill in the paths you use.
3. Attach `toolchains/` (period MS tools + DOSBox-X + vc71sp1) and point `config.json` at them.
4. Set `WT_DATA_ROOT` to your `Windows95/design/corpus` (and `config.json:msc6_data` to match).
5. (Optional) build `tools/nasm-masm` and set `config.json:nasm` for the native assembler.
6. `python wt.py doctor` — confirms what is and isn't configured.
