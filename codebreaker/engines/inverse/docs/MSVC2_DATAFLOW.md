# MSVC 2.0 register-dataflow model — scope & plan

*Scoping doc for the next tier of the 32-bit `msvc2` C-inversion (`msvc2model.py` /
`msvc2inv.py`). Written 2026-09-07. The single-return-expression, frameless,
`__fastcall`, and multi-statement-sequence + `if`-guard layers are byte-exact and
shipped (self-test 69/69; real direct coverage 50/1304 = 3.8%). This doc scopes
the piece that unblocks the bulk of what remains.*

---

## 1. Why — the wall the template layers hit

Every clean template we add now recovers ~0 new real functions **in isolation**,
because real functions interleave register state. The declined-branch census over
KERNEL32/GDI32/USER32 is decisive: the branch condition is

| condition before the first `jcc` | count |
|----------------------------------|------:|
| `test eax,eax` | 181 |
| `cmp <mem/reg>` | 52 |
| `or eax,eax`   | 28 |
| `cmp <mem>, imm` | 25 |

`test eax,eax` / `or eax,eax` **test the value in `eax`** — which is almost always
**the result of the immediately preceding `call`**. Our `if`-guard only models
`cmp [arg],0`. To recover these we must know *what value a register holds*.

Ground truth (MSVC 2.0 `/O1 /Oy-`, probed):

```
if(q(a)) g(a);          push [a]; call q; test eax,eax; je skip; push [a]; call g; skip:
int t=q(a); return t+1; push [a]; call q; inc eax                         # t == eax (call result)
int t=q(a); g(b); ret t push esi; push [a]; call q; <esi=eax>; push [b]; call g; <eax=esi>; ...
return q(a)+q(b);       push esi; push [b]; call q; push [a]; <esi=eax>; call q; lea/add eax,esi
```

Three mechanisms appear: **(a) `eax` holds the last call's result**; **(b) a value
that must survive a later call is parked in a callee-saved register (`esi`/`edi`/
`ebx`)**; **(c) branches/returns consume those register values**.

---

## 2. Precedent — port the proven 16-bit tracker

`codebreaker/engines/inverse/msc6reg.py` already does this for MSC 6.0a (16-bit): *"a
small abstract interpreter tracks each register's symbolic C-expression through the
instruction stream, recovers the enregistered locals (SI/DI) and the value-
numbering idiom, and emits C."* It is itself a port of `mc2inv.py`'s `class R` (the
MC-2.03 register tracker). The 16-bit `STATUS.md` records the same mechanisms we
need — *register value-tracking (backward alias + forward store-hoist), copy-
reversal, instruction-scheduling reversal, dead-store recovery* — proven byte-exact
(19/19 locals, 10/10 real functions). **We port the concept, retargeted to 32-bit
MSVC 2.0 / `eax`,`ecx`,`edx`,`esi`,`edi`,`ebx` and the `__stdcall` call ABI.**

---

## 3. The abstraction

A **value** is a symbolic expression the model already understands, extended with a
call node:

```
Value = ("arg",k) | ("const",v) | ("global",g) | ("deref",Value) | ("binop",op,V,V)
      | ("call", conv, cargs) | ("icall", cargs)          # NEW: a call's result is a value
```

A **register environment** `env : reg -> Value | None` (⊤ = unknown). The core is an
**abstract interpreter** `step(env, insn) -> env'` that both directions share:

* `call rel32 / call [imm]`  →  `env = {eax: ("call",…)}` (all caller-saved regs
  clobbered; eax gets the result; callee-saved esi/edi/ebx survive).
* `mov r, [ebp+argk]`         →  `env[r] = ("arg", k)`
* `mov r, imm` / `xor r,r`    →  `env[r] = ("const", …)`
* `mov dst, src`  (reg→reg)   →  `env[dst] = env[src]`      (value-numbering / copy)
* `op eax, r/m`               →  `env[eax] = ("binop", op, env[eax], operand-value)`
* `inc/dec eax`               →  `env[eax] = ("binop", ±, env[eax], 1)`
* `push r`                    →  a pending call arg = `env[r]` (register-passed arg)
* `test r,r` / `or r,r` ; `jcc` → branch cond = `env[r] != 0` (or `== 0`)

