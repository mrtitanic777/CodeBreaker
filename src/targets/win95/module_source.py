#!/usr/bin/env python3
r"""module_source.py -- one source file per module, in the source tree, every function labelled.

The verifier's results used to end as tens of thousands of loose per-function files in staging
and a percentage. This writes what the project is actually for: the module's source, in the
Windows 95 tree, where each function says what is known about it.

    python -m src.targets.win95.module_source comctl32.dll            # verify, then write the file
    python -m src.targets.win95.module_source comctl32.dll --no-verify  # write from stored results
    python -m src.targets.win95.module_source --scoreboard            # rewrite SCOREBOARD.md only

Each function carries one status line:

    VERIFIED            behaves as the original on every input tried, with most of it exercised
    VERIFIED (weak)     matched, but the match is thin -- the reason is stated
    UNVERIFIED          rebuilt and run; it behaves differently (what differed is stated)
    NOT JUDGED          the original could not be observed in the test environment
    DOES NOT COMPILE    the reconstructed text is not valid C yet
    NO SOURCE           nothing usable was produced

Nothing is counted as progress except what these labels say. The scoreboard (SCOREBOARD.md at
the tree root) is generated from the same results and lists only modules measured on the current
verifier; older results are not carried forward.
"""
import os
import sys
import time
from collections import Counter

from src.core import corpus, toolchain

WHY = {
    "ret-differs": "returns a different value",
    "deps-differ": "makes different API calls, or passes them different arguments",
    "exit-differs": "ends differently (one side faults or runs away)",
    "writes-differ": "leaves different values in memory",
    "stack-differs": "removes a different number of bytes from the stack (convention or argument count)",
    "registers-differ": "changes a register it must preserve",
    "retail: call-into-nothing": "the original calls through a pointer the loader fills in (a thunk entry)",
}
WEAK = {
    "low-coverage": "under half of the original's instructions ran",
    "stub-callee": "a function it calls was a stand-in",
    "fault-only": "both sides only fault alike",
    "truncated": "a run was cut short",
    "argc-unknown": "an API call was compared by name only",
    "calls-capped": "the call sequence was too long to compare in full",
    "unobserved": "some inputs could not be run",
    "writes-capped": "too many memory writes to compare",
}
ORDER = ("verified", "weak", "unverified", "not-judged", "no-compile", "no-source")
TITLE = {"verified": "VERIFIED", "weak": "VERIFIED (weak)", "unverified": "UNVERIFIED",
         "not-judged": "NOT JUDGED", "no-compile": "DOES NOT COMPILE", "no-source": "NO SOURCE"}


def classify(row):
    """(class, detail) for one results row."""
    v = row.get("verdict")
    limits = [c for c in (row.get("limits") or "").split(",") if c]
    cov = next((int(c[9:]) for c in limits if c.startswith("coverage=")), None)
    weak = [WEAK[c] for c in limits if c in WEAK]
    if v == "equivalent":
        if weak:
            return "weak", "; ".join(weak) + (" (%d%% ran)" % cov if cov is not None else "")
        return "verified", ("%d%% of it exercised" % cov) if cov is not None else ""
    if v == "consistent":
        return "weak", "; ".join(weak or [WEAK["fault-only"]])
    if v == "divergent":
        return "unverified", WHY.get(row.get("reason") or "", row.get("reason") or "")
    if v == "inconclusive":
        return "not-judged", WHY.get(row.get("reason") or "", row.get("reason") or "")
    if v == "no-compile":
        return "no-compile", ""
    return "no-source", (row.get("reason") or "")[:80]


def tree_dir(module):
    """Where a module's source lives in the Windows 95 tree (the directory already holding it)."""
    root = os.path.dirname(corpus.design_dir())
    stem = os.path.splitext(os.path.basename(module))[0].lower()
    for top in sorted(os.listdir(root)):
        d = os.path.join(root, top, stem)
        if top not in ("design", "build", ".git") and os.path.isdir(d):
            return d
    return None


