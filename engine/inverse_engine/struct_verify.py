#!/usr/bin/env python3
r"""struct_verify.py -- structural (source-fidelity) verification of recovered C.

Byte-exact verification fights the 1990s compiler's register allocator, stack-frame layout and
instruction scheduling -- forcing awkward C to coax one opcode. For SOURCE recovery the goal is
different: the generated C must recompile to the SAME LOGIC as the shipped function -- the same
instruction sequence, the same calls, the same memory-access and branch structure -- while
register choice (si vs di), local stack offsets ([bp-4] vs [bp-6]) and scheduling are free to
differ. That is provably faithful source without being byte-identical.

This is stricter than "it compiles" (which never checks the logic) and looser than byte-exact.
We compile the candidate with the frame-matched oracle, disassemble both, NORMALISE away the
free choices (GP register names, frame offsets, branch/call target addresses), and compare the
instruction streams. A wrong-logic function differs in mnemonic sequence / operand shape and is
rejected; a register-allocation tie-break passes.

    from engine.inverse_engine import struct_verify
    r = struct_verify.structural_match(shipped_bytes, candidate_c)   # {"match": bool, ...}
"""
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from engine.inverse_engine import oracle_router as router
from engine.inverse_engine.msc6fn import _mask

_MD = Cs(CS_ARCH_X86, CS_MODE_16)
_WREG = r"\b(?:ax|bx|cx|dx|si|di)\b"                # ALLOCATION-free word registers (not bp/sp,
_BREG = r"\b(?:al|ah|bl|bh|cl|ch|dl|dh)\b"          # which are the fixed frame regs = structural)
_EWREG = r"\b(?:eax|ebx|ecx|edx|esi|edi)\b"         # 32-bit forms
_BRANCH = {"jmp", "je", "jne", "jl", "jle", "jg", "jge", "jb", "jbe", "ja", "jae",
           "jc", "jnc", "js", "jns", "jo", "jno", "jp", "jnp", "jcxz", "loop", "call", "lcall"}


def _norm_ins(ins):
    """One instruction -> a token that keeps mnemonic + operand SHAPE but drops the compiler's
    free choices: which GP register, the exact frame/global offset, the branch/call target."""
    mn, ops = ins.mnemonic, ins.op_str
    if mn in _BRANCH:                                   # target address is layout-dependent
        return mn + " T"
    ops = re.sub(r"(-?0x[0-9a-f]+|\b\d+)(?=\s*\])", "K", ops)      # displacement inside [ ... ]
    ops = re.sub(r"\[bp \+ \w+\]", "[bp+P]", ops)       # param slot
    ops = re.sub(r"\[bp - \w+\]", "[bp-V]", ops)        # local slot
    ops = re.sub(r"\[0x[0-9a-f]+\]", "[G]", ops)        # global (link-assigned)
    ops = re.sub(_EWREG, "R", ops)
    ops = re.sub(_WREG, "r", ops)
    ops = re.sub(_BREG, "b", ops)
    # a bare trailing immediate operand (mov r, 5) is LOGIC -- keep its value
    return mn + " " + ops.strip()


def normalize(code):
    # mask relocations first (call/segment/far-ptr operands are link-assigned, like byte_exact),
    # then disassemble -- so only genuine LOGIC immediates survive to be compared. nop is padding/
    # scheduling, never logic, so drop it (matches byte_exact stripping trailing 90/00).
    return [_norm_ins(i) for i in _MD.disasm(_mask(bytes(code)), 0) if i.mnemonic != "nop"]


def _lcs(a, b):
    """Longest common subsequence length -- tolerates a stray reordered/extra instruction."""
    if not a or not b:
        return 0
    prev = [0] * (len(b) + 1)
    for x in a:
        cur = [0]
        for j, y in enumerate(b):
            cur.append(prev[j] + 1 if x == y else max(prev[j + 1], cur[-1]))
        prev = cur
    return prev[-1]


def structural_match(fb, candidate_c, thresh=1.0):
    """Compile candidate_c with its frame-matched oracle and compare the NORMALISED instruction
    streams against the shipped bytes. Returns {match, ratio, oracle, recipe, n_ship, n_cand}.
    `match` requires the streams to be structurally EQUAL by default (register/offset/nop
    differences already collapse to equality, so a faithful recovery is exact); the LCS `ratio`
    is reported so near-matches are visible, and thresh<1.0 can admit them, but the strict
    default keeps a stray differing instruction -- a possible logic error -- from passing."""
    fb = bytes(fb)
    ora, recipes = router.pick(fb)
    label = "msvc152" if ora is router._msvc152 else "msc6"
    if not ora.available():
        return {"match": False, "oracle": label, "reason": "oracle not configured"}
    ship = normalize(fb)
    outs = router._compile_batch(ora, [(candidate_c, rcp) for rcp in recipes])
    best = None
    for rcp, rc in zip(recipes, outs):
        if rc is None:
            continue
        cand = normalize(rc)
        if cand == ship:
            return {"match": True, "ratio": 1.0, "oracle": label, "recipe": rcp,
                    "n_ship": len(ship), "n_cand": len(cand)}
        ratio = _lcs(ship, cand) / max(len(ship), len(cand), 1)
        if best is None or ratio > best["ratio"]:
            best = {"match": ratio >= thresh, "ratio": ratio, "oracle": label, "recipe": rcp,
                    "n_ship": len(ship), "n_cand": len(cand)}
    return best or {"match": False, "oracle": label, "reason": "no compile"}


def structural_match_batch(items, thresh=1.0):
    """Structurally verify many (fb, candidate_c) at once, grouped by oracle so each compiler
    runs chunked DOSBox batches. Returns a list of {match, ratio, oracle} aligned with items."""
    items = list(items)
    results = [None] * len(items)
    for ora, recipes, label in ((router._msc6, router.RECIPES_MSC6, "msc6"),
                                 (router._msvc152, router.RECIPES_MSVC152, "msvc152")):
        idx = [k for k, (fb, _c) in enumerate(items) if router.pick(bytes(fb))[0] is ora]
        if not idx:
            continue
        if not ora.available():
            for k in idx:
                results[k] = {"match": False, "oracle": label, "reason": "oracle not configured"}
            continue
        jobs = [(items[k][1], rcp) for k in idx for rcp in recipes]
        outs = router._compile_batch(ora, jobs)
        for n, k in enumerate(idx):
            ship = normalize(items[k][0]); hit = False; ratio = 0.0
            for j in range(len(recipes)):
                rc = outs[n * len(recipes) + j]
                if rc is None:
                    continue
                cand = normalize(rc)
                if cand == ship:
                    hit = True; ratio = 1.0; break
                ratio = max(ratio, _lcs(ship, cand) / max(len(ship), len(cand), 1))
            results[k] = {"match": hit or ratio >= thresh, "ratio": ratio, "oracle": label}
    return results


if __name__ == "__main__":
    # sanity: same logic, different register + different local offset -> equal normalized stream
    a = normalize(bytes.fromhex("8b46068946f8"))       # mov ax,[bp+6]; mov [bp-8],ax
    b = normalize(bytes.fromhex("8b4e06894efa"))       # mov cx,[bp+6]; mov [bp-6],cx
    print("reg/offset-different equal:", a == b, "|", a)
    c = normalize(bytes.fromhex("8b4606034608"))       # mov ax,[bp+6]; add ax,[bp+8]  (different logic)
    print("different-logic distinct:", a != c)
