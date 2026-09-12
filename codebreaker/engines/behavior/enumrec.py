#!/usr/bin/env python3
r"""enumrec.py -- reconstruct app-specific enums from constants used together.

constants maps magic numbers onto KNOWN SDK flag/enum names. But an app's own enums aren't in any
SDK -- they show up as a variable compared against a cluster of related integer constants (a state
machine, a message/command kind, a mode), or as a switch's case values. This groups those
constants back into a recovered `enum` declaration, so a variable that is really an enum reads as
one instead of a scatter of magic numbers. Heuristic and conservative: only a variable compared
against several distinct small constants (or a switch's cases) becomes an enum.
"""
import re
from codebreaker.engines.behavior import tracer

_CMP = re.compile(r"^(?:(?:byte|word|dword) ptr )?(\[[^\]]+\]|e[a-z]{2}|[a-z]{2}|[a-z][a-z]l),"
                  r"\s*(0x[0-9a-f]+|\d+)$")


def enum_from_values(values, tag):
    """Emit an `enum <tag> { ... }` for a set of integer values."""
    vals = sorted(set(values))
    if len(vals) < 2:
        return None
    out = ["enum %s {" % tag]
    for v in vals:
        out.append("    K_%X = 0x%X," % (v, v) if v > 9 else "    K_%d = %d," % (v, v))
    out.append("};")
    return "\n".join(out)


def scan_rows(rows, min_distinct=3, max_val=0x10000):
    """{operand: [constants]} for operands compared against >= min_distinct distinct small constants."""
    groups = {}
    for _a, mn, ops, _sz in rows:
        if mn != "cmp":
            continue
        m = _CMP.match(ops.strip())
        if not m:
            continue
        operand, cst = m.group(1), m.group(2)
        try:
            v = int(cst, 0)
        except ValueError:
            continue
        if v > max_val:
            continue
        groups.setdefault(operand, set()).add(v)
    return {op: sorted(vs) for op, vs in groups.items() if len(vs) >= min_distinct}


def reconstruct(module, va, from_switch=None):
    """Recover enums for a function: from switch cases (if given) and from compare clusters."""
    enums = []
    if from_switch and len(set(from_switch)) >= 2:
        e = enum_from_values(from_switch, "e_switch")
        if e:
            enums.append(e)
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        rows = [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]
    except Exception:
        rows = []
    for k, (op, vals) in enumerate(scan_rows(rows).items()):
        e = enum_from_values(vals, "e_sub_%x_%d" % (off if rows else 0, k))
        if e:
            enums.append("/* %s compared against: %s */\n%s" % (op, ", ".join(hex(v) for v in vals), e))
    if not enums:
        return {"ok": False, "reason": "no enum-shaped constant clusters"}
    return {"ok": True, "nenums": len(enums), "c": "\n\n".join(enums)}
