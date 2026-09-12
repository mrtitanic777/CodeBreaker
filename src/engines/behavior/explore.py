#!/usr/bin/env python3
r"""explore.py -- active experimentation: let the engine CHOOSE the next experiment.

Slice 2 varies inputs the user picks; slice 3 accumulates hypotheses. This closes the loop
(spec Section 8/9): the engine looks at its own uncertainty, picks the experiment most likely
to resolve it, runs it, and updates the hypotheses -- then repeats.

The uncertainty it targets is concrete and honest. An argument that shows "no effect" under an
all-zero baseline may really be used behind a guard (`if (arg != 0) ...`) that a single
baseline never trips. So the loop:

  1. SEEDS the hypothesis store with one tag sweep (slice 2).
  2. FINDS UNCERTAINTY: the argument we know least about -- lowest confidence among its
     *active* (non-"unused") claims. An arg explained only as "unused" is maximally uncertain:
     maybe it's genuinely unused, maybe it's guarded. That's the one worth an experiment.
  3. CHOOSES AN EXPERIMENT: a small, PURPOSEFUL value ladder, each value aimed at a specific
     latent role -- a zero-guard, a sign/compare, a POINTER into mapped memory (so a real
     dereference reads rather than faults), a large bound. Not random fuzzing.
  4. RUNS + UPDATES: each probe that produces new behavior is evidence for an active role and
     against "unused"; a ladder that produces nothing is strong evidence the arg really is
     unused (we tried hard to make it matter and failed -- that raises "unused" honestly).
  5. REPEATS until every argument is resolved or the budget runs out.

All probe traces are recorded raw; the session report and the updated hypotheses are derived.
"""
from src.engines.behavior import tracer, perturb, hypothesis, corpus

# scratch region tracer maps is 0x200000..0x210000; a pointer probe must land INSIDE it so a
# dereference reads real (zeroed) mapped memory instead of faulting.
_PTR_PROBE = 0x204000

# Each probe value is chosen to reveal a specific latent role, with the rationale recorded.
_LADDER = [
    (0x00000001, "truthy / zero-guard (if arg != 0)"),
    (0xFFFFFFFF, "all-ones / sign or unsigned-max compare"),
    (_PTR_PROBE, "pointer into mapped memory (reveal a dereference)"),
    (0x00010000, "large value / range or bound check"),
]


def _active_claims(store, i):
    """Confidence of the best ACTIVE (role-asserting, non-'unused') claim for arg i, and list."""
    best, claims = 0.0, []
    for h in store["hypotheses"]:
        s = h["subject"]
        if s.get("kind") == "arg" and s.get("index") == i and h["claim"] != "arg_unused":
            claims.append(h)
            best = max(best, h["confidence"])
    return best, claims


def _most_uncertain(store, arity, resolved):
    """The arg we know least about and haven't resolved this session -> lowest active confidence."""
    cands = [i for i in range(arity) if i not in resolved]
    if not cands:
        return None
    return min(cands, key=lambda i: _active_claims(store, i)[0])


def _baseline_summary(bt):
    return {"exit": bt["exit"], "ret": bt["ret"], "insn_count": bt["insn_count"],
            "calls": len(bt.get("calls", [])), "mem_reads": len(bt.get("mem_reads", [])),
            "mem_writes": len(bt.get("mem_writes", []))}


def _ingest_probe(baseline, entry, variant_vec, baseline_summary, module, func_va, func_off, arity):
    """Fold one probe (a single-arg experiment) into the hypothesis store as new evidence."""
    report = {
        "schema": "cb.behavior.experiment/1", "module": module, "func_va": func_va,
        "func_off": func_off, "arity": arity, "base_args": variant_vec,
        "baseline": baseline_summary, "args": [entry],
    }
    corpus.record_derived("experiments", report)            # keep the probe as a derived experiment
    return hypothesis.ingest_experiment(report)


def discriminate_arg(module, func, base_args, i, baseline, baseline_summary,
                     func_va, func_off, arity, insn_cap=4000):
    """Run the value ladder on arg i. Returns a round record and ingests each probe as evidence."""
    probes = []
    any_effect = False
    for value, rationale in _LADDER:
        if value == base_args[i]:
            continue                                        # equals baseline -> no contrast
        variant = list(base_args)
        variant[i] = value
        vt = tracer.trace_function(module, func=func, args=variant, insn_cap=insn_cap)
        corpus.record(vt)                                   # raw evidence
        entry = perturb.arg_entry(baseline, vt, i, value)
        effect = entry["verdict"] != "no observed effect"
        any_effect = any_effect or effect
        _ingest_probe(baseline, entry, variant, baseline_summary, module, func_va, func_off, arity)
        probes.append({"value": value, "rationale": rationale,
                       "verdict": entry["verdict"], "effect": effect})
    return {"arg": i, "probes": probes, "discovered_effect": any_effect}


