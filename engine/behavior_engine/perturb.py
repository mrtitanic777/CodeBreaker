#!/usr/bin/env python3
r"""perturb.py -- controlled behavioral experiments over a traced function.

This is the first step past passive recording: instead of only observing one run, it
deliberately CHANGES one input at a time, holding the others fixed, and compares the
resulting trace against a baseline. What changed, and which input changed it, is
behavioral evidence about what that input MEANS -- the spec's "change X only, observe
what changes" (and the foundation the later active-experiment layer builds on).

Two kinds of evidence come out of a one-variable-at-a-time sweep:

  * INFLUENCE (black-box): the variant's return value / path / touched addresses differ
    from the baseline only in the arg we changed -> that arg influences that effect.
  * DATAFLOW (tagged, white-box): each varied arg is set to a DISTINCTIVE tag value
    (0x<i>1<i>1<i>1<i>1). If that exact tag later appears as a memory address, a written
    value, or the return value, the arg flowed THERE verbatim -- direct evidence that,
    e.g., "arg2 is the pointer dereferenced at pc X" or "arg0 is returned unchanged".
    (A transformed arg -- arg0<<24, say -- won't match the tag, but INFLUENCE still
    catches it via the changed return. The two are complementary.)

Everything here is DERIVED: it is computed from raw traces and may be regenerated. The
raw traces each experiment produces are themselves recorded (immutably) by corpus.py.
"""
from engine.behavior_engine import tracer


def _tag(i):
    """A distinctive, recognizable per-arg value (not 0/1/small, unlikely to collide with a
    real constant): 0x_1_1_1_1 with the arg index in the high nibble groups."""
    n = (i & 0xF)
    return ((0x10 | n) << 24) | ((0x10 | n) << 16) | (0x1100) | 0x11


def _near(tag, value, slop=0x400):
    """True if value looks like tag or tag+small-displacement (arg used as a base pointer with
    a struct-field offset), within +/- slop bytes."""
    if value is None:
        return False
    return abs((value & 0xFFFFFFFF) - tag) <= slop


def diff_traces(base, var):
    """Structural diff of two TraceRecords. Returns a dict of what differs (no interpretation)."""
    d = {}
    if base.get("ret") != var.get("ret"):
        d["ret"] = {"base": base.get("ret"), "var": var.get("ret")}
    if base.get("insn_count") != var.get("insn_count"):
        d["insn_count"] = {"base": base["insn_count"], "var": var["insn_count"]}
    if base.get("exit") != var.get("exit"):
        d["exit"] = {"base": base["exit"], "var": var["exit"]}
    # first point where the executed path diverges (a branch decision changed)
    pa, pb = base.get("path", []), var.get("path", [])
    for k in range(min(len(pa), len(pb))):
        if pa[k] != pb[k]:
            d["path_diverges_at"] = {"index": k, "base_pc": pa[k], "var_pc": pb[k]}
            break
    # addresses touched that the baseline did not (new memory reached because of the input)
    base_r = {m["addr"] for m in base.get("mem_reads", [])}
    var_r = {m["addr"] for m in var.get("mem_reads", [])}
    base_w = {m["addr"] for m in base.get("mem_writes", [])}
    var_w = {m["addr"] for m in var.get("mem_writes", [])}
    if var_r - base_r:
        d["new_read_addrs"] = sorted(var_r - base_r)[:16]
    if var_w - base_w:
        d["new_write_addrs"] = sorted(var_w - base_w)[:16]
    return d


def _dataflow(tag, trace):
    """Where did this arg's tag value show up in the variant trace? Direct-flow evidence."""
    hits = []
    for m in trace.get("mem_reads", []):
        if _near(tag, m["addr"]):
            hits.append({"as": "read_address", "pc": m["pc"], "addr": m["addr"]})
    for m in trace.get("mem_writes", []):
        if _near(tag, m["addr"]):
            hits.append({"as": "write_address", "pc": m["pc"], "addr": m["addr"]})
        if _near(tag, m.get("value"), slop=0):
            hits.append({"as": "written_value", "pc": m["pc"], "value": m["value"]})
    if _near(tag, trace.get("ret"), slop=0):
        hits.append({"as": "return_value", "value": trace.get("ret")})
    return hits


def run_experiment(module, func=None, base_args=None, arity=None, insn_cap=4000):
    """One-variable-at-a-time sweep. Returns (baseline_trace, report).

    report = {
      "module","func_va","arity","base_args",
      "baseline": {"exit","ret","insn_count"},
      "args": [ {"index","tag","influence": <diff_traces dict>, "dataflow": [...],
                 "verdict": "influences return | influences path | flows to address | ... | no effect"} ]
    }
    """
    baseline = tracer.trace_function(module, func=func, args=base_args, arity=arity, insn_cap=insn_cap)
    n = baseline["input"]["arity"]
    base = list(baseline["input"]["args"])            # whatever the baseline actually used
    out_args = []
    for i in range(n):
        tag = _tag(i)
        variant = list(base)
        variant[i] = tag
        vt = tracer.trace_function(module, func=func, args=variant, insn_cap=insn_cap)
        infl = diff_traces(baseline, vt)
        flow = _dataflow(tag, vt)
        verdict = []
        if any(h["as"] == "return_value" for h in flow):
            verdict.append("returned verbatim")
        elif "ret" in infl:
            verdict.append("influences return")
        if any(h["as"] in ("read_address", "write_address") for h in flow):
            verdict.append("used as a pointer (flows to a memory address)")
        if any(h["as"] == "written_value" for h in flow):
            verdict.append("stored to memory")
        if "path_diverges_at" in infl:
            verdict.append("influences control flow")
        if not verdict and (infl.get("new_read_addrs") or infl.get("new_write_addrs")):
            verdict.append("changes which memory is touched")
        out_args.append({
            "index": i, "tag": tag,
            "influence": infl, "dataflow": flow,
            "verdict": " + ".join(verdict) if verdict else "no observed effect",
        })
    report = {
        "schema": "cb.behavior.experiment/1",
        "module": baseline["module"], "func_va": baseline["func_va"],
        "func_off": baseline["func_off"], "arity": n, "base_args": base,
        "baseline": {"exit": baseline["exit"], "ret": baseline["ret"],
                     "insn_count": baseline["insn_count"]},
        "args": out_args,
    }
    return baseline, report
