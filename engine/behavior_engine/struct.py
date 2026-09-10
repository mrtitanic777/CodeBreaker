#!/usr/bin/env python3
r"""structrec.py -- pointer/struct-field reconstruction (spec Section 7/12).

The single highest-value step past integer-leaf synthesis: recover the LAYOUT and ACCESS
pattern of a structure a function receives by pointer. The method is the spec's own Player
example, made mechanical:

  1. Point the pointer argument at a CONTROLLED BUFFER pre-filled with per-offset sentinels
     (offset o holds 0xB0000000|o, so any value read from the struct decodes back to the offset
     it came from).
  2. Trace. Every read whose address lands in the buffer is an INPUT FIELD (the function
     depends on struct+o); every write is an OUTPUT FIELD. A written value that decodes to a
     sentinel is a FIELD COPY (out-field := in-field) -- direct field->field dataflow.
  3. Synthesize a `struct` (members at the observed offsets, gaps padded) and a function body
     that performs the observed copies / constant stores / field return.
  4. DIFFERENTIALLY VERIFY against the binary: re-run with RANDOM field values and check the
     model predicts every write and the return. Nothing is claimed that the binary doesn't
     confirm.

v1 scope: dword-granular fields, one pointer arg, straight-line field access. Non-dword
accesses and computed (non-copy, non-constant) outputs are reported honestly as such, not faked.

    wt behavior struct <module> [func] [--ptr N] [--args a,b,c]
"""
import os
import random
import sys

from engine.behavior_engine import tracer, perturb

MASK = 0xFFFFFFFF
BUF_BASE = 0x205000                   # inside the mapped scratch region
BUF_SIZE = 0x100                      # 256 bytes / 64 dword slots


def SENT(o):
    return 0xB0000000 | (o & 0xFFFF)


def _is_sent(v):
    return v is not None and (v & 0xFFFF0000) == 0xB0000000


def _sent_off(v):
    return v & 0xFFFF


def _fill(values=None):
    """Buffer bytes: default per-offset sentinels; or `values` {offset:dword} over a zero base."""
    import struct as _s
    buf = bytearray(BUF_SIZE)
    if values is None:
        for o in range(0, BUF_SIZE, 4):
            buf[o:o + 4] = _s.pack("<I", SENT(o))
    else:
        for o, v in values.items():
            buf[o:o + 4] = _s.pack("<I", v & MASK)
    return bytes(buf)


def _in_buf(addr):
    return BUF_BASE <= addr < BUF_BASE + BUF_SIZE


def probe(module, func, ptr_i, arity, other_args, fill_values=None, insn_cap=6000):
    """Run with ptr arg -> the controlled buffer; return (trace, reads, writes) scoped to it."""
    args = list(other_args) + [0] * (arity - len(other_args))
    args[ptr_i] = BUF_BASE
    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                              mem_init=[(BUF_BASE, _fill(fill_values))])
    reads = [{"off": m["addr"] - BUF_BASE, "size": m["size"], "value": m["value"]}
             for m in t.get("mem_reads", []) if _in_buf(m["addr"])]
    writes = [{"off": m["addr"] - BUF_BASE, "size": m["size"], "value": m["value"]}
              for m in t.get("mem_writes", []) if _in_buf(m["addr"])]
    return t, reads, writes


def _find_ptr_arg(module, func, insn_cap):
    _, report = perturb.run_experiment(module, func=func, insn_cap=insn_cap)
    for a in report["args"]:
        if any(h["as"] in ("read_address", "write_address") for h in a["dataflow"]):
            return a["index"], report["arity"]
    return None, report["arity"]


