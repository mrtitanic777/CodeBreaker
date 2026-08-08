#!/usr/bin/env python3
r"""calleesaved.py — recover callee-saved GP registers (ebx/esi/edi) from the SHIPPED
bytes so the raiser stops mistaking them for input registers.

Why this exists
---------------
The lift's register-machine C keeps ebx/esi/edi as plain register temporaries. When a
function uses one of them as an ordinary local (`push esi` in the prologue, `... esi as a
scratch/loop var ...`, `pop esi` in the epilogue), the lifted body *reads esi before it is
written* — because the lift drops the prologue `push`/epilogue `pop` (pure stack bookkeeping
that produces no C). `raiser.register_recover` sees the used-before-def and BAILS, calling
esi an "input register". That is correct for __fastcall inputs (ecx/edx, handled by
`fastcall_prep`) but WRONG for callee-saved registers: those are locals, not inputs.

The shipped bytes settle it. The x86 SysV/stdcall ABI makes ebx/esi/edi/ebp callee-saved: a
function that uses one must `push` it in the prologue and `pop` it in the epilogue. So:

    a register used-before-def in the lifted body is a callee-saved LOCAL
    (declare `unsigned esi;`) — NOT an input — exactly when the shipped bytes
    push it in the prologue and pop it in the epilogue.

`calleesaved(mod, seg, off, body)` returns that set. `is_input_register(body, saved)` is the
input test with the callee-saved set excluded — the drop-in replacement for the bail check in
`register_recover` and the input-scan in `fastcall_prep`.

Push/pop opcodes (single-byte, 32-bit):
    push ebx=0x53  push esi=0x56  push edi=0x57   (push ebp=0x55, push ecx..=0x50-0x52)
    pop  ebx=0x5b  pop  esi=0x5e  pop  edi=0x5f   (pop  ebp=0x5d)

Limits (see module bottom / the report): ebp is deliberately NOT returned — as a frame
pointer (`push ebp; mov ebp,esp`) it is not a local, and telling a frame ebp from a
local ebp needs more than the push/pop test. A register that is genuinely a __fastcall
input (ecx/edx) is never pushed callee-saved, so it never appears here.
"""
import re
from engine.foundation import cores

# opcode -> register, for the single-byte push/pop of the callee-saved GP registers.
_PUSH = {0x53: "ebx", 0x56: "esi", 0x57: "edi"}
_POP = {0x5b: "ebx", 0x5e: "esi", 0x5f: "edi"}
CALLEE_SAVED = ("ebx", "esi", "edi")           # ebp excluded on purpose (frame vs local)

# bytes that may legitimately sit BETWEEN/BEFORE the callee-save pushes in a prologue and
# must be stepped over without ending the prologue scan:
#   55            push ebp
#   8b ec / 89 e5 mov ebp, esp
#   83 ec ib      sub esp, imm8         (local frame)
#   81 ec id      sub esp, imm32
#   6a ib         push imm8             (rare: arg pre-push interleaved — still prologue-ish)


def _regs_pushed_in_prologue(sb):
    """Walk the leading bytes and collect the callee-saved regs pushed before the first
    'real' (non-prologue) instruction. Pure single-byte push scan with a small allow-list
    for frame setup so `push ebp; mov ebp,esp; push esi` still sees esi. Conservative: the
    moment an unrecognized opcode appears the prologue is over."""
    pushed = []
    i, n = 0, len(sb)
    while i < n:
        op = sb[i]
        if op in _PUSH:
            pushed.append(_PUSH[op]); i += 1; continue
        if op == 0x55:                                  # push ebp (frame) — step over
            i += 1; continue
        if op == 0x8b and i + 1 < n and sb[i + 1] == 0xec:   # mov ebp, esp
            i += 2; continue
        if op == 0x89 and i + 1 < n and sb[i + 1] == 0xe5:   # mov ebp, esp (alt encoding)
            i += 2; continue
        if op == 0x83 and i + 1 < n and sb[i + 1] == 0xec:   # sub esp, imm8
            i += 3; continue
        if op == 0x81 and i + 1 < n and sb[i + 1] == 0xec:   # sub esp, imm32
            i += 6; continue
        break                                           # first non-prologue insn: stop
    return pushed


