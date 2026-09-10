#!/usr/bin/env python3
r"""hypothesis.py -- the behavioral engine's hypothesis store (derived, cumulative).

An experiment reports what happened under one set of inputs. A HYPOTHESIS is a standing,
evidence-backed claim about what something MEANS -- "arg0 is a pointer written through",
"arg1 is unused", "this function is a leaf" -- carried across many experiments with a
confidence that evidence moves. Per the spec: competing readings coexist, uncertainty is
never prematurely collapsed, and every claim links back to the raw evidence that supports
or contradicts it, so it can be re-derived if our understanding changes.

Confidence is a Laplace/Beta estimate from pseudo-counts:

        confidence = (support + 1) / (support + contradict + 2)

A brand-new claim with one supporting observation sits at 2/3, not 1.0 -- honest about a
single data point -- and climbs toward 1 only as independent experiments keep confirming it
(or falls as they contradict it). It never reaches exactly 0 or 1, matching "do not collapse
uncertainty".

IDENTITY & IDEMPOTENCY. A hypothesis is keyed by (subject, claim); evidence is keyed by the
experiment's CONDITIONS (baseline args). Re-ingesting the same experiment changes nothing;
an experiment with a *different* baseline is genuinely new evidence and moves the count. So
the store only strengthens a claim when independent inputs agree.

Everything here is DERIVED and regenerable from raw traces + experiment reports.
"""
import json
import os

from codebreaker.behavior_engine import corpus


def confidence(support, contradict):
    return round((support + 1) / (support + contradict + 2), 3)


def _store_path(module, func_va):
    d = os.path.join(corpus.derived_dir(), "hypotheses", os.path.basename(module))
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, "0x%x.json" % int(func_va))


def load(module, func_va):
    fp = _store_path(module, func_va)
    if not os.path.exists(fp):
        return {"module": os.path.basename(module), "func_va": int(func_va), "hypotheses": []}
    with open(fp, encoding="utf-8") as fh:
        return json.load(fh)


def save(store):
    fp = _store_path(store["module"], store["func_va"])
    with open(fp, "w", encoding="utf-8") as fh:
        json.dump(store, fh, indent=1)
    return fp


def _find(store, subject, claim):
    for h in store["hypotheses"]:
        if h["subject"] == subject and h["claim"] == claim:
            return h
    return None


def _observe(store, subject, claim, statement, kind, cond, detail):
    """Record one evidence event (kind in {'support','contradict'}) for (subject, claim),
    idempotent on `cond` (the experiment conditions). Recompute confidence."""
    h = _find(store, subject, claim)
    if h is None:
        h = {"subject": subject, "claim": claim, "statement": statement,
             "support": 0, "contradict": 0, "confidence": 0.5, "evidence": []}
        store["hypotheses"].append(h)
    ref = "%s|%s" % (cond, kind)
    if any(e["ref"] == ref for e in h["evidence"]):
        return h                                             # already counted this condition
    h["evidence"].append({"ref": ref, "kind": kind, "detail": detail})
    h[kind if kind in ("support", "contradict") else "support"] += 1
    h["confidence"] = confidence(h["support"], h["contradict"])
    return h


# claim -> (statement template) for argument-level claims
def _arg_claims(arg):
    """Return [(claim, statement, detail)] observed-true for this arg in an experiment entry."""
    i = arg["index"]
    infl, flow = arg["influence"], arg["dataflow"]
    has_ret_flow = any(h["as"] == "return_value" for h in flow)
    has_ptr_flow = any(h["as"] in ("read_address", "write_address") for h in flow)
    has_store = any(h["as"] == "written_value" for h in flow)
    affects_ret = "ret" in infl
    affects_cf = "path_diverges_at" in infl
    touches_mem = bool(infl.get("new_read_addrs") or infl.get("new_write_addrs"))
    out = []
    if has_ret_flow:
        out.append(("arg_returned_verbatim", "arg%d is returned unchanged" % i, "tag == return value"))
    if affects_ret or has_ret_flow:
        out.append(("arg_affects_return", "arg%d affects the return value" % i,
                    "return changed when only arg%d changed" % i))
    if has_ptr_flow:
        pc = next(h["pc"] for h in flow if h["as"] in ("read_address", "write_address"))
        out.append(("arg_is_pointer", "arg%d is a pointer (dereferenced)" % i,
                    "tag used as a memory address at pc 0x%x" % pc))
    if has_store:
        out.append(("arg_stored_to_memory", "arg%d is stored to memory" % i, "tag written to memory"))
    if affects_cf:
        out.append(("arg_affects_control_flow", "arg%d affects control flow" % i,
                    "execution path diverged when only arg%d changed" % i))
    active = bool(out)
    if not active and not touches_mem:
        out.append(("arg_unused", "arg%d has no observed effect (may be unused)" % i,
                    "no change under this baseline"))
    return out, active


def ingest_experiment(report):
    """Fold an experiment report into the hypothesis store for its function. Returns the store."""
    # normalize to a bare module label: a caller may pass a path, and subjects keyed by path vs
    # basename would never dedupe (they'd split into duplicate hypotheses for the same function).
    module, func_va = os.path.basename(report["module"]), report["func_va"]
    store = load(module, func_va)
    cond = "exp:base=%s" % ",".join("0x%x" % a for a in report["base_args"])

    for arg in report["args"]:
        subject = {"kind": "arg", "module": module, "func_va": func_va, "index": arg["index"]}
        claims, active = _arg_claims(arg)
        for claim, stmt, detail in claims:
            _observe(store, subject, claim, stmt, "support", "%s:arg%d" % (cond, arg["index"]), detail)
        if active:
            # observing any real effect for this arg is evidence AGAINST "unused"
            usub = subject
            _observe(store, usub, "arg_unused",
                     "arg%d has no observed effect (may be unused)" % arg["index"],
                     "contradict", "%s:arg%d" % (cond, arg["index"]),
                     "an effect WAS observed for arg%d" % arg["index"])

    # function-level claim: leaf (made no calls under this baseline)
    fsub = {"kind": "function", "module": module, "func_va": func_va}
    b = report.get("baseline", {})
    if b.get("calls", 0) == 0:
        _observe(store, fsub, "function_is_leaf", "function makes no calls (leaf)",
                 "support", cond, "0 calls observed in baseline trace")
    else:
        _observe(store, fsub, "function_is_leaf", "function makes no calls (leaf)",
                 "contradict", cond, "%d calls observed in baseline trace" % b["calls"])

    save(store)
    return store
