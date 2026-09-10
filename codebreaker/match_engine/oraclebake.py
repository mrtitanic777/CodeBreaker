#!/usr/bin/env python3
r"""oraclebake.py — compiler ORACLE BAKE-OFF for the byte-exact-C project.

MSVC 4.2 reproduces win95's trivial-tier bytes but DIVERGES on real functions (operand
order / scheduling), because it isn't the exact compiler that built win95. We can't know
which compiler is closest by reasoning — only empirically. This compiles a fixed BATTERY
of functions (with known shipped bytes) through EVERY vendored MSVC version and reports
the per-version byte-match rate. The version that matches the most shipped bytes IS, by
definition, the closest proxy to win95's real compiler — and should become the oracle.

    wt --target win95 oraclebake            # bake-off over all vendored versions

Versions are auto-discovered under win95/docs/reference/toolchain/*/ (any dir with a
VC/BIN/CL.EXE or DEVSTUDIO/VC/BIN/CL.EXE). cc32 respects WT_VC_ROOT, so we just point it
at each in turn. The BATTERY mixes trivial baselines (match everywhere) with the
DISCRIMINATING real functions — 64-bit arithmetic helpers where 4.2 missed by operand
order — so the table shows exactly where the versions part ways.
"""
import glob
import os
import sys

from codebreaker.foundation import layout

# (label, shipped-bytes hex, candidate C). The arithmetic trio is the discriminator:
# same instructions, different operand-load order across compiler versions.
BATTERY = [
    ("identity (stdcall)", "8b442404c20400",
     "int __stdcall f(int a){return a;}"),
    ("i64 multiply 0x059a8", "8b442404f76c2408c20800",
     "__int64 __stdcall f(int a,int b){return (__int64)a*b;}"),
    ("i64 add 0x05994", "8b4424040344240c8b54240813542410c21000",
     "__int64 __stdcall f(__int64 a,__int64 b){return a+b;}"),
    ("i64 subtract 0x05a9c", "8b4424042b44240c8b5424081b542410c21000",
     "__int64 __stdcall f(__int64 a,__int64 b){return a-b;}"),
]


def _discover():
    """{name: vc_root} for every vendored MSVC toolchain under docs/reference/toolchain."""
    out = {}
    base = layout.reference_sub("toolchain")
    if not os.path.isdir(base):
        return out
    for d in sorted(os.listdir(base)):
        for sub in ("VC", os.path.join("DEVSTUDIO", "VC")):
            vc = os.path.join(base, d, sub)
            if os.path.exists(os.path.join(vc, "BIN", "CL.EXE")):
                out[d] = os.path.abspath(vc)
                break
    return out


def _cl_version(cc32, vc):
    try:
        import re
        import subprocess
        env, bindir = cc32._cl_env(vc)
        p = subprocess.run([os.path.join(bindir, "CL.EXE")], env=env,
                           capture_output=True, text=True, timeout=20)
        m = re.search(r"Version (\d+\.\d+(?:\.\d+)?)", p.stdout + p.stderr)
        if m:
            return m.group(1)
    except Exception:
        pass
    return "?"


def main():
    import cc32
    versions = _discover()
    if not versions:
        print("no vendored MSVC toolchains found under %s" % layout.reference_sub("toolchain"))
        return
    print("ORACLE BAKE-OFF  (%d vendored versions, %d battery functions)\n"
          % (len(versions), len(BATTERY)))

    results = {}      # name -> {version: 'MATCH'/'miss'/'err'}
    clver = {}
    for vname, vc in versions.items():
        os.environ["WT_VC_ROOT"] = vc
        clver[vname] = _cl_version(cc32, vc)
        for label, hexb, csrc in BATTERY:
            r = cc32.compile_c(csrc, flags="/c /O2")
            cell = "err"
            if "obj" in r:
                fns = cc32.Obj(r["obj"]).functions()
                got = bytes(list(fns.values())[0][0]) if fns else b""
                if hexb is None:
                    cell = "n/a"     # no shipped anchor for this baseline
                else:
                    cell = "MATCH" if got == bytes.fromhex(hexb) else "miss"
            results.setdefault(label, {})[vname] = cell
    os.environ.pop("WT_VC_ROOT", None)

    order = sorted(versions)
    print("  %-24s %s" % ("CL version", "  ".join("%-8s" % v for v in order)))
    print("  %-24s %s" % ("", "  ".join("%-8s" % clver[v].split()[-1][:8] if clver[v] != "?" else "%-8s" % "?" for v in order)))
    print("  " + "-" * (24 + 10 * len(order)))
    tally = {v: 0 for v in order}
    denom = 0
    for label, hexb, csrc in BATTERY:
        row = results.get(label, {})
        cells = []
        for v in order:
            c = row.get(v, "-")
            cells.append("%-8s" % c)
            if c == "MATCH":
                tally[v] += 1
        if hexb is not None:
            denom += 1
        print("  %-24s %s" % (label[:24], "  ".join(cells)))
    print("  " + "-" * (24 + 10 * len(order)))
    print("  %-24s %s" % ("MATCHED (of %d anchored)" % denom,
                          "  ".join("%-8s" % ("%d" % tally[v]) for v in order)))
    best = max(order, key=lambda v: tally[v])
    print("\n  => best proxy for win95 codegen: %s (%d/%d)  clver: %s"
          % (best, tally[best], denom, clver.get(best, "?")))

    # The codegen bake-off (above) picks the COMPILER; the LINKER version stamped in the
    # shipped PE headers independently corroborates it and pins the per-module link stage.
    # kernel32/gdi32/user32 -> LINK 2.50 (== MSVC 2.0's linker); the multimedia DLLs and
    # VxDs -> LINK 2.60 (the Win95 DDK's patched linker). Both are staged; see toolchain.py.
    try:
        from codebreaker.foundation import toolchain
        print("\n  FULL WIN95 BUILD TOOLCHAIN  (per PE linker-version fingerprint):")
        for role, kind, path, banner in toolchain.inventory():
            mark = "OK " if path and os.path.exists(path) else "-- "
            print("    %s %-9s %-24s %s" % (mark, role, kind, banner))
        print("    per-module linker: core=LINK 2.50, multimedia/VxD=LINK 2.60"
              "  (toolchain.linker_for_module)")
    except Exception as e:
        print("  (toolchain inventory unavailable: %s)" % str(e)[:60])


if __name__ == "__main__":
    main()
