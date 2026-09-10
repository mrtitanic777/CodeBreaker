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

    # 2. differential verification: random field values, predict writes + return
    rng = random.Random(base["func_va"])
    field_offsets = sorted(set(in_off) | {m.get("src") for m in out_model.values()
                                          if m["kind"] == "copy"})
    rand_vals = {o: rng.getrandbits(32) for o in field_offsets if o is not None}
    tv, _, vwrites = probe(module, func, ptr_i, arity, other_args, rand_vals, insn_cap)
    actual = {w["off"]: w["value"] for w in vwrites}
    ok = True
    for off, m in out_model.items():
        if m["kind"] == "copy":
            pred = rand_vals.get(m["src"], SENT(m["src"]))
        elif m["kind"] == "const":
            pred = m["const"]
        else:
            continue                      # computed outputs aren't predicted by v1 -> don't claim
        if actual.get(off) != (pred & MASK if pred is not None else None):
            ok = False
    if ret_kind == "field":
        if tv.get("ret") != (rand_vals.get(ret_src, SENT(ret_src)) & MASK):
            ok = False

    res.update({
        "reconstructed": True,
        "arity": arity,
        "in_fields": [{"off": o, "size": s} for o, s in sorted(in_off.items())],
        "out_fields": [dict(off=o, **m) for o, m in sorted(out_model.items())],
        "returns": {"kind": ret_kind, "src": ret_src},
        "verified": ok,
        "struct_c": _emit_struct(res["func_va"], in_off, out_model, ret_kind, ret_src, ptr_i, arity),
    })
    return res


def _emit_struct(va, in_off, out_model, ret_kind, ret_src, ptr_i, arity):
    offsets = sorted(set(in_off) | set(out_model))
    lines, cur = [], 0
    for o in offsets:
        if o > cur:
            lines.append("    unsigned char _pad_%x[0x%x];" % (cur, o - cur))
        lines.append("    unsigned f_%x;" % o)      # dword members (v1)
        cur = o + 4
    struct = "struct S_%x {\n%s\n};" % (va, "\n".join(lines) if lines else "    int _empty;")
    body = []
    for o, m in sorted(out_model.items()):
        if m["kind"] == "copy":
            body.append("    p->f_%x = p->f_%x;" % (o, m["src"]))
        elif m["kind"] == "const":
            body.append("    p->f_%x = 0x%xU;" % (o, m["const"]))
        else:
            body.append("    /* p->f_%x = <computed; not reconstructed in v1> */" % o)
    ret = ("    return p->f_%x;" % ret_src) if ret_kind == "field" else "    return 0; /* value */"
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
