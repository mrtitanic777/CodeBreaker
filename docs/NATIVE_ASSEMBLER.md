# Native MASM assembler (optional, recommended)

The byte-exact verifier re-assembles each recovered object and compares it to the
oracle. By default it does this with Microsoft's own **`ML.EXE` (DDK 6.11c) under
DOSBox** — correct, but slow: every object is a fresh DOSBox cold-start.

If a **MASM-compatible build of NASM** is installed, the verifier uses it instead,
natively:

* **~10–20× faster** — no DOSBox, no emulation; each object assembles in
  milliseconds. A full 200+ VxD corpus converges in minutes instead of an hour.
* **At least as faithful.** On the Win95 corpus it reaches byte-exact on every
  module, and it byte-preserves *fewer* instructions than `ML.EXE` does — it
  matches the shipped bytes even on the SIB base/index orderings where ML 6.11c
  diverges, so more instructions stay genuinely symbolic.

Selection is automatic: if a usable assembler is found the native path is used;
otherwise the code falls back to `ML.EXE`/DOSBox with no change in results.

---

## You must install the tool yourself

The native assembler is **not bundled with this repository.** Nothing in the
verifier will use it until you install it and point the verifier at it. Until
then, verification silently falls back to the slower `ML.EXE`/DOSBox path.

It requires a specific build of NASM: the fork that adds a **`--masm`** flag
(MASM 6.x syntax compatibility). A stock upstream `nasm` does **not** understand
`--masm` and will not work — the verifier probes for the flag and ignores any
`nasm` that lacks it.

### 1. Build the `--masm` NASM

Clone the MASM-compatible NASM fork (the one whose `--masm` mode is documented in
its own `README.md`) and build it like upstream NASM:

```sh
sh autogen.sh          # first time only, generates ./configure
sh configure
make                   # produces the `nasm` binary (nasm.exe on Windows)
```

On Windows, build under **MSYS2 / MinGW** (`mingw32-make`), or use the project
files under the fork's `Mkfiles/`.

Confirm the build supports `--masm`:

```sh
nasm --help | grep -- --masm
```

### 2. Point the verifier at it

Either of the following makes the verifier use the native path:

* Set the **`NASM_MASM`** environment variable to the built binary's full path
  (this takes precedence and is the most explicit):

  ```sh
  # Windows (PowerShell)
  $env:NASM_MASM = "C:\path\to\nasm-masm\nasm.exe"

  # Linux / macOS
  export NASM_MASM=/path/to/nasm-masm/nasm
  ```

* **or** put a `--masm`-capable `nasm` (or a copy named `nasm-masm`) on your
  `PATH`.

### 3. Verify it is active

```sh
python -c "from engine.inverse_engine import vxd_masm; print(vxd_masm.find_nasm())"
```

A path means the native assembler is in use. `None` means none was found and the
verifier is using the `ML.EXE`/DOSBox fallback (check that `--masm` is supported
and, if you used `NASM_MASM`, that the path is correct).

---

## Fallback: `ML.EXE` under DOSBox

With no native assembler installed, verification uses Microsoft `ML.EXE`
(DDK 6.11c) through DOSBox. This is the reference path and needs the DDK MASM
toolchain and DOSBox available to the engine; it produces identical byte-exact
verdicts, only slower. No configuration is required to use it — it is what runs
when `find_nasm()` returns `None`.
