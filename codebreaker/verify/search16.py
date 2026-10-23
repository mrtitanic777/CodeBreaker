"""search16.py -- the last bytes of a near miss, found by trying small rewrites of its source.

    python -m codebreaker match <16-bit module> --search ROUNDS

A function a few bytes from the original under Microsoft C (verify/match16.py) is often one the
compiler would make exactly from a source written a little differently: its locals declared in
another order (the compiler lays the frame out by declaration, so every [bp-N] moves), or one of
them unsigned where the decompiler guessed signed (a compare, a shift, a widening). Each round
takes every near miss's best text so far, rewrites it one small way, compiles all of them (one
emulator session for each compiler and switches), and keeps a rewrite that comes nearer. A rewrite
that comes out identical is source: the compiler says so, byte for byte.
"""
import random
import re

_DECL = re.compile(r"^(\s+)((?:unsigned |signed )?\w+)(\s+\**\s*)(\w+)(\s*(?:\[[^\]]*\])?\s*;)\s*$")
_SIGN = {"int1": "uint1", "uint1": "int1", "int2": "uint2", "uint2": "int2", "int4": "uint4", "uint4": "int4",
         "xunknown2": "int2", "xunknown4": "int4", "xunknown1": "int1"}


def _locals(lines):
    """the indices of the body's leading declarations (not the K&R parameter declarations)"""
    start = next((k for k, ln in enumerate(lines) if ln.strip() == "{"), None)
    if start is None:
        return []
    out = []
    for k in range(start + 1, len(lines)):
        if _DECL.match(lines[k]):
            out.append(k)
        elif lines[k].strip():
            break
    return out


# A comparison with a constant, as the decompiler normalizes it (`x <= 0x19`) and as a source may
# have had it (`x < 0x1a`): the compiler writes the constant given and the jump that goes with it.
_COMPARE = re.compile(r"(?<![<>=!])(<=|>=|<|>)\s*(-?)(0x[0-9a-fA-F]+|\d+)\b")


def _compare(m):
    op, neg, num = m.group(1), m.group(2), m.group(3)
    v = -int(num, 0) if neg else int(num, 0)
    op, v = {"<=": ("<", v + 1), "<": ("<=", v - 1), ">=": (">", v - 1), ">": (">=", v + 1)}[op]
    return "%s %s" % (op, ("-0x%x" % -v) if v < 0 else ("0x%x" % v))


# Ghidra puts the constant first (`0x19 < iVar1`); a source had the variable first, and the
# comparison it meant may be the other side of the constant (`iVar1 >= 0x1a`).
_LEFT = re.compile(r"\(\s*(-?)(0x[0-9a-fA-F]+|\d+)\s*(<=|>=|<|>)\s*([\w.\[\]]+)\s*\)")


def _left(m):
    neg, num, op, var = m.groups()
    v = -int(num, 0) if neg else int(num, 0)
    op, v = {"<": (">=", v + 1), "<=": (">", v - 1), ">": ("<=", v - 1), ">=": ("<", v + 1)}[op]
    return "(%s %s %s)" % (var, op, ("-0x%x" % -v) if v < 0 else ("0x%x" % v))


_IF = re.compile(r"^(\s*)if \((.*)\) \{\s*$")


def _if_else(lines):
    """[(if line, else line, end line)] of each `if (c) { ... } else { ... }` written a block to a line"""
    out = []
    for k, ln in enumerate(lines):
        m = _IF.match(ln)
        if not m:
            continue
        ind = m.group(1)
        close = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if close is None or close + 1 >= len(lines) or lines[close + 1] != ind + "else {":
            continue
        end = next((j for j in range(close + 2, len(lines)) if lines[j] == ind + "}"), None)
        if end is not None:
            out.append((k, close + 1, end))
    return out


def _negate(cond):
    """`cond` said the other way: a comparison turned around, anything else under a !"""
    m = re.fullmatch(r"\s*([^()&|!<>=]+?)\s*(==|!=|<=|>=|<|>)\s*([^()&|!<>=]+?)\s*", cond)
    if m:
        return "%s %s %s" % (m.group(1), {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}[m.group(2)],
                             m.group(3))
    return "!(%s)" % cond


def rewrite(text, rng):
    """one small rewrite of `text`, or None when there is nothing of the kind to rewrite"""
    lines = text.split("\n")
    decls = _locals(lines)
    kinds = []
    if len(decls) > 1:
        kinds.append("order")
    if any(_DECL.match(lines[k]).group(2) in _SIGN for k in decls):
        kinds.append("sign")
    body = next((k for k, ln in enumerate(lines) if ln.strip() == "{"), 0)
    compares = [(k, m, _compare) for k in range(body, len(lines)) for m in _COMPARE.finditer(lines[k])]
    compares += [(k, m, _left) for k in range(body, len(lines)) for m in _LEFT.finditer(lines[k])]
    if compares:
        kinds += ["compare"] * 2
    branches = _if_else(lines)
    if branches:
        kinds += ["branch"] * 2
    if not kinds:
        return None
    kind = rng.choice(kinds)
    if kind == "branch":                             # `if (c) {A} else {B}` -> `if (!c) {B} else {A}`
        k, e, end = rng.choice(branches)
        m = _IF.match(lines[k])
        a, b = lines[k + 1:e - 1], lines[e + 1:end]
        lines[k:end + 1] = ([m.group(1) + "if (" + _negate(m.group(2)) + ") {"] + b + [m.group(1) + "}",
                            m.group(1) + "else {"] + a + [m.group(1) + "}"])
    elif kind == "compare":
        k, m, how = rng.choice(compares)
        lines[k] = lines[k][:m.start()] + how(m) + lines[k][m.end():]
    elif kind == "order":
        a, b = rng.sample(decls, 2)
        lines[a], lines[b] = lines[b], lines[a]
    else:
        k = rng.choice([k for k in decls if _DECL.match(lines[k]).group(2) in _SIGN])
        m = _DECL.match(lines[k])
        lines[k] = m.group(1) + _SIGN[m.group(2)] + m.group(3) + m.group(4) + m.group(5)
    return "\n".join(lines)


def search(near, compile_group, score, rounds, seed=1995, log=print):
    """near: {fn: (text, verdict, group)} -- each near miss's best text, its verdict and the
    compiler and switches it is judged under; compile_group(group, {fn: text}) -> {fn: verdict};
    score(verdict) -> number, higher nearer. -> {fn: (text, verdict)} the best found."""
    rng = random.Random(seed)
    best = {fn: (t, v) for fn, (t, v, _g) in near.items()}
    group = {fn: g for fn, (_t, _v, g) in near.items()}
    for r in range(rounds):
        tries = {}
        for fn, (t, v) in best.items():
            if v[0] == "identical":
                continue
            new = rewrite(t, rng)
            if new and new != t:
                tries[fn] = new
        if not tries:
            break
        gained = 0
        for g in sorted({group[fn] for fn in tries}):
            mine = {fn: t for fn, t in tries.items() if group[fn] == g}
            for fn, v in compile_group(g, mine).items():
                if score(v) > score(best[fn][1]):
                    best[fn] = (mine[fn], v)
                    gained += 1
        if log:
            log("  round %d: %d rewrites, %d nearer, %d identical" % (
                r + 1, len(tries), gained, sum(v[0] == "identical" for _t, v in best.values())))
    return best