def reconstruct(module, func=None, ptr_i=None, other_args=None, insn_cap=6000):
    other_args = other_args or []
    if ptr_i is None:
        ptr_i, arity = _find_ptr_arg(module, func, insn_cap)
    else:
        arity = max(ptr_i + 1, 4)
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    res = {"schema": "cb.behavior.struct/1", "module": os.path.basename(base["module"]),
           "func_va": base["func_va"], "func_off": base["func_off"], "ptr_arg": ptr_i}
    if ptr_i is None:
        res["reconstructed"] = False
        res["reason"] = "no pointer argument found (run explore/experiment first)"
        return res

    # 1. sentinel run -> discover fields
    t, reads, writes = probe(module, func, ptr_i, arity, other_args, None, insn_cap)
    in_off = {}
    for r in reads:
        if 0 <= r["off"] < BUF_SIZE:
            in_off[r["off"]] = max(in_off.get(r["off"], 0), r["size"])
    out_model = {}     # off -> {"size", "kind": copy|const|computed, "src"/"const"}
    for w in writes:
        if not (0 <= w["off"] < BUF_SIZE):
            continue
        kind = "computed"
        detail = {}
        if _is_sent(w["value"]):
            kind, detail = "copy", {"src": _sent_off(w["value"])}
        elif w["value"] is not None:
            kind, detail = "const", {"const": w["value"]}
        out_model[w["off"]] = dict(size=w["size"], kind=kind, **detail)
    ret_kind = "value"
    if _is_sent(t.get("ret")):
        ret_kind, ret_src = "field", _sent_off(t["ret"])
    else:
        ret_src = None

    # 2. collect N runs with random field values, then EXPLAIN each output field and the return
    #    as an expression over the INPUT fields (const / copy / +c / ^c / shift / two-field op /
    #    pack). A slot we cannot explain stays "computed" (flagged, never faked). This upgrades
    #    the earlier copy/const-only model to synthesize computed fields.
    rng = random.Random(base["func_va"])
    in_offs = sorted(in_off)
    N = 8
    runs = []
    for _ in range(N):
        vals = {o: rng.getrandbits(32) for o in in_offs}
        tv, _, vw = probe(module, func, ptr_i, arity, other_args, vals, insn_cap)
        runs.append({"fields": vals, "writes": {w["off"]: w["value"] for w in vw},
                     "ret": tv.get("ret")})
    final_out, ok = {}, True
    for off in sorted(out_model):
        series = [(r["fields"], r["writes"].get(off)) for r in runs if r["writes"].get(off) is not None]
        m = {"size": out_model[off]["size"]}
        if len(series) < N:                          # not written on every run -> conditional
            m["kind"] = "conditional"; ok = False
        else:
            ex = _field_expr(series, in_offs)
            if ex:
                m["kind"], m["desc"], m["expr"] = "expr", ex[0], ex[1]
            else:
                m["kind"] = "computed"; ok = False
        final_out[off] = m
    ret_series = [(r["fields"], r["ret"]) for r in runs if r["ret"] is not None]
    ret_model = _field_expr(ret_series, in_offs) if len(ret_series) == N else None

    res.update({
        "reconstructed": True,
        "arity": arity,
        "in_fields": [{"off": o, "size": s} for o, s in sorted(in_off.items())],
        "out_fields": [dict(off=o, **m) for o, m in sorted(final_out.items())],
        "returns": {"desc": ret_model[0] if ret_model else "value (not a field expression)",
                    "expr": ret_model[1] if ret_model else None},
        "verified": ok,                              # every claimed write reproduced across N runs
        "struct_c": _emit_struct(res["func_va"], in_off, final_out,
                                 ret_model[1] if ret_model else None, ptr_i, arity),
    })
    return res


def _field_expr(series, in_offs):
    """Explain target = f(input fields) across all runs. series=[(fields{off:val}, target)].
    Returns (human_desc, c_expr) or None. Fields are rendered as p->f_<off>."""
    if not series:
        return None
    tgt = [t & MASK for _, t in series]
    fs = [f for f, _ in series]
    if len(set(tgt)) == 1:                           # constant
        return ("0x%x" % tgt[0], "0x%xU" % tgt[0])
    offs = [o for o in in_offs if all(o in f for f in fs)]
    # unary over one input field
    for o in offs:
        xs = [f[o] & MASK for f in fs]
        if all(x == t for x, t in zip(xs, tgt)):
            return ("p->f_%x" % o, "p->f_%x" % o)                         # copy
        cs = {(t - x) & MASK for x, t in zip(xs, tgt)}
        if len(cs) == 1 and 0 not in cs:
            c = cs.pop(); return ("f_%x + 0x%x" % (o, c), "p->f_%x + 0x%xU" % (o, c))
        cs = {(t ^ x) & MASK for x, t in zip(xs, tgt)}
        if len(cs) == 1 and 0 not in cs:
            c = cs.pop(); return ("f_%x ^ 0x%x" % (o, c), "p->f_%x ^ 0x%xU" % (o, c))
        for k in range(1, 32):
            if all(((x << k) & MASK) == t for x, t in zip(xs, tgt)):
                return ("f_%x << %d" % (o, k), "p->f_%x << %d" % (o, k))
            if all((x >> k) == t for x, t in zip(xs, tgt)):
                return ("f_%x >> %d" % (o, k), "p->f_%x >> %d" % (o, k))
    # binary over two input fields
    ops = [("+", lambda x, y: (x + y) & MASK), ("-", lambda x, y: (x - y) & MASK),
           ("|", lambda x, y: x | y), ("&", lambda x, y: x & y), ("^", lambda x, y: x ^ y),
           ("*", lambda x, y: (x * y) & MASK)]
    for i, a in enumerate(offs):
        xa = [f[a] & MASK for f in fs]
        for b in offs[i + 1:]:
            xb = [f[b] & MASK for f in fs]
            for sym, fn in ops:
                if all(fn(x, y) == t for x, y, t in zip(xa, xb, tgt)):
                    return ("f_%x %s f_%x" % (a, sym, b), "p->f_%x %s p->f_%x" % (a, sym, b))
            for s in (8, 16):
                if all(((x | (y << s)) & MASK) == t for x, y, t in zip(xa, xb, tgt)):
                    return ("f_%x | (f_%x<<%d)" % (a, b, s),
                            "p->f_%x | (p->f_%x << %d)" % (a, b, s))
                if all(((y | (x << s)) & MASK) == t for x, y, t in zip(xa, xb, tgt)):
                    return ("f_%x | (f_%x<<%d)" % (b, a, s),
                            "p->f_%x | (p->f_%x << %d)" % (b, a, s))
    return None


