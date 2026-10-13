r"""module_writer.py -- one source file per module, every function labelled with what is known.

    python -m codebreaker module comctl32.dll               reconstruct, judge, then write the file
    python -m codebreaker module comctl32.dll --no-verify   write from what the store already holds
    python -m codebreaker module comctl32.dll --out DIR     write there instead of the source tree
    python -m codebreaker scoreboard                        rewrite the scoreboard only

Each function carries one status line:

    VERIFIED            behaves as the original on every input tried, with most of it exercised
    VERIFIED (weak)     matched, but the match is thin -- the reason is stated
    UNVERIFIED          rebuilt and run; it behaves differently (what differed is stated)
    NOT JUDGED          the original could not be observed in the test environment
    DOES NOT COMPILE    the reconstructed text is not valid C yet
    NO SOURCE           nothing usable was produced

Nothing is counted as progress except what these labels say. Everything written here is read
from the function store, so the file and the scoreboard cannot disagree with the results.
"""
import os
import time

from codebreaker.base import corpus
from codebreaker.store import functions as F

WHY = {
    "ret-differs": "returns a different value",
    "deps-differ": "makes different API calls, or passes them different arguments",
    "exit-differs": "ends differently (one side faults or runs away)",
    "writes-differ": "leaves different values in memory",
    "stack-differs": "removes a different number of bytes from the stack (convention or argument count)",
    "registers-differ": "changes a register it must preserve",
    "retail: call-into-nothing": "the original calls through a pointer the loader fills in (a thunk entry)",
}
ORDER, TITLE = F.ORDER, F.TITLE


def describe(row):
    """The words after the status for one function's best candidate."""
    limits = [c for c in (row.get("limits") or "").split(",") if c]
    cov = next((int(c[9:]) for c in limits if c.startswith("coverage=")), None)
    cls = row["class"]
    if cls == "verified":
        return ("%d%% of it exercised" % cov) if cov is not None else ""
    if cls == "weak":
        return "; ".join(F.WEAK[c] for c in row["weak"]) + (" (%d%% ran)" % cov if cov is not None else "")
    if cls in ("unverified", "not-judged"):
        return WHY.get(row.get("reason") or "", row.get("reason") or "")
    if cls == "no-compile":
        return (row.get("reason") or "")[:90]
    return (row.get("reason") or "")[:80]


def tree_dir(module):
    """Where a module's source lives in the target's source tree (the folder already holding it)."""
    root = os.path.dirname(corpus.design_dir())
    stem = os.path.splitext(os.path.basename(module))[0].lower()
    for top in sorted(os.listdir(root)):
        d = os.path.join(root, top, stem)
        if top not in ("design", "build", ".git") and os.path.isdir(d):
            return d
    return None


