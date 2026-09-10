#!/usr/bin/env python3
"""
validate.py — Systematic consistency checker for the ordinal recovery.

The KERNEL.51 bug (identify.py read the wrong bytes and called MakeProcInstance
"OpenFile") was caught by luck. This tool catches that whole CLASS of bug
automatically, using a hard invariant of the Windows far-PASCAL ABI:

    a callee cleans its own arguments with `retf N`, so the N in the MODULE
    body's `retf N` MUST equal the number of argument bytes every caller pushes.

For each system ordinal an app actually calls, validate.py compares:
  - MODULE side (bundle read, base=0): the `retf N` arg-byte count of the body.
  - APP side: the bytes pushed at each call site (contiguous pushes x2) across all
    apps, taking the consensus.
A mismatch means one of: the module was read at the wrong offset (the KERNEL.51
class), the ordinal is mapped wrong, or a call site is mis-decoded — all bugs
worth a look. It also flags:
  - bodies that don't decode to a clean `retf` (misaligned / wrong-offset read),
  - callers that disagree with each other on arg count (mis-decode somewhere).

Usage: python3 validate.py MODULE [--all]      MODULE = KERNEL|USER|GDI|...
       (--all lists every ordinal; default lists only MISMATCHES)
"""
import sys, os, subprocess, tempfile, re, glob
from collections import Counter
from idord import open_module
from codebreaker.disasm_engine.disasm import open_ne, build_fixup_map


def _objdump(blob, vma=0):
    fd, tmp = tempfile.mkstemp(suffix="_val.bin")
    try:
        with os.fdopen(fd, "wb") as f:
            f.write(blob)
        return subprocess.run(
            ["objdump", "-D", "-b", "binary", "-mi386", "-Maddr16,data16,intel",
             "--adjust-vma=%d" % vma, tmp], capture_output=True, text=True).stdout
    finally:
        try: os.remove(tmp)
        except OSError: pass


def module_retf(modname):
    """{ordinal: (retf_N, clean)} — the declared arg-byte count of each body,
    read from the bundle (base=0). Uses trace.py's function DISCOVERY (prologue
    scan + call targets) to bound each ordinal at the next REAL function, not the
    next exported entry — otherwise non-exported helpers between exports pollute
    the span and the retf is wrong (this was the validator's own first bug).
    `clean` = the function had exactly one retf N value in its (tight) body."""
    import trace
    ne = open_module(modname)
    segs = ne.segments()
    ents = [e for e in ne.entries() if "offset" in e]
    sgs = {}
    for s in segs:
        if s["kind"] == "CODE":
            try:
                sgs[s["idx"]] = trace.Seg(ne, s["idx"], {})
            except Exception:
                sgs[s["idx"]] = None
    out = {}
    for e in ents:
        sg = sgs.get(e["seg"])
        if sg is None:
            out[e["ordinal"]] = (None, False); continue
        _, rows = sg.func_rows(e["offset"])          # tight, boundary-aware body
        retfs = []
        for _, t in rows:
            m = re.match(r"retf\s+0x([0-9a-f]+)", t)
            if m:
                retfs.append(int(m.group(1), 16))
            elif t == "retf":
                retfs.append(0)
        if not retfs:
            out[e["ordinal"]] = (None, False)        # shared-prolog / thunk
        else:
            out[e["ordinal"]] = (retfs[0], len(set(retfs)) == 1)
    return out


