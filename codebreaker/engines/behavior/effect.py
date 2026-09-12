#!/usr/bin/env python3
r"""effect.py -- full-effect reconstruction: return value AND memory writes over the
UNIFIED variable space of struct fields + scalar args (the synth<->struct composition).

synth models the return as f(args) but declines any memory side effect. struct models
struct-field writes + the return, but deliberately holds scalar ARGS fixed (varying them
changes which branch runs and erases the field writes it is analyzing), so it cannot see
an output that depends on a scalar arg -- the commonest pointer-function shape:

    SetRect(LPRECT p, int l, int t, int r, int b):  p->left=l; ...; return TRUE;

struct alone, with args pinned to 0, reconstructs `p->left = 0; ...` -- wrong. This
composes the two: vary struct fields AND scalar args together, and explain every write
AND the return over the JOINT variable space (fields f_<off> + args a<i>). It reuses
struct's expression/gate engine (which already accepts arg variables) and its struct
emitter; the only new thing is feeding args into the variable space and the runs.

Faithful-only: samples the real oracle on real inputs (controlled buffer + varied args),
and every claimed write/return must reproduce across many random field+arg combinations.
Scope v1: one pointer arg, dword-granular fields, expression/gated-expression outputs.

    wt behavior effect <module> [func] [--ptr N]
"""
import os
import random
import sys

from codebreaker.engines.behavior import tracer, perturb
from codebreaker.engines.behavior import struct as S
from codebreaker.cli.common import opt as _opt

MASK = 0xFFFFFFFF

# Two-pointer copy detection (CopyRect-style): dst -> buffer A (struct's 0xB sentinels), src ->
# buffer B with a DISTINCT 0xC tag, so a write to A whose value decodes to a B-sentinel is a
# proven cross-pointer field copy dst->f = src->f. B sits well clear of A and the per-arg buffers.
_BUF_B = 0x205800
_SENT_B = 0xC0000000


def _sentB(o):
    return _SENT_B | (o & 0xFFFF)


def _is_sentB(v):
    return v is not None and (v & 0xFFFF0000) == _SENT_B


def _fillB(values=None):
    import struct as _s
    b = bytearray(S.BUF_SIZE)
    if values is None:
        for o in range(0, S.BUF_SIZE, 4):
            b[o:o + 4] = _s.pack("<I", _sentB(o))
    else:
        for o, v in values.items():
            b[o:o + 4] = _s.pack("<I", v & MASK)
    return bytes(b)


def _probe_two(module, func, dst_i, src_i, arity, insn_cap, fillA=None, fillB=None):
    """Run with dst->buffer A and src->buffer B; return (trace, writes-to-A)."""
    args = [0] * arity
    args[dst_i] = S.BUF_BASE
    args[src_i] = _BUF_B
    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                              mem_init=[(S.BUF_BASE, S._fill(fillA)), (_BUF_B, _fillB(fillB))])
    wa = [{"off": m["addr"] - S.BUF_BASE, "size": m["size"], "value": m["value"]}
          for m in t.get("mem_writes", []) if S._in_buf(m["addr"])]
    return t, wa


def _guess_two_ptrs(module, func, arity, insn_cap):
    """Find (dst_i, src_i) by trying arg pairs: the pair whose run writes A with values that
    decode to B-sentinels (a cross-pointer copy) wins. Returns (dst, src) or None."""
    best, best_n = None, 0
    for dst_i in range(min(arity, 4)):
        for src_i in range(min(arity, 4)):
            if dst_i == src_i:
                continue
            try:
                _t, wa = _probe_two(module, func, dst_i, src_i, arity, insn_cap)
            except Exception:
                continue
            n = sum(1 for w in wa if _is_sentB(w["value"]))     # cross-pointer copies observed
            if n > best_n:
                best, best_n = (dst_i, src_i), n
    return best