def write_module(module, verify=True, out_dir=None, log=print):
    from codebreaker.verify import reconstruct
    path = corpus.retail(module)
    if not path:
        raise SystemExit("no module %r in the corpus" % module)
    name = os.path.basename(path).lower()
    stem = os.path.splitext(name)[0]
    out_dir = out_dir or tree_dir(name)
    if not out_dir:
        raise SystemExit("%s has no folder in the source tree yet (use --out DIR)" % name)
    os.makedirs(out_dir, exist_ok=True)
    fs = F.open()
    if verify:
        sha, digest = reconstruct.run(path, fs=fs, log=log)
    else:
        sha, digest = reconstruct.identity(path)
    best = fs.best(sha, digest)
    addrs = fs.addresses(sha)             # every function the module is known to have
    counts = dict.fromkeys(ORDER, 0)
    parts = []
    for va in addrs:
        row = best.get(va)
        if not row:
            counts["no-source"] += 1
            parts.append("/* [%s] @ %s -- not measured on the current verifier */\n" % (TITLE["no-source"], va))
            continue
        cls = row["class"]
        counts[cls] += 1
        detail = describe(row)
        parts.append("/* [%s] %s  @ %s%s */\n%s\n" % (
            TITLE[cls], row["name"] or "sub_" + va, va, (" -- " + detail) if detail else "",
            (row["source"] or "/* (no reconstruction) */").strip()))
    total = len(addrs)
    banner = ["/* %s -- reconstructed source." % name, " *",
              " * STATUS, generated %s by CodeBreaker (verifier settings %s)." % (time.strftime("%Y-%m-%d"), digest),
              " * %d functions:" % total]
    for cls in ORDER:
        banner.append(" *   %-18s %5d  %5.1f%%" % (TITLE[cls], counts[cls], 100.0 * counts[cls] / max(1, total)))
    banner += [
        " *", " * Builds as a whole module: NO. Functions are compiled and checked one at a time; the",
        " * module's data (globals, tables, resources) is still referred to by its address in the",
        " * original and has not been reconstructed.",
        " * Runs in place of the original: NOT TESTED.", " *",
        " * VERIFIED = rebuilt from this text with OpenWatcom and run beside the original on an x86",
        " * emulator: same return value, same API calls with the same arguments, same memory changed,",
        " * same stack and saved registers, on every input tried. It is evidence, not proof. Names of",
        " * internal functions (sub_...) and locals are placeholders; no original names are claimed.", " */", ""]
    header = fs.get_text(sha, reconstruct.SOURCE, "header")
    if header:
        with open(os.path.join(out_dir, stem + ".h"), "w", encoding="utf-8", newline="\n") as f:
            f.write("/* %s -- types, declarations and globals used by the functions in %s.c.\n"
                    " * Globals are bound to their addresses in the original until the module's data is\n"
                    " * reconstructed. */\n\n" % (name, stem) + header)
        banner.append('#include "%s.h"' % stem)
    banner.append("")
    out = os.path.join(out_dir, stem + ".c")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(banner) + "\n" + "\n".join(parts))
    log("%s: %s" % (out, ", ".join("%s %d" % (TITLE[c], counts[c]) for c in ORDER if counts[c])))
    return out, counts, total


def scoreboard(out_dir=None, log=print):
    """Rewrite SCOREBOARD.md from the store: modules judged under the CURRENT verifier settings."""
    from codebreaker.verify import win32 as pipeline
    fs = F.open()
    digest = pipeline.settings_profile("pe")[0]
    lines = ["# Reconstruction scoreboard", "",
             "Generated %s by `python -m codebreaker scoreboard` (verifier settings `%s`)." % (
                 time.strftime("%Y-%m-%d %H:%M"), digest), "",
             "Only modules measured on the current verifier are listed. A function is VERIFIED when "
             "its reconstructed C, rebuilt and run beside the original, returns the same value, makes "
             "the same API calls with the same arguments, changes the same memory and leaves the stack "
             "and saved registers alike on every input tried, with at least half of it exercised. "
             "Everything else is not progress.", "",
             "| Module | Functions | Verified | Verified (weak) | Unverified | Not judged | Does not compile | No source | Builds whole | Runs in place |",
             "|---|---:|---:|---:|---:|---:|---:|---:|---|---|"]
    tot = dict.fromkeys(ORDER, 0)
    mods = fs.modules(digest)
    for sha, name in mods:
        c = fs.tally(digest, sha)
        n = sum(c.values())
        for k in ORDER:
            tot[k] += c[k]
        lines.append("| %s | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | no | not tested |" % (
            name, n, c["verified"], 100.0 * c["verified"] / max(1, n), c["weak"], c["unverified"],
            c["not-judged"], c["no-compile"], c["no-source"]))
    n = sum(tot.values())
    if n:
        lines.append("| **all measured** | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | | |" % (
            n, tot["verified"], 100.0 * tot["verified"] / n, tot["weak"], tot["unverified"],
            tot["not-judged"], tot["no-compile"], tot["no-source"]))
    lines += ["", "%d of %d 32-bit modules measured so far." % (len(mods), len(corpus.modules("pe"))), ""]
    out = os.path.join(out_dir or os.path.dirname(corpus.design_dir()), "SCOREBOARD.md")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    log("wrote %s (%d modules)" % (out, len(mods)))
    return out


def main(argv):
    out_dir = argv[argv.index("--out") + 1] if "--out" in argv else None
    skip = {out_dir}
    mods = [a for a in argv if not a.startswith("--") and a not in skip]
    if not mods and "--scoreboard" not in argv:
        print(__doc__)
        return 2
    for m in mods:
        write_module(m, verify="--no-verify" not in argv, out_dir=out_dir)
    scoreboard(out_dir)
    return 0