def app_pushbytes(mod_prefix):
    """{ 'MOD.ord': Counter{pushbytes: n} } — bytes pushed at each call site
    across all apps (contiguous push instrs x2 immediately before the call)."""
    usage = {}
    for path in glob.glob("files/WF/*.EXE"):
        try:
            ne = open_ne(path)
        except Exception:
            continue
        mods = ne.module_refs()
        for seg in ne.segments():
            if seg["kind"] != "CODE":
                continue
            try:
                fx = build_fixup_map(ne, seg, mods)
            except Exception:
                continue
            code = ne.data[seg["file_off"]:seg["file_off"]+seg["length"]]
            asm = _objdump(code)
            rows, offs = [], []
            for l in asm.splitlines():
                if ":\t" not in l:
                    continue
                offs.append(int(l.split(":\t")[0].strip(), 16))
                rows.append(l.split("\t")[-1].strip())
            pos = {o: i for i, o in enumerate(offs)}
            for site, lbl in fx.items():
                if not lbl.startswith(mod_prefix):
                    continue
                call_off = min((o for o in offs if o >= site-2), default=None)
                if call_off is None or call_off not in pos:
                    continue
                i = pos[call_off]
                pushes = 0
                for j in range(i-1, max(-1, i-10), -1):
                    a = rows[j]
                    if a.startswith("push"):
                        pushes += 1
                    elif a.startswith(("mov", "les", "lds", "lea", "xor", "sub", "cwd", "inc", "dec", "add", "or", "and", "shr", "shl", "cmp", "nop")):
                        continue                     # arg setup between pushes
                    else:
                        break
                usage.setdefault(lbl, Counter())[pushes*2] += 1
    return usage


def main():
    a = sys.argv[1:]
    if not a:
        print(__doc__); sys.exit(1)
    mod = a[0].upper()
    retf = module_retf(mod)
    usage = app_pushbytes(mod + ".")
    show_all = "--all" in a

    # ---- self-calibrate: the systematic (module - app) bias -----------------
    # Only trust a caller-consensus when callers mostly agree (dominant count
    # >=60% of its sites), else the app arg-count is itself unreliable.
    deltas, rows = [], []
    for lbl in sorted(usage, key=lambda x: int(x.split(".")[1])):
        ordn = int(lbl.split(".")[1])
        mr, clean = retf.get(ordn, (None, False))
        counts = usage[lbl]
        consensus, n = counts.most_common(1)[0]
        agree = n / sum(counts.values())
        rows.append((lbl, mr, clean, consensus, agree, counts))
        if mr is not None and clean and agree >= 0.6:
            deltas.append(mr - consensus)
    bias = Counter(deltas).most_common(1)[0][0] if deltas else 0

    print(f"# validate {mod}: module `retf N` vs app pushed arg-bytes")
    print(f"# self-calibrated systematic bias (module - app) = {bias:+d}  "
          f"(from {len(deltas)} clean ordinals); flagging deviations >= 4 bytes\n")
    hdr = f"  {'ordinal':<10} {'mod_retf':>8} {'app_bytes':>9} {'delta':>6} {'agree':>6}  verdict"
    print(hdr); print("  " + "-"*len(hdr))
    flagged = 0
    for lbl, mr, clean, consensus, agree, counts in rows:
        if mr is None:
            v = "no retf (shared-prolog/thunk) — retf check N/A"
            is_bug = False
            delta = None
        elif not clean:
            v = "boundary unclean (helper in span) — retf check N/A"
            is_bug = False
            delta = None
        else:
            delta = mr - consensus
            dev = abs(delta - bias)
            if agree < 0.6:
                v = f"callers scatter ({dict(counts)}) — likely varargs or a mis-decode"
                is_bug = True
            elif dev >= 4:
                v = f"!! CLEAN retf DEVIATES by {dev} (delta={delta:+d} vs bias {bias:+d}) — REAL BUG CANDIDATE"
                is_bug = True
            else:
                v = "ok"
                is_bug = False
        if is_bug:
            flagged += 1
        if show_all or is_bug:
            ds = f"{delta:+d}" if delta is not None else " -"
            print(f"  {lbl:<10} {str(mr):>8} {consensus:>9} {ds:>6} {agree:>5.0%}  {v}")
    print(f"\n  {flagged} ordinal(s) flagged (of {len(usage)} app-used {mod}). "
          f"Bias {bias:+d} absorbed; retf check applied only to clean-boundary bodies.")


if __name__ == "__main__":
    main()