def reconstruct_copy(module, func, dst_i, src_i, arity, insn_cap=6000, runs=16):
    """Reconstruct a two-pointer field-copy function (dst->f = src->f / const), verified by
    re-running with random field values in both buffers."""
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    res = {"schema": "cb.behavior.effect/1", "mode": "copy", "module": os.path.basename(base["module"]),
           "func_va": base["func_va"], "func_off": base["func_off"],
           "dst_arg": dst_i, "src_arg": arity and src_i, "reconstructed": False}
    _t, wa = _probe_two(module, func, dst_i, src_i, arity, insn_cap)
    model = {}                                            # dst off -> {kind: src|self|const, ...}
    src_offs = set()
    for w in wa:
        o, v = w["off"], w["value"]
        if _is_sentB(v):
            model[o] = {"kind": "src", "src": v & 0xFFFF, "size": w["size"]}; src_offs.add(v & 0xFFFF)
        elif S._is_sent(v):
            model[o] = {"kind": "self", "src": S._sent_off(v), "size": w["size"]}
        elif v is not None:
            model[o] = {"kind": "const", "const": v & MASK, "size": w["size"]}
    if not any(m["kind"] in ("src", "self") for m in model.values()):
        res["reason"] = "no cross-pointer field copy observed (not a copy function)"
        return res
    # verify: random field values in both buffers; each modeled copy must reproduce
    import random
    rng = random.Random(base["func_va"])
    ok = True
    for _ in range(runs):
        av = {o: rng.getrandbits(32) for o in range(0, S.BUF_SIZE, 4)}
        bv = {o: rng.getrandbits(32) for o in range(0, S.BUF_SIZE, 4)}
        _tt, w2 = _probe_two(module, func, dst_i, src_i, arity, insn_cap, fillA=av, fillB=bv)
        got = {w["off"]: w["value"] for w in w2}
        for o, m in model.items():
            exp = (bv.get(m["src"]) if m["kind"] == "src"
                   else av.get(m["src"]) if m["kind"] == "self" else m["const"])
            if got.get(o) is not None and (got[o] & MASK) != (exp & MASK):
                ok = False; break
        if not ok:
            break
    offs = sorted(set(model) | src_offs)
    lines, cur = [], 0
    for o in offs:
        if o > cur:
            lines.append("    unsigned char _pad_%x[0x%x];" % (cur, o - cur))
        lines.append("    unsigned f_%x;" % o); cur = o + 4
    body = []
    for o, m in sorted(model.items()):
        if m["kind"] == "src":
            body.append("    dst->f_%x = src->f_%x;" % (o, m["src"]))
        elif m["kind"] == "self":
            if m["src"] == o:                                # same slot: read-modify-write, no-op
                body.append("    /* dst->f_%x read and written back unchanged (no-op) */" % o)
            else:                                            # intra-struct field copy (meaningful)
                body.append("    dst->f_%x = dst->f_%x;" % (o, m["src"]))
        else:
            body.append("    dst->f_%x = 0x%xU;" % (o, m["const"]))
    struct_c = ("struct S_%x {\n%s\n};\n\nvoid sub_%x(struct S_%x *dst, const struct S_%x *src)\n{\n%s\n}\n"
                % (res["func_va"], "\n".join(lines), res["func_va"], res["func_va"], res["func_va"],
                   "\n".join(body)))
    res.update(reconstructed=True, verified=ok, copies=len(model),
               out_fields=[dict(off=o, **m) for o, m in sorted(model.items())], struct_c=struct_c)
    return res


def _explain_narrow(recs, off, vars_, size):
    """Width-aware explanation of a sub-dword field write (size 1/2): the stored value is
    truncated, so match modulo the width. Returns (desc, c_expr) or None. The emitted field
    is typed to the width (unsigned char/short), so `f = a1` truncates -- no explicit mask
    needed. Only used for size<4; the dword path stays with struct._expr."""
    mask = (1 << (8 * size)) - 1
    tgt = [(r["writes"][off] & mask) for r in recs]
    if len(set(tgt)) == 1:                                # constant (fits the width)
        return ("0x%x" % tgt[0], "0x%xU" % tgt[0])
    for k in vars_:                                       # a field/arg whose low bits are the value
        if all(k in r["vars"] for r in recs):
            xs = [(r["vars"][k] & mask) for r in recs]
            if xs == tgt:
                return (S._vd(k), S._vc(k))
    return None


def _recovered_arity(module, func, insn_cap):
    try:
        _, report = perturb.run_experiment(module, func=func, insn_cap=insn_cap)
        return report["arity"]
    except Exception:
        return 4


