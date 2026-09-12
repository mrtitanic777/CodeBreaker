#!/usr/bin/env python3
r"""experiment.py -- `wt behavior experiment`: change one input at a time, attribute effects.

    wt behavior experiment <module> [func] [options]

    <module>        a win95 binary, e.g. gdi32.dll
    [func]          VA or section offset (hex/decimal); omitted -> first function.
    --args a,b,c    baseline stdcall dword args (hex/decimal); default: zero-filled.
    --arity N       baseline arg count when --args is absent (default: recovered, else 4).
    --insns N       per-run instruction cap (default 4000; experiments run many variants).

For each argument, the baseline value is replaced with a distinctive tag and the function is
re-traced; the variant's trace is diffed against the baseline and the tag is tracked through
it. The result is a per-argument VERDICT (influences return / used as a pointer / influences
control flow / stored to memory / no observed effect), stored as a derived experiment report.
The raw baseline + variant traces are themselves recorded immutably in the corpus.

This is the seed of the hypothesis + active-experiment loop: today it attributes effects to
inputs; next it will propose competing meanings and pick the input change that separates them.
"""
import sys

from src.engines.behavior import perturb, corpus, hypothesis


from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    flags = {"--args", "--arity", "--insns"}
    func = None
    i = 1
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
    arity = _opt(argv, "--arity")
    arity = int(arity) if arity is not None else None
    insns = int(_opt(argv, "--insns", 4000))

    baseline, report = perturb.run_experiment(module, func=func, base_args=base_args,
                                              arity=arity, insn_cap=insns)
    # raw: the baseline trace is evidence too
    corpus.record(baseline)
    fp = corpus.record_derived("experiments", report)
    # fold the verdicts into the cumulative hypothesis store (this experiment is new evidence)
    hstore = hypothesis.ingest_experiment(report)

    b = report["baseline"]
    print("experiment: %s  func VA 0x%x  (off 0x%x)" %
          (report["module"], report["func_va"], report["func_off"]))
    print("  baseline args : %s" % (", ".join("0x%x" % a for a in report["base_args"]) or "(none)"))
    print("  baseline      : exit=%s  ret=%s  insns=%d" %
          (b["exit"], ("0x%x" % b["ret"]) if b["ret"] is not None else "(none)", b["insn_count"]))
    print("  arity         : %d\n" % report["arity"])
    for a in report["args"]:
        print("  arg%-2d (tag 0x%08x) : %s" % (a["index"], a["tag"], a["verdict"]))
        infl = a["influence"]
        if "ret" in infl:
            rv = infl["ret"]
            print("        return  : 0x%s -> 0x%s" %
                  (format(rv["base"] or 0, "x"), format(rv["var"] or 0, "x")))
        if "path_diverges_at" in infl:
            pd = infl["path_diverges_at"]
            print("        branch  : path diverges at insn #%d (0x%x vs 0x%x)" %
                  (pd["index"], pd["base_pc"], pd["var_pc"]))
        for h in a["dataflow"][:4]:
            if h["as"] in ("read_address", "write_address"):
                print("        dataflow: tag reached pc 0x%x as a %s (0x%x)" %
                      (h["pc"], h["as"], h["addr"]))
            elif h["as"] == "written_value":
                print("        dataflow: tag stored to memory at pc 0x%x" % h["pc"])
            elif h["as"] == "return_value":
                print("        dataflow: tag is the return value (returned verbatim)")
    print("\n  stored (derived): %s" % fp)
    print("  hypotheses now  : %d for this function  (wt behavior hyp %s 0x%x)" %
          (len(hstore["hypotheses"]), report["module"], report["func_va"]))


if __name__ == "__main__":
    main()
