#!/usr/bin/env python3
r"""dyndiscover.py -- DYNAMIC function discovery: recover the vtable/indirect-call targets that static
analysis cannot resolve, by observing what the code actually calls on the oracle.

The static decoder resolves direct calls, reloc/tail-jmp targets, and constant-address indirect calls,
but `call [reg+disp]` COM/vtable dispatch is a RUNTIME value (the object's vtable) -- statically
irreducible. The oracle tracer, however, RUNS the code and records the real target of every indirect
call (rec['calls'][].resolved_target). This pass traces a module's functions and collects every
observed intra-module call target that static discovery missed, writing them as extra function seeds:
    recon/authored/_dynseeds/<module>.txt   (one hex RVA/linear per line)
Feeding those to the decoder's discovery (a --seeds input) recovers the vtable-reached bodies.

Coverage is bounded by the oracle's inputs (synthetic args exercise a subset of dispatch paths); the
COMPLETE dynamic environment is the running system (boot-integration). This is the static-complement.

    wt --target win95 dyndiscover <module> [--max N]
"""
import os
import subprocess
import sys

from src.targets.win95 import authsrc, owlink


def _static_starts(binpath, fmt):
    dec = authsrc._native(fmt)
    out = subprocess.run([dec, binpath, "--names"], capture_output=True, text=True).stdout
    s = set()
    for line in out.splitlines():
        p = line.split("\t")
        if p and p[0].strip():
            try:
                s.add(int(p[0], 16))
            except ValueError:
                pass
    return s


def run(module, maxn=80):
    binpath = owlink._find_binary(module)
    if not binpath:
        print("no binary for %s" % module); return 1
    fmt = authsrc.classify(binpath)[0]
    static = _static_starts(binpath, fmt)
    # PE uses the behavior-engine tracer; NE the ne dynamic oracle.
    observed = set()
    traced = 0
    if fmt == "PE":
        from src.engines.behavior import tracer as T
        mod = os.path.basename(binpath)
        try:
            funcs = T.list_functions(mod)
        except Exception as e:                                        # noqa: BLE001
            print("tracer list_functions failed: %s" % e); funcs = []
        for f in funcs[:maxn]:
            try:
                rec = T.trace_function(mod, func=f, arity=8)
            except Exception:                                        # noqa: BLE001
                continue
            traced += 1
            for c in rec.get("calls", []):
                t = c.get("resolved_target") or c.get("target")
                if isinstance(t, int):
                    observed.add(t)
    else:
        from src.targets.win95 import ne_dynoracle as N
        # trace by export ordinal
        dec = authsrc._native(fmt)
        exps = subprocess.run([dec, binpath, "--exports"], capture_output=True, text=True).stdout
        ords = []
        for line in exps.splitlines():
            p = line.split("\t")
            if p and p[0].strip().isdigit():
                ords.append(int(p[0]))
        for o in ords[:maxn]:
            try:
                rec = N.probe(binpath, ordinal=o, args=[N.SAFE_ARG] * 16)
            except Exception:                                        # noqa: BLE001
                continue
            traced += 1
            for c in rec.get("calls", []):
                t = c.get("target") or c.get("resolved_target")
                if isinstance(t, int):
                    observed.add(t)
    new = sorted(t for t in observed if t and t not in static)
    outdir = os.path.join(authsrc._dr(), "recon", "authored", "_dynseeds")
    os.makedirs(outdir, exist_ok=True)
    outp = os.path.join(outdir, module + ".txt")
    with open(outp, "w") as fh:
        for t in new:
            fh.write("%X\n" % t)
    print("%s [%s]: traced %d fns, %d observed call targets, %d NEW (not statically discovered)" %
          (module, fmt, traced, len(observed), len(new)))
    print("  -> %s" % outp)
    return 0


def main(argv):
    pos = [a for a in argv if not a.startswith("--")]
    if not pos:
        print(__doc__); return 0
    maxn = int(argv[argv.index("--max") + 1]) if "--max" in argv else 80
    return run(pos[0], maxn)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