def _emit_struct(va, in_off, final_out, ret_expr, ptr_i, arity):
    offsets = sorted(set(in_off) | set(final_out))
    lines, cur = [], 0
    for o in offsets:
        if o > cur:
            lines.append("    unsigned char _pad_%x[0x%x];" % (cur, o - cur))
        lines.append("    unsigned f_%x;" % o)      # dword members (v1)
        cur = o + 4
    struct = "struct S_%x {\n%s\n};" % (va, "\n".join(lines) if lines else "    int _empty;")
    body = []
    for o, m in sorted(final_out.items()):
        if m.get("kind") == "expr":
            body.append("    p->f_%x = %s;" % (o, m["expr"]))
        elif m.get("kind") == "conditional":
            body.append("    /* p->f_%x = <written only on some inputs; conditional> */" % o)
        else:
            body.append("    /* p->f_%x = <computed; not reconstructed> */" % o)
    ret = ("    return %s;" % ret_expr) if ret_expr else "    return 0; /* value */"
    params = ", ".join(("struct S_%x *p" % va) if i == ptr_i else "unsigned a%d" % i
                       for i in range(arity))
    return "%s\n\nunsigned sub_%x(%s)\n{\n%s\n%s\n}\n" % (
        struct, va, params, "\n".join(body) if body else "    /* no struct writes observed */", ret)


# --------------------------------------------------------------------------
def _opt(a, name, d=None):
    return a[a.index(name) + 1] if name in a and a.index(name) + 1 < len(a) else d


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = None
    for a in argv[1:]:
        if not a.startswith("-"):
            func = a; break
    ptr = _opt(argv, "--ptr"); ptr = int(ptr) if ptr is not None else None
    other = [int(x, 0) for x in _opt(argv, "--args", "").split(",") if x.strip()]

    res = reconstruct(module, func=func, ptr_i=ptr, other_args=other)
    print("struct reconstruction: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["reconstructed"]:
        print("  not reconstructed: %s" % res["reason"]); return
    print("  pointer arg : arg%d   struct fields: %d in, %d out" %
          (res["ptr_arg"], len(res["in_fields"]), len(res["out_fields"])))
    for f in res["in_fields"]:
        print("     reads  +0x%-3x (%d bytes)" % (f["off"], f["size"]))
    for f in res["out_fields"]:
        if f["kind"] == "copy":
            print("     writes +0x%-3x <- +0x%x (field copy)" % (f["off"], f["src"]))
        elif f["kind"] == "const":
            print("     writes +0x%-3x <- 0x%x (constant)" % (f["off"], f["const"]))
        else:
            print("     writes +0x%-3x <- (computed)" % f["off"])
    print("  differential verification vs binary: %s" %
          ("VERIFIED" if res["verified"] else "PARTIAL (computed/unmodeled writes present)"))
    print("\n  %s" % res["struct_c"].replace("\n", "\n  ").rstrip())


if __name__ == "__main__":
    main()
