# Microsoft C 6.0a RE — Full Project Map (end to end)

The complete route from shipped floppies to faithful, verified recovered source for the
Microsoft C 6.0a compiler. Companion: [`notes.md`](notes.md), [`signatures.md`](signatures.md).

**Guiding rule (CodeBreaker's):** faithfulness over coverage. Triage by origin first, recover from
evidence (the Unicorn oracle) not guesswork, tier every result by confidence, and prove correctness
by byte-exact re-assembly wherever the code class allows.

**Primary targets, in recommended order:** `CL.EXE` (driver, warm-up) → `C1.EXE` (front end) →
`C3.EXE` (optimizer) → `C2.EXE` (code generator, hardest) → `C1L.EXE` (large-model diff of C1).
Secondary: `LINK`, `LIB`, `MASM`, `NMAKE`, `CV`, `RC`, PWB.

---

## Phase 0 — Environment & corpus (DONE / in progress)
**Goal:** everything needed to work, reproducibly.
- [x] Verify CodeBreaker (`wt doctor`), Python, DOSBox-X, 7-Zip, period MSC6 toolchain.
- [x] Inventory all 7 disks; identify primary vs. secondary targets (`notes.md` §2).
- [x] Confirm binary format: real-mode MZ, **EXEPACK-packed** (`signatures.md` §1–2).
- [x] Workspace (`work/{unpacked,extracted,disasm,oracle}`), git init, `.gitignore`.
- [ ] **Extract secondary targets** from disk images: decompress `.EX$`/`.LI$`/`.OB$` (SZDD) →
      `work/extracted/`. Tools: MS `EXPAND`/`msexpand`, or DOSBox `EXPAND`, or a Python SZDD decoder.
- [ ] Snapshot SHA256 of every input into `signatures.md`.

**Exit:** all binaries on disk, formats known, tooling green.

## Phase 1 — Unpack (EXEPACK)  ← DONE (the gate for everything downstream)
**Goal:** a faithful *unpacked* MZ image of each pass (you cannot disassemble the packed image).
- [x] Wrote **`tools/unexepack.py`** (our own EXEPACK reverser — in-place backward RLE + reloc
      rebuild; format notes in `signatures.md` §1a). No UNP/DOSBox needed.
- [x] Unpacked all 5 → `work/unpacked/{CL,C1,C1L,C2,C3}.exe`.
- [x] Verified each: image at real `CS:IP` disassembles to a genuine MSC startup; realistic rebuilt
      reloc counts (4 / 845 / 2575 / 2400 / 1501). Headers + SHA-256 → `signatures.md` §3.
- [x] Bonus: harvested SCCS `@(#)` what-strings → `work/module_manifest.txt` (internal module map).
- [ ] **`C2.EXE` overlay:** the ~158 KB tail past the MZ image is still unhandled (unexepack does the
      MZ image only). Characterize (overlay manager? packed data?) before analyzing that region.
- [ ] Optional belt-and-suspenders: run an unpacked image in DOSBox to confirm it executes/prints usage.

**Exit:** ✅ `work/unpacked/*.exe` produced & verified; headers logged. (C2 overlay carried to Phase 5.)

## Phase 2 — Load, segment map & disassembly  ← DONE (inventory + floor)
**Goal:** clean, reassembleable 16-bit disassembly with a correct segment/relocation model.
- [x] Wrote **`tools/carve.py`** (Capstone 16-bit): recursive-descent + **linear-sweep** leader
      seeding (direct near `E8`/far `9A` call targets + `55 8B EC` prologues) → function inventory,
      call graph, indirect-branch sites. Handles the segmented (medium/large-model) passes.
- [x] Ran on all 5 → `work/<pass>/functions.json` + `floor.asm`. Summary: `work/phase2_summary.md`.
      **~4,150 functions total**; code = 81–91% of each image; **byte-decode 96–99%** (`db` =
      relocation-patched words + data-in-code — faithful by construction).
- [x] Byte-exact **ASM floor** emitted straight from the real bytes (every byte accounted for).
- [ ] **Round-trip verification** of the floor (re-assemble → compare) not yet wired — needs MASM
      (have MASM 5.1/6.13 in `toolchains/period`) or native `nasm-masm` config → carry to Phase 6.
- [ ] Refine the segment map (group functions by code segment); prune sweep false-positives.
- [ ] Extract strings/message tables; link `C1.ERR`/`C23.ERR`/`CL.MSG` to referencing code.

**Exit:** ✅ function inventory + call graph + byte-exact ASM floor for all 5 passes. (Round-trip
verify + segment grouping refined later.)

## Phase 3 — Origin triage (identify, don't reconstruct)  ← first pass DONE
**Goal:** separate authored compiler code from toolchain/library code before recovering anything.
- [x] Library ground truth in place: period `.LIB`/`.OBJ` already decompressed under
      `toolchains/period/compilers/msc-6.0a/LIB` (CLIBCR/SLIBCR/MLIBCR/LLIBCR, math, LIBH, …).
      `wt inverse omf` reads them cleanly (PUBDEFs: `__astart`, `__cintDIV`, `_spawnve`, …).
- [x] Ran **`wt inverse libmatch`** (FLIRT-style: mask FIXUPP sites, slide-search lib funcs in the
      image) on all 5 passes → `work/triage/` + `work/triage_summary.md`. CRT identified:
      CL 41 · C1 45 · C1L 43 · C2 22 · C3 24. Remaining = authored (the reconstruction targets):
      168 / 971 / 939 / 969 / 927. Low CRT fraction is expected — the passes are ~all compiler code.
      **Fixed a libmatch bug** (it only accepted Lattice's `PROG` code segment; MSC uses `_TEXT` /
      `<mod>_TEXT`) — one-line, backward-compatible; see notes log. Worth upstreaming to CodeBreaker.
- [ ] Improve recall: match extra libs (LIBH, `*FA`/`*FP` math, EM, 87), align sweep leaders to exact
      CRT entries, and fold in `@(#)` module boundaries for authored-side naming.
- [ ] Build **MSC6 compiler-idiom signatures** (prologue/epilogue, `__chkstk`, thunks, switch tables)
      seeded from `src/engines/msc6/IDIOMS.md` (`signatures.md` §6).
- [ ] Confirm self-hosting hypothesis (built with MSC6/MASM 5.1) → unlocks the `msc6` inverse suite.

**Exit:** ✅ first-pass origin split done (CRT vs authored, per pass). Recall-improvement + idiom
signatures carried forward; authored worklist = the "remain" columns.

## Phase 4 — Behavioral & inverse reconstruction of authored code  ← baseline run; gap pinned
**Goal:** turn authored functions into readable, faithful C, evidence-backed and tiered.
- [x] Baseline: ran `msc6dec.invert()` on every authored function (`tools/recover.py`). **100% decline
      at "prologue miss"** — the MSC6 inverse models only the Win95 DLLs' **far-loadds** prologue;
      our passes use the **near-frame `55 8B EC`** prologue (measured — `signatures.md` §6).
- [ ] **Unblock:** add a near-frame prologue/epilogue template to the MSC6 inverse (`msc6inv`/`msc6dec`)
      — the single gating change. Then re-run the baseline; expect leaf/simple functions to invert
      byte-exact. Extend templates one class at a time (probe → model → invert → verify), MSC6-style.
- [ ] Bottom-up over the call graph (callees before callers); tier every function.
- [ ] Leaf/pure → `wt behavior synth` on the Unicorn oracle (independent evidence path; check it
      supports real-mode 16-bit first).
- [ ] Structs/globals/loops → `behavior struct|globals|loops`; CFG → `behavior cfg|structure`.
- [ ] Orchestrators → `behavior interproc` (named call sequences, arg mapping).
- [ ] Type propagation, constant/flag naming, stack-string + message recovery; floor the rest at ASM.

**Exit:** per-pass recovered C at mixed tiers, nothing faked, ASM floor everywhere else. *(Blocked on
the near-frame template; baseline + gap analysis done.)*

## Phase 5 — Architecture recovery (make it a *compiler*, not a pile of functions)
**Goal:** recover the design, not just bodies. This is where it becomes readable as MSC6 internals.
- [ ] **CL.EXE:** command-line grammar, option table, pass-chaining / temp-file protocol, env handling.
- [ ] **C1:** preprocessor, lexer, parser, symbol table, type system, the C1→C2 **intermediate file**
      format (the on-disk IL handed to the code generator) — a headline artifact.
- [ ] **C2:** IL reader, code selection, register allocation, the OMF `.OBJ` **emitter**, overlay use.
- [ ] **C3:** which optimizations, and where it sits in the chain.
- [ ] Diff **C1 vs C1L** to isolate memory-model-dependent code from model-independent logic.
- [ ] Document data structures (symbol table, type nodes, IL records) in `docs/`.

**Exit:** an "MSC6 internals" document set: pass responsibilities, the IL/intermediate format, data structures.

## Phase 6 — Verification (prove it)
**Goal:** earn every claim. Byte-exact where possible; oracle-equivalent otherwise.
- [ ] Re-assemble recovered ASM → compare **byte-for-byte** to the unpacked image (the floor must be exact).
- [ ] Re-compile recovered C (period MSC6) → differential-test against the binary on the oracle.
- [ ] Behavioral equivalence: recovered `CL`/passes reproduce reference compiles of known inputs.
- [ ] Coverage ledger per pass: % byte-exact / % oracle-verified C / % ASM-floor. Nothing hidden.

**Exit:** signed-off coverage ledger; reproducible verify commands checked in.

## Phase 7 — Secondary toolchain (breadth)
Apply Phases 1–6 to `LINK`, `LIB`, `MASM`, `NMAKE`, `IMPLIB`, `CVPACK`, `EXEHDR`, `RC/RCPP`, `CV`, PWB —
prioritized by interest (LINK + LIB first: they complete the build story and cross-check C2's OMF output).

## Phase 8 — Publish (local) & write-up
- [ ] Consolidate findings into `docs/`: MSC6 architecture, the C1↔C2 IL format, verification results.
- [ ] Keep `notes.md`/`signatures.md` current throughout (they are the source of truth).
- [ ] Optional: a browsable report (CodeBreaker `wt ui` / a generated HTML artifact) of the recovered map.

---

## Milestones (checkpoints)
1. **M1 — Unpacked & verified:** ✅ all 5 passes unpacked, real entries verified as MSC startups,
   headers/hashes logged, internal module map harvested. *(Phase 1 — done 2026-09-14)*
2. **M2 — Mapped:** ✅ inventory + call graph + byte-exact ASM floor for all 5 passes
   (~4,150 funcs, 96–99% decoded). *(Phase 2 — done 2026-09-14; round-trip verify deferred to M-verify)*
3. **M3 — Triaged:** every function origin-tagged; authored worklist locked. *(Phase 3)*
4. **M4 — CL recovered:** the driver fully recovered & verified end-to-end (proof of method). *(Phase 4–6 on CL)*
5. **M5 — Front end (C1) recovered:** incl. the C1→C2 IL format documented. *(Phase 4–6 on C1)*
6. **M6 — Code generator (C2) recovered:** the hard target, incl. overlay + OMF emitter. *(Phase 4–6 on C2)*
7. **M7 — Compiler complete:** C3 + C1L done; full "MSC6 internals" doc set; coverage ledger signed off.
8. **M8 — Toolchain breadth:** LINK/LIB/MASM/etc. as far as interest warrants. *(Phase 7)*

## Risks / watch-items
- **EXEPACK unpack fidelity** (esp. relocation rebuild) — validate static unpack against a DOSBox dump.
- **C2 overlay** — may need an overlay-aware loader before its code is fully reachable.
- **Far/segmented model** — keep segment:offset rigor; don't flatten addresses.
- **Self-host assumption** — if a pass wasn't MSC6-built, the `msc6` inverse won't apply; fall back to `behavior`.
- **Scope** — C2 alone is large; land CL + C1 first to prove the pipeline before committing to C2.