def write_module(module, verify=True, log=print):
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify as B, verify95
    from src.targets.win95.verification import pipeline, store
    path = corpus.retail(module)
    if not path:
        raise SystemExit("no retail module %r in the corpus" % module)
    name = os.path.basename(path).lower()
    out_dir = tree_dir(name)
    if not out_dir:
        raise SystemExit("%s has no directory in the source tree yet" % name)
    db = store.open()
    digest = pipeline.profile("pe")[0]
    funcs = pipeline.functions("pe", path)
    pe = open_pe(path)
    # Other decompilers, judged by the same verifier; each function is taken from whichever
    # reconstruction holds up best. In order of preference when two are equally good:
    #   core    the Ghidra decompiler core embedded in CodeBreaker, fed our facts (our names,
    #           proven argument counts; structured, real local variables)
    #   ghidra  Ghidra run headless, its C export adapted
    others = []                                   # [(engine name, results, preamble)]
    from src.targets.win95.verification import ghidra_engine
    for eng, label, flag in (("core", "core", "--no-core"), ("export", "ghidra", "--no-ghidra")):
        if flag in sys.argv:
            continue
        t0 = time.time()
        try:
            res, pre = ghidra_engine.verify(path, funcs, store=db, log=log, engine=eng)
        except Exception as e:                    # an engine that fails must not sink the module
            log("%s engine failed: %s: %s" % (label, type(e).__name__, str(e)[:120]))
            res, pre = {}, ""
        if res:
            others.append((label, res, pre))
            if verify:
                log("%s: %d functions judged in %.0fs" % (label, len(res), time.time() - t0))
    # Our own decompiler is asked only about the functions no other engine fully verified: one
    # verified reconstruction of a function is enough, and judging is the slow part.
    if verify:
        settled = set()
        for _label, res, _pre in others:
            settled |= {k for k, g in res.items() if g.get("source") and ORDER.index(classify(g)[0]) == 0}
        rest = [va for va in funcs if pipeline.LANES["pe"].key(va) not in settled]
        t0 = time.time()
        pipeline.verify("pe", path, rest, store=db, bank=False)
        log("codebreaker: %d of %d functions judged in %.0fs (%d already verified by another engine)" % (
            len(rest), len(funcs), time.time() - t0, len(funcs) - len(rest)))
    rows = db.get_many("pe", verify95._mod_sha(path) or "")
    parts, counts, engines = [], Counter(), Counter()
    for va in funcs:
        key = pipeline.LANES["pe"].key(va)
        row = rows.get(key) or {}
        if row.get("profile") != digest:
            row = {}                              # measured on an older verifier: not carried forward
        cls, detail = classify(row) if row else ("no-source", "not measured on the current verifier")
        try:
            fn, c = B.synth(path, va, 4, fast=True)
        except Exception:
            fn, c = None, None
        if not fn:
            blk = None
            try:
                blk = B._static_body(path, va)
            except Exception:
                pass
            fn, c = "sub_%06X" % (va - pe.image_base), blk
            if cls not in ("no-source",):
                cls, detail = "no-source", detail
        engine = "codebreaker"
        best_rank = ORDER.index(cls) + 0.5        # ours loses a tie to a structured reconstruction
        for label, res, _pre in others:
            g = res.get(key)
            if g and g.get("source"):
                gcls, gdetail = classify(g)
                if ORDER.index(gcls) < best_rank:
                    best_rank = ORDER.index(gcls)
                    cls, detail, fn, c, engine = gcls, gdetail, g.get("fname") or fn, g["source"], label
        counts[cls] += 1
        engines[(cls, engine)] += 1
        head = "/* [%s] %s  @ 0x%08X%s  (decompiler: %s) */" % (
            TITLE[cls], fn, va, (" -- " + detail) if detail else "", engine)
        parts.append(head + "\n" + ((c or "/* (no reconstruction) */").strip()) + "\n")
    total = len(funcs)
    banner = [
        "/* %s -- reconstructed source." % name, " *",
        " * STATUS, generated %s by CodeBreaker (verifier settings %s, decompiler %s)." % (
            time.strftime("%Y-%m-%d"), digest, toolchain.decompiler_build_id()),
        " * %d functions:" % total]
    for cls in ORDER:
        banner.append(" *   %-18s %5d  %5.1f%%" % (TITLE[cls], counts[cls], 100.0 * counts[cls] / max(1, total)))
    banner.append(" * Verified functions by decompiler: embedded core %d, Ghidra export %d, our own %d." % (
        engines[("verified", "core")], engines[("verified", "ghidra")], engines[("verified", "codebreaker")]))
    banner += [
        " *", " * Builds as a whole module: NO. Functions are compiled and checked one at a time; the",
        " * module's data (globals, tables, resources) is still referred to by its address in the",
        " * original and has not been reconstructed.",
        " * Runs in place of the original: NOT TESTED.", " *",
        " * VERIFIED = rebuilt from this text with OpenWatcom and run beside the original on an x86",
        " * emulator: same return value, same API calls with the same arguments, same memory written,",
        " * on every input tried. It is evidence, not proof. Names of internal functions (sub_...)",
        " * and locals (vN) are placeholders; no original names are claimed.", " */", ""]
    out = os.path.join(out_dir, os.path.splitext(name)[0] + ".c")
    stem = os.path.splitext(name)[0]
    for label, _res, pre in others:
        if not pre:
            continue
        hname = "%s.h" % stem if label == "core" else "%s.%s.h" % (stem, label)
        with open(os.path.join(out_dir, hname), "w", encoding="utf-8", newline="\n") as f:
            f.write("/* %s -- types, declarations and globals used by the functions marked\n"
                    " * (decompiler: %s) in %s.c. Globals are bound to their addresses in the original\n"
                    " * until the module's data is reconstructed. */\n\n" % (name, label, stem) + pre)
        banner.append("/* functions marked (decompiler: %s) use %s */" % (label, hname))
    banner.append("")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(banner) + "\n" + "\n".join(parts))
    log("%s: %s" % (out, ", ".join("%s %d" % (TITLE[c], counts[c]) for c in ORDER if counts[c])))
    return out, counts, total


