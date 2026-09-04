# Native MASM assembler (optional, recommended)

The byte-exact verifier re-assembles each recovered object and compares it to the
oracle. By default it does this with Microsoft's own **`ML.EXE` (DDK 6.11c) under
DOSBox** — correct, but slow: every object is a fresh DOSBox cold-start.

If the bundled **MASM-compatible NASM** is built and configured, the verifier uses
it instead, natively:

* **~10–20× faster** — no DOSBox, no emulation; the whole Win95 VxD corpus
  converges in minutes instead of an hour.
* **At least as faithful.** It reaches byte-exact on every module, and it
  byte-preserves *fewer* instructions than `ML.EXE` — it matches the shipped
  bytes even on the SIB base/index orderings where ML 6.11c diverges, so more
  instructions stay genuinely symbolic.

Selection is automatic: if a usable assembler is found the native path is used;
otherwise the code falls back to `ML.EXE`/DOSBox with no change in results.

---

## You must build the tool yourself

The assembler is a specific fork of NASM — the one that adds a **`--masm`** flag
(MASM 6.x syntax). Its **source is vendored** in [`tools/nasm-masm/`](../tools/nasm-masm),
but the **binary is not committed** (like every other tool here) — you build it
once. Nothing uses the native path until you do; until then verification silently
falls back to the slower `ML.EXE`/DOSBox path.

A stock upstream `nasm` does **not** understand `--masm` and will not be used —
the verifier probes each candidate and skips any that rejects `--masm` **or** that
folds the case of public symbols (the convergence locates drift by `public Q<off>`
symbols, so a build that lower-cases publics is rejected rather than silently
failing every lookup).

### 1. Build the vendored fork

From the repo root:

```sh
cd tools/nasm-masm
sh autogen.sh          # first time only, generates ./configure
sh configure
make                   # produces the `nasm` binary (nasm.exe on Windows)
```

On Windows, build under **MSYS2 / MinGW** (`mingw32-make`), or use the project
files under `tools/nasm-masm/Mkfiles/`. Confirm the build supports `--masm`:

```sh
tools/nasm-masm/nasm --help | grep -- --masm
```

### 2. Point the verifier at it

The default `config.json` already resolves to the bundled build:

```json
{ "nasm": "tools/nasm-masm/nasm.exe" }
```

A **relative** path is resolved against the repo root, so once step 1 produces
`tools/nasm-masm/nasm.exe` nothing else is needed. To use a build elsewhere, set
that key to its full path (the workbench's **Settings** panel writes the same
file). Two overrides are also honored, in this order of precedence:

1. `config.json`'s `nasm`
2. the **`NASM_MASM`** environment variable (full path to the executable)
3. a `--masm`-capable `nasm` (or a copy named `nasm-masm`) on `PATH`

### 3. Verify it is active

```sh
python -c "from engine.inverse_engine import vxd_masm; print(vxd_masm.find_nasm())"
```

A path means the native assembler is in use. `None` means none qualified and the
verifier is on the `ML.EXE`/DOSBox fallback — check that the build supports
`--masm` and preserves public case (rebuild the vendored fork if it predates that
fix), and that the configured path is correct.

---

## Fallback: `ML.EXE` under DOSBox

With no native assembler configured, verification uses Microsoft `ML.EXE`
(DDK 6.11c) through DOSBox. This is the reference path; it needs the DDK MASM
toolchain and DOSBox attached (see the main README's **Get the dependencies**),
produces identical byte-exact verdicts, and is simply slower. No configuration is
required — it is what runs when `find_nasm()` returns `None`.