def _guess_ptr(module, func, arity, insn_cap):
    """When perturb can't flag a pointer arg (a setter with a null baseline writes to address 0,
    which perturb doesn't correlate to an arg), try each early arg as the pointer: aim it at the
    sentinel buffer and keep the one that produces the most struct read/write activity."""
    best, best_n = None, 0
    for cand in range(min(arity, 4)):
        try:
            _t, reads, writes = S.probe(module, func, cand, arity, [], None, insn_cap)
        except Exception:
            continue
        n = len(reads) + len(writes)
        if n > best_n:
            best, best_n = cand, n
    return best


def reconstruct(module, func=None, ptr_i=None, arity=None, insn_cap=6000, runs=20):
    if arity is None:
        arity = max(_recovered_arity(module, func, insn_cap), 8)
    if ptr_i is None:                                       # two-pointer copy (CopyRect) auto-route
        two = _guess_two_ptrs(module, func, arity, insn_cap)
        if two:
            return reconstruct_copy(module, func, two[0], two[1], arity, insn_cap, runs=min(runs, 16))
    if ptr_i is None:
        ptr_i, _a0 = S._find_ptr_arg(module, func, insn_cap)
    if ptr_i is None:
        ptr_i = _guess_ptr(module, func, arity, insn_cap)   # fallback: probe each arg as the pointer
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    res = {"schema": "cb.behavior.effect/1", "module": os.path.basename(base["module"]),
           "func_va": base["func_va"], "func_off": base["func_off"], "ptr_arg": ptr_i,
           "reconstructed": False}
    if ptr_i is None:
        res["reason"] = "no pointer argument -- a pure-value function is synth/piecewise's job"
        return res
    # Variable space = fields + ALL scalar args. perturb flags args that influence the
    # return/path/an address, but an arg that flows into a struct-WRITE VALUE (the whole point
    # of a setter like SetRect) shows up in neither -- so include every scalar arg and let _expr
    # pick the ones that matter. Arity is taken generously (recovered, else >= 8 slots) since a
    # field-setter has an arg per field and a too-small arity would miss the last ones; extra
    # unused stack slots are harmless (we stop at the RET sentinel, cleanup is irrelevant).
    arity = max(arity, ptr_i + 1)
    sargs = [i for i in range(arity) if i != ptr_i]

    # 1. sentinel run -> discover input/output fields (struct's discovery, unchanged)
    _t, reads, writes = S.probe(module, func, ptr_i, arity, [], None, insn_cap)
    in_off, out_size = {}, {}
    for r in reads:
        if 0 <= r["off"] < S.BUF_SIZE:
            in_off[r["off"]] = max(in_off.get(r["off"], 0), r["size"])
    for w in writes:
        if 0 <= w["off"] < S.BUF_SIZE:
            out_size[w["off"]] = max(out_size.get(w["off"], 0), w["size"])

    # 2. JOINT variable space: input fields + affecting scalar args (the new part)
    in_offs = sorted(in_off)
    vars_ = [("f", o) for o in in_offs] + [("a", i) for i in sargs]

    # 3. runs varying BOTH fields and scalar args
    rng = random.Random(base["func_va"])
    argspread = [0, 1, 2, 16, 0x100, 0xFFFFFFFF, 0x7FFFFFFF]
    recs = []
    for _ in range(runs):
        fvals = {o: rng.getrandbits(32) for o in in_offs}
        avals = {i: (rng.choice(argspread) if rng.random() < 0.5 else rng.getrandbits(32)) for i in sargs}
        tv, _rr, vw = S.probe(module, func, ptr_i, arity, [], fvals, insn_cap, arg_vals=avals)
        vmap = {("f", o): fvals[o] for o in in_offs}
        vmap.update({("a", i): avals[i] & MASK for i in sargs})
        recs.append({"vars": vmap, "writes": {w["off"]: w["value"] for w in vw}, "ret": tv.get("ret")})

    # 4. explain each output field + the return over the JOINT space (struct's engine, reused)
    final_out, ok = {}, True
    for off in sorted(out_size):
        wrote = [off in r["writes"] and r["writes"][off] is not None for r in recs]
        m = {"size": out_size[off]}
        if all(wrote):
            ex = None
            if out_size[off] < 4:                         # sub-dword: width-aware match first
                ex = _explain_narrow(recs, off, vars_, out_size[off])
            if ex is None:
                ex = S._expr([(r["vars"], r["writes"][off]) for r in recs], vars_)
            if ex:
                m.update(kind="expr", desc=ex[0], expr=ex[1])
            else:
                m["kind"] = "computed"; ok = False
        elif any(wrote):
            gate = S._gate([(r["vars"], w) for r, w in zip(recs, wrote)], vars_)
            val = S._expr([(r["vars"], r["writes"][off]) for r, w in zip(recs, wrote) if w], vars_)
            if gate and val:
                m.update(kind="cond", gate=gate[0], gate_c=gate[1], desc=val[0], expr=val[1])
            else:
                m["kind"] = "conditional"; ok = False
        else:
            m["kind"] = "none"
        final_out[off] = m
    ret_series = [(r["vars"], r["ret"]) for r in recs if r["ret"] is not None]
    ret_model = S._expr(ret_series, vars_) if len(ret_series) == len(recs) else None

    # trim the emitted signature to args actually referenced (drop the generous-arity padding)
    import re as _re
    used = set()
    for m in final_out.values():
        for key in ("expr", "gate_c"):
            if m.get(key):
                used |= {int(x) for x in _re.findall(r"\ba(\d+)\b", m[key])}
    if ret_model and ret_model[1]:
        used |= {int(x) for x in _re.findall(r"\ba(\d+)\b", ret_model[1])}
    emit_arity = max([ptr_i + 1] + [u + 1 for u in used])

    res.update({
        "reconstructed": True, "arity": arity, "uses_args": sargs,
        "in_fields": [{"off": o, "size": s} for o, s in sorted(in_off.items())],
        "out_fields": [dict(off=o, **m) for o, m in sorted(final_out.items())],
        "returns": {"desc": ret_model[0] if ret_model else "value (not an expression)",
                    "expr": ret_model[1] if ret_model else None},
        "verified": ok,
        "struct_c": S._emit_struct(res["func_va"], in_off, final_out,
                                   ret_model[1] if ret_model else None, ptr_i, emit_arity),
    })
    return res


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = next((a for a in argv[1:] if not a.startswith("-")), None)
    ptr = _opt(argv, "--ptr"); ptr = int(ptr) if ptr is not None else None
    arity = _opt(argv, "--arity"); arity = int(arity) if arity is not None else None
    res = reconstruct(module, func=func, ptr_i=ptr, arity=arity)
    print("full-effect reconstruction: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["reconstructed"]:
        print("  not reconstructed: %s" % res["reason"]); return
    if res.get("mode") == "copy":                           # two-pointer field-copy result
        print("  TWO-POINTER COPY: dst=arg%d src=arg%d   %d field copies" %
              (res["dst_arg"], res["src_arg"], res["copies"]))
        print("  differential verification vs binary: %s"
              % ("VERIFIED" if res["verified"] else "PARTIAL"))
        print("\n  %s" % res["struct_c"].replace("\n", "\n  ").rstrip())
        return
    print("  pointer arg : arg%d   scalar args used: %s   fields: %d in, %d out"
          % (res["ptr_arg"], res["uses_args"] or "(none)", len(res["in_fields"]), len(res["out_fields"])))
    for f in res["out_fields"]:
        if f["kind"] == "expr":
            print("     writes +0x%-3x = %s" % (f["off"], f["desc"]))
        elif f["kind"] == "cond":
            print("     writes +0x%-3x = %s   if %s" % (f["off"], f["desc"], f["gate"]))
        elif f["kind"] == "none":
            print("     +0x%-3x : never written in these runs" % f["off"])
        else:
            print("     writes +0x%-3x : <%s; not reconstructed>" % (f["off"], f["kind"]))
    print("  returns     : %s" % res["returns"]["desc"])
    print("  differential verification vs binary: %s"
          % ("VERIFIED" if res["verified"] else "PARTIAL (unmodeled writes present)"))
    print("\n  %s" % res["struct_c"].replace("\n", "\n  ").rstrip())


if __name__ == "__main__":
    main()