**Inverse** = run `step` forward over the instruction stream; when a `jcc`,
`ret`(eax), or `push`(arg) consumes a register, resolve it to `env[reg]`. **Forward
model** = the same env is the scheduling oracle: to emit `if(q(a))…` we emit the
call (eax←result), then `test eax,eax; jcc`, because the value is already in eax.

This unifies the currently-separate recognizers: `_parse_ret_body`, the callbin op-
on-eax, and the seq return-call are all special cases of "the return/branch value is
`env[eax]` after the statements".

---

## 4. Milestones (each self-verified byte-exact, measured on the real corpus)

**M1 — `eax` = last-call result (branch + return).**  Track only `eax`. Recognize
`call; test eax,eax; jcc` → `if(<call>) …`, and `call; <op on eax>; ret` →
`return <call> <op> …`. This is the 181-`test eax,eax` bucket + the `t=q(a);return
t±1` returns. Forward: emit the call then the test/op. *Highest yield; smallest
step.* Touches `_parse_stmts` (branch cond can be a call-result test) and
`_parse_ret_body` (return value can be `env[eax]`).

**M2 — callee-saved preservation (`esi`/`edi`/`ebx`).**  Model `push esi; …; mov
esi,eax; …; <use esi>; pop esi`: a value needed across a later call is parked in a
callee-saved reg. Recognize the save/park/restore bracket and treat `esi` as a
named temp (a `Value`). Covers `int t=q(a); g(b); return t;` and `q(a)+q(b)`
(the `esi` two-call bucket). Forward model must choose the same reg + insert the
save/restore — the register-allocation decision (park iff the value is live across a
clobbering call).

**M3 — general enregistered locals + reuse.**  A value loaded once and reused from a
register (arg or computed) instead of re-read; the value-numbering idiom `push r;
mov s,r` (pass AND keep). Directly mirrors `msc6reg.py`'s SI/DI recovery.

**M4 — loops + scheduling.**  `while`/`for` with a register counter (msc6reg has the
16-bit template), and the instruction-scheduling reversal (load-hoist/store-defer)
the 16-bit `STATUS.md` documents. Largest; last.

---

## 5. Shape of the code

* New module `msvc2reg.py` — the abstract interpreter (`class Env` + `step`), the
  32-bit sibling of `msc6reg.py`. Pure, shared by model and inverse.
* `msvc2model.emit`: a `("call",…)`/`("icall",…)` **as a Value inside an expr or
  cond** (not only as a whole statement) — emit the call, then consume `eax`.
* `msvc2inv`: replace the ad-hoc `_parse_ret_body` / branch-cond parsing with an
  `Env`-driven resolve; a `jcc` after `test eax,eax` reads `env[eax]`.
* Everything stays **emit-verified** (byte-exact or declined) — the interpreter only
  proposes; `emit(node) == bytes` is still the sole acceptance test, so a wrong
  dataflow guess can never produce wrong C.

---

## 6. Hard parts / risks

* **Register-allocation *decisions* in the forward model** (M2+): *which* value gets
  parked in *which* callee-saved reg, and *when*, is an optimizer heuristic. The
  16-bit track cracked its analogue ("park iff live across a clobber"); expect
  several probe→rule→verify iterations per decision. This is the genuine cost.
* **Branchless selects** (`neg;sbb;inc` = `x?1:2`) are a separate idiom family, not
  dataflow per se — recognize as fixed templates when they appear post-call.
* **Sampling**: control-flow / multi-return functions exceed the `_leaf_bytes`
  "first ret" cutoff — the coverage harness needs full-function extraction (carve
  `[start,end)`, trim trailing padding) before M1's yield is visible.

**Order of attack:** fix the sampler → M1 (`eax` tracking) → measure → M2 (callee-
saved) → measure. M1 alone should convert the 181-`test eax,eax` bucket from "0" to
a real number, the first evidence the dataflow model pays off.