def explore(module, func=None, base_args=None, arity=None, budget=None, insn_cap=4000):
    """The active loop. Returns a session report (also persisted under derived/explorations)."""
    # 1. seed with a tag sweep so every arg has a starting hypothesis
    baseline, seed = perturb.run_experiment(module, func=func, base_args=base_args,
                                            arity=arity, insn_cap=insn_cap)
    corpus.record(baseline)
    corpus.record_derived("experiments", seed)
    hypothesis.ingest_experiment(seed)

    n = baseline["input"]["arity"]
    base = list(baseline["input"]["args"])
    func_va, func_off = baseline["func_va"], baseline["func_off"]
    bsum = _baseline_summary(baseline)
    if budget is None:
        budget = n                                          # at most one targeted round per arg

    rounds, resolved = [], set()
    for _ in range(budget):
        store = hypothesis.load(module, func_va)
        i = _most_uncertain(store, n, resolved)
        if i is None:
            break
        before, _ = _active_claims(store, i)
        rd = discriminate_arg(module, func, base, i, baseline, bsum,
                              func_va, func_off, n, insn_cap)
        after_store = hypothesis.load(module, func_va)
        after, _ = _active_claims(after_store, i)
        rd["active_confidence"] = {"before": before, "after": after}
        rounds.append(rd)
        resolved.add(i)                                     # we gave this arg its ladder

    session = {
        "schema": "cb.behavior.exploration/1",
        "module": baseline["module"], "func_va": func_va, "func_off": func_off,
        "arity": n, "base_args": base, "rounds": rounds,
    }
    corpus.record_derived("explorations", session)
    return session


# --------------------------------------------------------------------------
# `wt behavior explore` -- the command.
# --------------------------------------------------------------------------
import sys  # noqa: E402


from src.cli.common import opt as _opt


def main():
    r"""wt behavior explore <module> [func] [--args a,b,c] [--arity N] [--budget R] [--insns N]

    Actively experiments on a function: seeds hypotheses, then repeatedly picks the
    least-understood argument and probes it with a purposeful value ladder, updating
    confidence, until every argument is resolved or the round budget runs out.
    """
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(main.__doc__); return
    module = argv[0]
    flags = {"--args", "--arity", "--budget", "--insns"}
    func, i = None, 1
    while i < len(argv):
        a = argv[i]
        if a in flags:
            i += 2; continue
        if a.startswith("-"):
            i += 1; continue
        func = a; break
    base_args = None
    if "--args" in argv:
        base_args = [int(x, 0) for x in _opt(argv, "--args", "").split(",") if x.strip()]
    arity = _opt(argv, "--arity"); arity = int(arity) if arity is not None else None
    budget = _opt(argv, "--budget"); budget = int(budget) if budget is not None else None
    insns = int(_opt(argv, "--insns", 4000))

    session = explore(module, func=func, base_args=base_args, arity=arity,
                      budget=budget, insn_cap=insns)

    print("active exploration: %s  func VA 0x%x  (arity %d, %d rounds)" %
          (session["module"], session["func_va"], session["arity"], len(session["rounds"])))
    print("  baseline args : %s\n" % (", ".join("0x%x" % a for a in session["base_args"]) or "(none)"))
    for rd in session["rounds"]:
        cf = rd["active_confidence"]
        head = "resolved (no latent role found)" if not rd["discovered_effect"] \
            else "DISCOVERED a latent role"
        print("  round -> arg%d : %s  (active conf %.3f -> %.3f)" %
              (rd["arg"], head, cf["before"], cf["after"]))
        for p in rd["probes"]:
            mark = "*" if p["effect"] else " "
            print("     %s probe 0x%-8x [%s] -> %s" %
                  (mark, p["value"], p["rationale"], p["verdict"]))
    print("\n  -> wt behavior hyp %s 0x%x   to see the updated model" %
          (session["module"], session["func_va"]))


if __name__ == "__main__":
    main()
