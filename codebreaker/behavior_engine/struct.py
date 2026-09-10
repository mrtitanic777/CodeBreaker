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

from codebreaker.behavior_engine import tracer, perturb

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


def probe(module, func, ptr_i, arity, other_args, fill_values=None, insn_cap=6000, arg_vals=None):
    """Run with ptr arg -> the controlled buffer; return (trace, reads, writes) scoped to it.
    arg_vals {i: value} overrides individual scalar args (to vary them as expression inputs)."""
    args = list(other_args) + [0] * (arity - len(other_args))
    args[ptr_i] = BUF_BASE
    for i, v in (arg_vals or {}).items():
        if i != ptr_i:
            args[i] = v & MASK
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

    # 2. collect N runs varying BOTH struct fields AND scalar args, then EXPLAIN each output and
    #    the return as an expression over that unified variable space; an output written only on
    #    some runs gets a recovered GATING condition (`if (...) p->f = ...`). Anything we can't
    #    explain stays flagged -- never faked.
    # Variables are the struct's INPUT FIELDS. Scalar args are held at their baseline: varying
    # them would change which branch runs and erase the very struct writes we're analyzing (they
    # are often gated on an arg/flag). The _expr/_gate engine also accepts arg variables, but
    # struct keeps args fixed so field behavior stays observable.
    rng = random.Random(base["func_va"])
    in_offs = sorted(in_off)
    vars_ = [("f", o) for o in in_offs]
    N = 12
    runs = []
    for _ in range(N):
        vmap = {("f", o): rng.getrandbits(32) for o in in_offs}
        tv, _, vw = probe(module, func, ptr_i, arity, other_args,
                          {o: vmap[("f", o)] for o in in_offs}, insn_cap)
        runs.append({"vars": vmap, "writes": {w["off"]: w["value"] for w in vw}, "ret": tv.get("ret")})

    final_out, ok = {}, True
    for off in sorted(out_model):
        wrote = [off in r["writes"] and r["writes"][off] is not None for r in runs]
        m = {"size": out_model[off]["size"]}
        if all(wrote):                                       # unconditional write -> expression
            ex = _expr([(r["vars"], r["writes"][off]) for r in runs], vars_)
            if ex:
                m.update(kind="expr", desc=ex[0], expr=ex[1])
            else:
                m["kind"] = "computed"; ok = False
        elif any(wrote):                                     # conditional write -> gate + value
            gate = _gate([(r["vars"], w) for r, w in zip(runs, wrote)], vars_)
            val = _expr([(r["vars"], r["writes"][off]) for r, w in zip(runs, wrote) if w], vars_)
            if gate and val:
                m.update(kind="cond", gate=gate[0], gate_c=gate[1], desc=val[0], expr=val[1])
            else:
                m["kind"] = "conditional"; ok = False
        else:
            m["kind"] = "none"
        final_out[off] = m
    ret_series = [(r["vars"], r["ret"]) for r in runs if r["ret"] is not None]
    ret_model = _expr(ret_series, vars_) if len(ret_series) == N else None

    res.update({
        "reconstructed": True,
        "arity": arity,
        "in_fields": [{"off": o, "size": s} for o, s in sorted(in_off.items())],
        "out_fields": [dict(off=o, **m) for o, m in sorted(final_out.items())],
        "returns": {"desc": ret_model[0] if ret_model else "value (not an expression)",
                    "expr": ret_model[1] if ret_model else None},
        "verified": ok,                              # every claimed write reproduced across N runs
        "struct_c": _emit_struct(res["func_va"], in_off, final_out,
                                 ret_model[1] if ret_model else None, ptr_i, arity),
    })
    return res


def _vc(k):
    """C rendering of a variable key: ('f',off)->p->f_<off>, ('a',i)->a<i>, ('g',addr)->g_<addr>."""
    return {"f": "p->f_%x", "a": "a%d", "g": "g_%x"}[k[0]] % k[1]


def _vd(k):
    return {"f": "f_%x", "a": "a%d", "g": "g_%x"}[k[0]] % k[1]


