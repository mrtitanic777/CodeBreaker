# inverse_engine — compiler-inverse decompilation

A deterministic **bytes → byte-exact C** engine for the **Lattice C 2.12 (DOS, 1984)** code
generator and its exact twin, **Microsoft C 2.03** (`MC1.EXE`/`MC2.EXE` — a rebranded Lattice
C). Instead of treating the compiler as a black box to brute-force against, this reverse-
engineers its **code generator** and **inverts** it: recognize the exact codegen templates in
the machine code, recover an expression tree plus structured control flow, undo the
optimizer, and emit C that recompiles to the *identical* bytes.

    wt inverse list                                   # commands
    wt inverse framemodel                             # frame-allocator self-test (no oracle)
    wt inverse raiser16 <image.bin> <hexoff> <len>    # reconstruct one function
    wt inverse rtlocals                               # local-variable round-trip battery

## How it works

**probe → model → invert → verify.** Ground truth comes from the compiler itself: compile
small C, look at the bytes, find the deterministic rule, add the template to the inverse, then
prove it round-trips byte-exact (compile → invert → recompile → diff). Nothing is guessed —
functions outside the modeled class are cleanly *declined*, never mis-reconstructed.

| Module | Role |
|---|---|
| `mc2model.py`  | **forward model** — `quad → bytes`, a deterministic predictor of the codegen |
| `mc2inv.py`    | **inverse engine** — `bytes → C`: template recognition + optimizer reversal (register value-tracking, copy offset-swaps, instruction scheduling, dead-store recovery) |
| `framemodel.py`| deterministic model of the stack-frame allocator + copy-reversal |
| `raiser16.py`  | reconstruction backend — drives the inverse, then verifies byte-exact via the oracle |
| `oracle16.py`  | the MC1/MC2-under-DOSBox compile oracle (config-driven; see below) |
| `allocprobe.py` / `schedprobe.py` | probes that recover the frame-offset / scheduling rules |
| `rtlocals.py`  | round-trip regression battery |
| `omf.py` / `mzdis.py` / `qdump.py` / `qalign.py` | OMF parser / 16-bit disasm / quad annotator / quad↔obj aligner |
| `libmatch.py` / `vfy.py` / `bvfy.py` / `cmp.py` | library-code id / byte-exact verification helpers |
| `mzasmrecon.py`| MZ → byte-exact `.asm` fallback (via the engine's `masmlift`) |

## The oracle (you supply it)

Verification recompiles with the real compiler. CodeBreaker ships **no** compiler binaries —
you provide **MC1.EXE / MC2.EXE** (Microsoft C 2.03) and **DOSBox-X**, and point `config.json`
at them:

```json
{ "dosbox_x": "<path to dosbox-x.exe>",
  "msc203_oracle": "<dir containing MC1.EXE and MC2.EXE>" }
```

The inverse engine, forward model, and disassembly all run **without** the oracle; only the
final byte-exact *verify* step needs it. `oracle16.available()` reports whether it is wired up.
