# msc6 — byte-exact MSC-6.0a reconstruction of 16-bit (NE) Windows DLLs

Merged from the standalone **w95recon** DLL tool. This engine is CodeBreaker's **byte-exact
rung-3 for the NE / MSC-6.0a class** — the rung `recompile95` explicitly leaves out ("GCC will
never reproduce MSVC's exact codegen; the readable lift is deliberately not the byte-exact
source") and that the `inverse_engine` (modeled for Lattice C / MS C 2.03) does not cover.

Where the other engines lift shipped bytes to *readable, compilable* C (`ir_engine`) or invert
a *different* compiler (`inverse_engine`), msc6 reconstructs source that **re-links to the exact
shipped bytes** through the *authentic period toolchain* the DLLs were actually built with:
**MSC 6.0a + MASM 5.1 + LINK 5.20/5.60 + the Win3.1 SDK runtime.** Verified, never eyeballed.

Proven on **LZEXPAND.DLL**: 8/12 code segments byte-identical to Microsoft's shipped bytes
(the rest differ only in the compiler's non-deterministic register tie-breaks), data + runtime
+ DOS stub identical, exact 12-segment NE structure. Source in `reconstructed/lzexpand/`.

## Commands

    wt msc6 list
    wt msc6 detect_recipe <dll>        # auto-pin the DLL's compiler+flags from its own WEP bytes
    wt msc6 detect_recipe --sweep      # ... across the whole NE corpus
    wt msc6 ne_normalize <in> <ref> <out>   # compact an NE's DOS stub to byte-match retail
    wt msc6 skeleton <asm_file>        # STEP-2 skeleton via ir_engine lift (replaces Ghidra)

Toolchain, corpus, and DOS host are external (CodeBreaker ships no binaries) — set in
`config.json`: `msc6_toolchain`, `msc6_data`, `dosbox_x`. `paths.py` resolves them.

## What's here (the unique byte-exact capability)

| Module | Role |
|--------|------|
| `detect_recipe.py` | **Auto-pins each DLL's compiler recipe from its own bytes** — brute-forces `{WEP source forms} × {MSC 6.0a, MSVC 1.52c} × {model} × {prologue} × {/O}` against WEP, using the linker stamp as a compiler prior. DLLs vary (LZEXPAND=/Ox, VER=/Os); declines/@REVIEWs when it can't pin. `recipe_flags(dll)` feeds the search loop. |
| `ne_normalize.py` | Post-link NE stub-compactor — fixes every absolute field so the DOS stub + NE header byte-match retail (LINK pads to 0x400; retail's NE is at 0x80). |
| `bruteforce.py` / `autoaxis.py` | Free compile+diff source-form search (compiler as oracle) to crack the exact C form per function. |
| `toolchain.py` + `toolchain_manifest.json` | Version-keyed resolver: `resolve('linker','5.20')`, `resolve('compiler','msc-6.0a')`. Both LINK 5.20 (LZEXPAND) and 5.60 (majority) validated; 5.10/5.30/5.50 marked gaps, never faked. |
| `SPEC.md` | The end-to-end reconstruction spec (Phase 0–10). |
| `IDIOMS.md` | MSC 6.0a codegen idiom rulebook (grows per function). |
| `reconstructed/` | Proven byte-exact source (LZEXPAND). |

## What was RETIRED in the merge (CodeBreaker already does it, better)

The standalone w95recon reimplemented pieces CodeBreaker owns maturely. Those are **not**
ported — use CodeBreaker's:

| w95recon (retired) | Use instead |
|--------------------|-------------|
| `ne.py` (NE parse) | `src/formats` |
| `disasm.py` (16-bit disasm + discovery) | `src/disasm` |
| `omf.py` (OMF parse) | `src/engines/inverse/omf.py` |
| `verify.py` / segdiff | `src/engines/recon/{verify,segdiff}.py` |
| `skeleton.py` / `pydiscover.py` (Ghidra step-2 skeleton) | `src/ir` lift (16-bit-native, symbol-resolved, no `unaff_SS` garbage) |
| `triage.py` / `scaffold.py` | `src/engines/recompile/tools/triage.py` + the lift/package flow |
| `win95tree.py` (component tree) | **ported** → `src/engines/msc6/tree.py` (`wt msc6 tree`), files into the separate Windows95 source repo (`WT_WIN95_ROOT`) |

## How msc6 fits the byte-exact pipeline

Discovery / disasm / lift come from CodeBreaker's mature engines; **msc6 supplies the byte-exact
back half** the others don't: the *right compiler + flags* (`detect_recipe`), the authentic
toolchain, the source-form search that reproduces the exact bytes, and the NE metadata
normalization.

**Step-2 handoff (done): `skeleton.py` drives `ir_engine`'s lift** as the reconstruction
starting point, replacing the external Ghidra decompiler entirely. The lift is 16-bit-native,
symbol-resolved (named `KERNEL_*` imports), control-flow-recovered, and free of Ghidra's
`unaff_SS`/`CONCAT` segment garbage. It is *register-machine* C (compiles, not yet byte-exact),
so it is a clean draft the recipe-driven search refines toward the exact bytes.

The remaining endgame is to add an **MSC-6.0a model to `inverse_engine`** so byte-exact C comes
from *inverting the codegen* (template recognition + optimizer reversal, the way `inverse_engine`
already does for MS C 2.03) rather than from search — the principled path to the exact bytes.