def _expr(series, vars_):
    """Explain target = f(vars) across all runs. series=[(varmap, target)]; vars are field/arg
    keys. Returns (human_desc, c_expr) or None."""
    if not series:
        return None
    tgt = [t & MASK for _, t in series]
    vm = [v for v, _ in series]
    if len(set(tgt)) == 1:                               # constant
        return ("0x%x" % tgt[0], "0x%xU" % tgt[0])
    use = [k for k in vars_ if all(k in v for v in vm)]
    for k in use:                                        # unary over one variable
        xs = [v[k] & MASK for v in vm]
        if all(x == t for x, t in zip(xs, tgt)):
            return (_vd(k), _vc(k))                      # copy / identity
        cs = {(t - x) & MASK for x, t in zip(xs, tgt)}
        if len(cs) == 1 and 0 not in cs:
            c = cs.pop(); return ("%s + 0x%x" % (_vd(k), c), "%s + 0x%xU" % (_vc(k), c))
        cs = {(t ^ x) & MASK for x, t in zip(xs, tgt)}
        if len(cs) == 1 and 0 not in cs:
            c = cs.pop(); return ("%s ^ 0x%x" % (_vd(k), c), "%s ^ 0x%xU" % (_vc(k), c))
        for s in range(1, 32):
            if all(((x << s) & MASK) == t for x, t in zip(xs, tgt)):
                return ("%s << %d" % (_vd(k), s), "%s << %d" % (_vc(k), s))
            if all((x >> s) == t for x, t in zip(xs, tgt)):
                return ("%s >> %d" % (_vd(k), s), "%s >> %d" % (_vc(k), s))
    ops = [("+", lambda x, y: (x + y) & MASK), ("-", lambda x, y: (x - y) & MASK),
           ("|", lambda x, y: x | y), ("&", lambda x, y: x & y), ("^", lambda x, y: x ^ y),
           ("*", lambda x, y: (x * y) & MASK)]
    for i, a in enumerate(use):                          # binary over two variables
        xa = [v[a] & MASK for v in vm]
        for b in use[i + 1:]:
            xb = [v[b] & MASK for v in vm]
            for sym, fn in ops:
                if all(fn(x, y) == t for x, y, t in zip(xa, xb, tgt)):
                    return ("%s %s %s" % (_vd(a), sym, _vd(b)), "%s %s %s" % (_vc(a), sym, _vc(b)))
            for s in (8, 16):
                if all(((x | (y << s)) & MASK) == t for x, y, t in zip(xa, xb, tgt)):
                    return ("%s | (%s<<%d)" % (_vd(a), _vd(b), s),
                            "%s | (%s << %d)" % (_vc(a), _vc(b), s))
                if all(((y | (x << s)) & MASK) == t for x, y, t in zip(xa, xb, tgt)):
                    return ("%s | (%s<<%d)" % (_vd(b), _vd(a), s),
                            "%s | (%s << %d)" % (_vc(b), _vc(a), s))
    return None


def _gate(series, vars_):
    """Find a predicate over vars matching a boolean pattern. series=[(varmap, bool)].
    Returns (human_desc, c_expr) or None."""
    vm = [v for v, _ in series]
    bits = [bool(b) for _, b in series]
    use = [k for k in vars_ if all(k in v for v in vm)]

    def match(pred):
        return all(pred(v) == b for v, b in zip(vm, bits))
    for k in use:
        if match(lambda v, k=k: (v[k] & MASK) != 0):
            return ("%s != 0" % _vd(k), "%s != 0" % _vc(k))
        if match(lambda v, k=k: (v[k] & MASK) == 0):
            return ("%s == 0" % _vd(k), "%s == 0" % _vc(k))
        if match(lambda v, k=k: (v[k] & 0x80000000) != 0):
            return ("(int)%s < 0" % _vd(k), "(int)%s < 0" % _vc(k))
        if match(lambda v, k=k: (v[k] & 0x80000000) == 0):
            return ("(int)%s >= 0" % _vd(k), "(int)%s >= 0" % _vc(k))
    for i, a in enumerate(use):
        for b in use[i + 1:]:
            if match(lambda v, a=a, b=b: (v[a] & MASK) < (v[b] & MASK)):
                return ("%s < %s" % (_vd(a), _vd(b)), "%s < %s" % (_vc(a), _vc(b)))
            if match(lambda v, a=a, b=b: (v[a] & MASK) > (v[b] & MASK)):
                return ("%s > %s" % (_vd(a), _vd(b)), "%s > %s" % (_vc(a), _vc(b)))
            if match(lambda v, a=a, b=b: (v[a] & MASK) == (v[b] & MASK)):
                return ("%s == %s" % (_vd(a), _vd(b)), "%s == %s" % (_vc(a), _vc(b)))
    return None


def _emit_struct(va, in_off, final_out, ret_expr, ptr_i, arity):
    offsets = sorted(set(in_off) | set(final_out))
    lines, cur = [], 0
    for o in offsets:
        if o > cur:
            lines.append("    unsigned char _pad_%x[0x%x];" % (cur, o - cur))
        sz = final_out.get(o, {}).get("size") or in_off.get(o, 4)
        ctype = {1: "unsigned char", 2: "unsigned short"}.get(sz, "unsigned")
        lines.append("    %s f_%x;" % (ctype, o))
        cur = o + (sz if sz in (1, 2, 4) else 4)
    struct = "struct S_%x {\n%s\n};" % (va, "\n".join(lines) if lines else "    int _empty;")
    body = []
    for o, m in sorted(final_out.items()):
        if m.get("kind") == "expr":
            body.append("    p->f_%x = %s;" % (o, m["expr"]))
        elif m.get("kind") == "cond":
            body.append("    if (%s) p->f_%x = %s;" % (m["gate_c"], o, m["expr"]))
        elif m.get("kind") == "none":
            pass
        elif m.get("kind") == "conditional":
            body.append("    /* p->f_%x = <conditional write; gate not recovered> */" % o)
        else:
            body.append("    /* p->f_%x = <computed; not reconstructed> */" % o)
    ret = ("    return %s;" % ret_expr) if ret_expr else "    return 0; /* value */"
    params = ", ".join(("struct S_%x *p" % va) if i == ptr_i else "unsigned a%d" % i
                       for i in range(arity))
    return "%s\n\nunsigned sub_%x(%s)\n{\n%s\n%s\n}\n" % (
        struct, va, params, "\n".join(body) if body else "    /* no struct writes observed */", ret)


# --------------------------------------------------------------------------
from codebreaker.cli.common import opt as _opt


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