def _regs_popped(sb):
    """Set of callee-saved regs restored anywhere in the function. Scanning the whole
    stream (not just the tail) tolerates multiple epilogues / early returns. Single-byte
    pop opcodes can in principle collide with an operand/displacement byte, so this is a
    *confirmation* of the prologue push, never the sole signal."""
    return {_POP[b] for b in sb if b in _POP}


def calleesaved_from_bytes(sb):
    """Core: (set_of_callee_saved_regs, detail_dict) from shipped bytes `sb`.
    A reg is callee-saved iff it is pushed in the prologue AND popped in the function."""
    pushed = _regs_pushed_in_prologue(sb)
    popped = _regs_popped(sb)
    saved = {r for r in pushed if r in popped}
    return saved, {"pushed": pushed, "popped": sorted(popped),
                   "saved": sorted(saved)}


def calleesaved(mod, seg, off, body=None, matchbatch=None):
    """Set of callee-saved GP registers (ebx/esi/edi) for the function at mod/seg/off,
    read from its SHIPPED bytes. These are LOCALS, not inputs.

    `matchbatch` is the engine module (passed in to avoid an import cycle / sys.path
    juggling); if omitted it is imported. `body` is unused for the byte-based decision but
    accepted so callers can pass it for symmetry / future body-cross-checks. Returns an
    empty set when the asm/bytes can't be found (fail safe: no regs reclassified)."""
    if matchbatch is None:
        _mb = cores.get('matchbatch')
        matchbatch = _mb
    ap = matchbatch._asm_for(mod, seg, off)
    if not ap:
        return set()
    sb = matchbatch.shipped_bytes(ap)
    if not sb:
        return set()
    saved, _ = calleesaved_from_bytes(sb)
    return saved


# --- the input-register test, callee-saved-aware -----------------------------------------
# raiser currently bails when ANY GP register is used-before-def. This is that same scan,
# but a register counts as a true INPUT only if used-before-def AND NOT in `saved`.

REG = r"(?:e[abcd]x|e[sd]i|ebp)"


def used_before_def(body, regs=None):
    """{reg} for registers whose first textual mention in `body` is a USE, not `reg = ...`.
    (Mirrors the scan in raiser.register_recover / fastcall_prep.)"""
    out = set()
    for r in (regs if regs is not None else set(re.findall(REG, body))):
        first_def = re.search(r"\b%s\s*=" % r, body)
        first_use = re.search(r"\b%s\b" % r, body)
        if first_use and (not first_def or first_use.start() < first_def.start()):
            if not (first_def and first_def.start() == first_use.start()):
                out.add(r)
    return out


def input_registers(body, saved):
    """The TRUE input registers: used-before-def MINUS the callee-saved set. These are the
    ones the raiser must still treat as incoming (ecx/edx -> __fastcall; esi/edi/ebx here
    would mean 'saved but genuinely read as input' — rare, left as a real limit)."""
    return used_before_def(body) - set(saved)


if __name__ == "__main__":
    # tiny self-check on synthetic byte streams
    cases = {
        "esi local":      (bytes([0x56, 0x8b, 0x74, 0x24, 0x08, 0x5e, 0xc3]), {"esi"}),
        "ebx esi edi":    (bytes([0x53, 0x56, 0x57, 0x90, 0x5f, 0x5e, 0x5b, 0xc3]),
                           {"ebx", "esi", "edi"}),
        "frame + esi":    (bytes([0x55, 0x8b, 0xec, 0x56, 0x90, 0x5e, 0x5d, 0xc3]), {"esi"}),
        "no saves":       (bytes([0x8b, 0x44, 0x24, 0x04, 0xc3]), set()),
        "push no pop":    (bytes([0x53, 0x90, 0xc3]), set()),
    }
    ok = True
    for name, (sb, want) in cases.items():
        got, det = calleesaved_from_bytes(sb)
        flag = "ok" if got == want else "FAIL"
        ok = ok and got == want
        print("  %-14s -> %-18s want %-18s %s  %s" % (name, sorted(got), sorted(want), flag, det))
    print("SELFTEST:", "PASS" if ok else "FAIL")
