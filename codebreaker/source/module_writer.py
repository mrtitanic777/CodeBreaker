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
    IDENTIFIED (runtime library)
                        its bytes are a function of a library that came with the compiler (the C
                        runtime, a compiler helper): named, with the library. Not this module's
                        own code, so not counted as a reconstruction that failed -- unless it was
                        verified too, and then it is counted as VERIFIED

Imports by number are written with their real names where the exporting module gives one
(`GetDlgItem`, not `USER_91`); a C++ export is shown with the declaration its decorated name
stands for.

Nothing is counted as progress except what these labels say. Everything written here is read
from the function store, so the file and the scoreboard cannot disagree with the results.
"""
import os
import re
import time

from codebreaker.base import corpus
from codebreaker.decompile import describe as description
from codebreaker.store import functions as F
from codebreaker.symbols import demangle, names

WHY = {
    "ret-differs": "returns a different value",
    "deps-differ": "makes different API calls, or passes them different arguments",
    "exit-differs": "ends differently (one side faults or runs away)",
    "writes-differ": "leaves different values in memory",
    "stack-differs": "removes a different number of bytes from the stack (convention or argument count)",
    "registers-differ": "changes a register it must preserve",
    "retail: call-into-nothing": "the original calls through a pointer the loader fills in (a thunk entry)",
}
ORDER, TITLE = F.SCORED, F.TITLE


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


def real_import_names(path, used=()):
    """{symbol: name} for a 16-bit module's imports by number that can be written by their real
    name: the name is a C name, no function of the module has it (`used`), and no two imports
    share it. (A 32-bit module's are already named in the decompiled text.)"""
    if corpus.lane(path) != "ne":
        return {}
    from codebreaker.formats.ne import NEFile
    found = description.import_names16(NEFile(path))
    times = {}
    for name in found.values():
        times[name] = times.get(name, 0) + 1
    return {s: n for s, n in found.items() if times[n] == 1 and n not in used and n not in found}


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
    known = fs.identified(sha)            # the runtime library's among them: {address: (name, library)}
    exported = {names.key(path, a): n for a, n in description.decorated_names(path).items()}
    real = real_import_names(path, {r["name"] for r in best.values()})
    spell = re.compile(r"\b(%s)\b" % "|".join(sorted(real, key=len, reverse=True))) if real else None
    counts = dict.fromkeys(ORDER, 0)
    parts = []
    for va in addrs:
        row = best.get(va)
        said = []
        if va in exported:
            said.append("exported as %s: %s" % (exported[va], demangle.readable(exported[va])))
        if not row:
            counts[F.IDENTIFIED if va in known else "no-source"] += 1
            parts.append("/* [%s] %s@ %s -- %s */\n" % (
                TITLE[F.IDENTIFIED if va in known else "no-source"], known[va][0] + "  " if va in known else "", va,
                "; ".join(([known[va][1]] if va in known else []) + said + ["not measured on the current verifier"])))
            continue
        cls = row["class"]
        detail = describe(row)
        shown = F.IDENTIFIED if va in known and cls not in ("verified", "weak") else cls
        counts[shown] += 1
        if shown == F.IDENTIFIED:
            said.insert(0, "%s of %s; reconstructed below: %s%s" % (
                known[va] + (TITLE[cls], (", " + detail) if detail else "")))
        else:
            said = ([detail] if detail else []) + said
            if va in known:
                said.append("this is %s of %s" % known[va])
        source = (row["source"] or "/* (no reconstruction) */").strip()
        parts.append("/* [%s] %s  @ %s%s */\n%s\n" % (
            TITLE[shown], row["name"] or "sub_" + va, va, (" -- " + "; ".join(said)) if said else "",
            spell.sub(lambda m: real[m.group(1)], source) if spell else source))
    total = len(addrs)
    banner = ["/* %s -- reconstructed source." % name, " *",
              " * STATUS, generated %s by CodeBreaker (verifier settings %s)." % (time.strftime("%Y-%m-%d"), digest),
              " * %d functions:" % total]
    for cls in ORDER:
        banner.append(" *   %-28s %5d  %5.1f%%" % (TITLE[cls], counts[cls], 100.0 * counts[cls] / max(1, total)))
    banner += [
        " *", " * Builds as a whole module: NO. Functions are compiled and checked one at a time; the",
        " * module's data (globals, tables, resources) is still referred to by its address in the",
        " * original and has not been reconstructed.",
        " * Runs in place of the original: NOT TESTED.", " *",
        " * VERIFIED = rebuilt from this text with OpenWatcom and run beside the original on an x86",
        " * emulator: same return value, same API calls with the same arguments, same memory changed,",
        " * same stack and saved registers, on every input tried. It is evidence, not proof. Names of",
        " * internal functions (sub_...) and locals are placeholders; no original names are claimed.",
        " * IDENTIFIED = the function's bytes are those of a function in a library that came with the",
        " * compiler; it is named after that function. It is reconstructed and judged all the same."]
    if real:
        banner += [" * Imports by number are written with their real names (%d of them; %s.h lists the" % (len(real), stem),
                   " * module and number of each). The text that was built and judged calls them MODULE_number."]
    banner += [" */", ""]
    header = fs.get_text(sha, reconstruct.SOURCE, "header")
    if header:
        if spell:                                            # which module and number each import is
            header = spell.sub(lambda m: real[m.group(1)], header) + "\n/* imports by number:\n%s */\n" % "".join(
                " *   %-28s %s.%s\n" % ((n,) + tuple(s.rsplit("_", 1))) for s, n in sorted(real.items(), key=lambda kv: kv[1]))
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
             "Everything else is not progress. A function IDENTIFIED as a runtime library's (the C "
             "runtime, a compiler helper) is not the module's own code and is counted apart, unless it "
             "was verified as well.", "",
             "| Module | Functions | Verified | Verified (weak) | Identified (runtime library) | Unverified | Not judged | Does not compile | No source | Builds whole | Runs in place |",
             "|---|---:|---:|---:|---:|---:|---:|---:|---:|---|---|"]
    tot = dict.fromkeys(ORDER, 0)
    mods = fs.modules(digest)
    for sha, name in mods:
        c = fs.scores(digest, sha)
        n = sum(c.values())
        for k in ORDER:
            tot[k] += c[k]
        lines.append("| %s | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | %d | no | not tested |" % (
            name, n, c["verified"], 100.0 * c["verified"] / max(1, n), c["weak"], c["identified"], c["unverified"],
            c["not-judged"], c["no-compile"], c["no-source"]))
    n = sum(tot.values())
    if n:
        lines.append("| **all measured** | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | %d | | |" % (
            n, tot["verified"], 100.0 * tot["verified"] / n, tot["weak"], tot["identified"], tot["unverified"],
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