def scoreboard(log=print):
    """Rewrite SCOREBOARD.md from the results measured on the CURRENT verifier settings."""
    from src.targets.win95.verification import pipeline, store
    db = store.open()
    digest = pipeline.profile("pe")[0]
    # one class per function: the better of the two engines' results
    best = {}
    q = ("SELECT module, key, verdict, reason, limits FROM body "
         "WHERE lane IN ('pe', 'pe-ghidra', 'pe-core') AND profile=?")
    for module, key, verdict, reason, limits in db.db.execute(q, (digest,)):
        cls = classify({"verdict": verdict, "reason": reason, "limits": limits})[0]
        old = best.get((module, key))
        if old is None or ORDER.index(cls) < ORDER.index(old):
            best[(module, key)] = cls
    per = {}
    for (module, _key), cls in best.items():
        per.setdefault(module, Counter())[cls] += 1
    root = os.path.dirname(corpus.design_dir())
    lines = ["# Windows 95 reconstruction -- scoreboard", "",
             "Generated %s by `python -m src.targets.win95.module_source --scoreboard` "
             "(verifier settings `%s`)." % (time.strftime("%Y-%m-%d %H:%M"), digest), "",
             "Only modules measured on the current verifier are listed. A function is VERIFIED when "
             "its reconstructed C, rebuilt and run beside the original, returns the same value, makes "
             "the same API calls with the same arguments and writes the same memory on every input "
             "tried, with at least half of it exercised. Everything else is not progress.", "",
             "| Module | Functions | Verified | Verified (weak) | Unverified | Not judged | Does not compile | No source | Builds whole | Runs in place |",
             "|---|---:|---:|---:|---:|---:|---:|---:|---|---|"]
    tot = Counter()
    for module in sorted(per):
        c = per[module]
        n = sum(c.values())
        tot.update(c)
        lines.append("| %s | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | no | not tested |" % (
            module, n, c["verified"], 100.0 * c["verified"] / max(1, n), c["weak"], c["unverified"],
            c["not-judged"], c["no-compile"], c["no-source"]))
    n = sum(tot.values())
    if n:
        lines.append("| **all measured** | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | | |" % (
            n, tot["verified"], 100.0 * tot["verified"] / n, tot["weak"], tot["unverified"],
            tot["not-judged"], tot["no-compile"], tot["no-source"]))
    measured = len(per)
    lines += ["", "%d of %d 32-bit modules measured so far. The 16-bit modules and the VxD drivers are "
              "tracked separately (VXD_FROM_SOURCE.md)." % (measured, len(corpus.modules("pe"))), ""]
    out = os.path.join(root, "SCOREBOARD.md")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    log("wrote %s (%d modules)" % (out, measured))
    return out


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from src.targets.win95.verification import pipeline
    pipeline._arm()
    mods = [a for a in argv if not a.startswith("--")]
    for m in mods:
        write_module(m, verify="--no-verify" not in argv)
    scoreboard()
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
