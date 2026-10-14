r"""module_writer.py -- one source file per module, every function labelled with what is known.

    python -m codebreaker module comctl32.dll               reconstruct, judge, then write the file
    python -m codebreaker module comctl32.dll --no-verify   write from what the store already holds
    python -m codebreaker module comctl32.dll --out DIR     write there instead of the source tree
    python -m codebreaker module comctl32.dll --build      also build the whole module and report it
    python -m codebreaker module comctl32.dll --split 200   at most 200 functions to a file
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

A driver or a real-mode program is written as its assembly file (source/driver_writer.py,
source/dos_writer.py) with the same status counts at its head. Where a module's files go in the
source tree is source/placement.py's business.

Nothing is counted as progress except what these labels say. Everything written here is read
from the function store, so the file and the scoreboard cannot disagree with the results.
"""
import os
import re
import time

from codebreaker.base import corpus, settings
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


def real_import_names(path, used=()):
    """{symbol: name} for a 16-bit module's imports by number that can be written by their real
    name: the name is a C name, no function of the module has it (`used`), and no two imports
    share it. (A 32-bit module's are already named in the decompiled text.)"""
    from codebreaker.verify import reconstruct
    if reconstruct.kind_name(path) == "powerpc":
        return console_import_names(path, used)
    if corpus.lane(path) != "ne":
        return {}
    from codebreaker.formats.ne import NEFile
    found = description.import_names16(NEFile(path))
    times = {}
    for name in found.values():
        times[name] = times.get(name, 0) + 1
    return {s: n for s, n in found.items() if times[n] == 1 and n not in used and n not in found}


def console_import_names(path, used=()):
    """{symbol: name} for a PowerPC image's imports. An image names what it imports by library
    and number only (`import0_ord_16`); the numbers of the console's system libraries are public
    knowledge (symbols/console_ordinals.json). The image as extracted does not carry the
    libraries' names: its first is taken to be the kernel and its second the system library,
    which is the order every image seen has them in -- a call's arguments bear it out
    (number 16 of the kernel takes a category, a setting, a buffer, its size and a length)."""
    import json
    from codebreaker.formats import powerpc_image
    with open(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "symbols",
                           "console_ordinals.json"), encoding="utf-8") as f:
        table = json.load(f)
    libraries = {0: table["xboxkrnl.exe"], 1: table["xam.xex"]}
    found = {}
    for symbol in set(powerpc_image.read(path).imports.values()):
        m = re.fullmatch(r"import(\d+)_ord_(\d+)", symbol)
        name = libraries.get(int(m.group(1)), {}).get(m.group(2)) if m else None
        if name and re.fullmatch(r"[A-Za-z_]\w*", name):
            found[symbol] = name
    times = {}
    for name in found.values():
        times[name] = times.get(name, 0) + 1
    return {s: n for s, n in found.items() if times[n] == 1 and n not in used}


def _globals_note(path, header):
    """A comment block describing the module's global variables, from the globals analysis
    (codebreaker.analyze.globals): for each global the functions refer to by its address, the
    type recovered from how it is used, its initial value, and how many functions read and write
    it. Nothing is renamed -- the globals keep the macros the decompiler bound to their original
    addresses -- so this only adds information. '' when there is nothing to say."""
    import re as _re
    addrs = {int(a, 16) for a in _re.findall(r"\b[a-z]+Ram([0-9a-f]{8})\b", header)}
    if not addrs:
        return ""
    try:
        from codebreaker.analyze import globals as _globals, image as _image
        var = _globals.variables(_image.load(path))
    except Exception:
        return ""
    lines = []
    for a in sorted(addrs):
        g = var.get(a)
        if not g:
            continue
        init = g["initial"] if isinstance(g["initial"], str) else ("%#x" % g["initial"] if g["initial"] is not None else "?")
        lines.append(" *   %08x  %-34s read by %d, written by %d%s" % (
            a, g["type"], len(g["readers"]), len(g["writers"]),
            ("  initially " + str(init)[:24]) if init not in ("", "uninitialised", "?") else
            "  uninitialised" if init == "uninitialised" else ""))
    return "\n/* globals (type recovered from use; still at the original's address):\n%s */\n" % "\n".join(lines) \
        if lines else ""


def write_assembly(path, fs, sha, digest, out_dir, log=print):
    """A driver's or a real-mode program's source: its one assembly file, the status at its head."""
    from codebreaker.verify import reconstruct
    name = os.path.basename(path).lower()
    stem = os.path.splitext(name)[0]
    counts = fs.scores(digest, sha)
    total = sum(counts.values())
    text = fs.get_text(sha, reconstruct.SOURCE, "header") or ""
    best = fs.best(sha, digest)
    how = next((r.get("detail") for r in best.values() if r.get("detail")), "")
    banner = ["; %s -- reconstructed source." % name, ";",
              "; STATUS, generated %s by CodeBreaker (verifier settings %s)." % (time.strftime("%Y-%m-%d"), digest),
              "; %d entry points or functions:" % total]
    banner += [";   %-28s %5d  %5.1f%%" % (TITLE[c], counts[c], 100.0 * counts[c] / max(1, total)) for c in ORDER]
    if how:
        banner += [";", "; " + how[:160]]
    for va in fs.addresses(sha):
        row = best.get(va)
        if row and row["class"] not in ("verified",):
            banner.append(";   %-10s %-28s %s %s" % (va, row["name"] or "", TITLE[row["class"]], describe(row)))
    out = os.path.join(out_dir, stem + ".asm")
    with open(out, "w", encoding="latin1", errors="replace", newline="\r\n") as f:
        f.write("\n".join(banner) + "\n;\n" + (text or "; (no source could be written)\n"))
    log("%s: %s" % (out, ", ".join("%s %d" % (TITLE[c], counts[c]) for c in ORDER if counts[c])))
    return out, counts, total


def in_corpus(path):
    """Is the file one of the selected product's originals?"""
    root = os.path.normcase(os.path.abspath(settings.get("corpus") or ""))
    return bool(root) and os.path.normcase(os.path.abspath(path)).startswith(root + os.sep)


def write_module(module, verify=True, out_dir=None, log=print, split=0, build_whole=False):
    from codebreaker.source import placement
    from codebreaker.verify import reconstruct
    path = module if os.path.isfile(module) else corpus.retail(module)
    if not path:
        raise SystemExit("no module %r in the corpus" % module)
    name = os.path.basename(path).lower()
    stem = os.path.splitext(name)[0]
    # A product's source tree is for that product's modules. A file from anywhere else (given
    # by path: another product's, a PowerPC image) is written under state/source/ here.
    out_dir = out_dir or (placement.folder(name, path) if in_corpus(path) else settings.state("source", stem))
    os.makedirs(out_dir, exist_ok=True)
    fs = F.open()
    if verify:
        sha, digest = reconstruct.run(path, fs=fs, log=log)
    else:
        sha, digest = reconstruct.identity(path)
    kind = reconstruct.kind_name(path)
    if kind in ("le", "dos"):
        return write_assembly(path, fs, sha, digest, out_dir, log)
    best = fs.best(sha, digest)
    addrs = fs.addresses(sha)             # every function the module is known to have
    known = fs.identified(sha)            # the runtime library's among them: {address: (name, library)}
    exported = {names.key(path, a): n for a, n in description.decorated_names(path).items()}
    real = real_import_names(path, {r["name"] for r in best.values()})
    # a recovered name for an internal function (a name it shares with a named function in
    # another module, a runtime-library name, or one from a debug symbol file) replaces the
    # decompiler's sub_... placeholder everywhere the module uses it
    recovered = {}
    try:
        for _address, k, nm, src in names.of_module(path, fs)["functions"]:
            if src != "(none)" and not nm.startswith("sub_"):
                recovered[k] = nm
    except Exception:
        pass
    rename, taken = {}, set(real.values()) | set(exported.values())
    for va in addrs:
        cur = (best.get(va) or {}).get("name") or ("sub_" + va)
        nm = recovered.get(va)
        if nm and cur.startswith("sub_") and nm not in taken and nm not in rename.values():
            rename[cur] = nm
            taken.add(nm)
    sub = dict(real, **rename)
    spell = re.compile(r"\b(%s)\b" % "|".join(sorted(sub, key=len, reverse=True))) if sub else None
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
        shown_name = rename.get(row["name"] or "sub_" + va, row["name"] or "sub_" + va)
        parts.append("/* [%s] %s  @ %s%s */\n%s\n" % (
            TITLE[shown], shown_name, va, (" -- " + "; ".join(said)) if said else "",
            spell.sub(lambda m: sub[m.group(1)], source) if spell else source))
    total = len(addrs)
    banner = ["/* %s -- reconstructed source." % name, " *",
              " * STATUS, generated %s by CodeBreaker (verifier settings %s)." % (time.strftime("%Y-%m-%d"), digest),
              " * %d functions:" % total]
    for cls in ORDER:
        banner.append(" *   %-28s %5d  %5.1f%%" % (TITLE[cls], counts[cls], 100.0 * counts[cls] / max(1, total)))
    whole = None
    if build_whole:
        from codebreaker.build import module as whole_module
        whole = whole_module.build(path, outdir=os.path.join(out_dir, "build"), call=False, log=lambda s: None)
    if whole and whole.linked:
        banner += [" *", " * Builds as a whole module: YES (%s). Every original export is present; a function the" % (
            "all the original's exports match" if whole.exports_same == whole.exports_total else
            "%d of %d exports match" % (whole.exports_same, whole.exports_total)),
            " * compiler rejected, or a call to code with no reconstruction, is a stand-in that returns 0",
            " * (%d rejected, %d absent). The built image is %s." % (
                len(whole.rejected), len(whole.absent), os.path.join(out_dir, "build", stem + os.path.splitext(name)[1])),
            " * Runs in place of the original: NOT TESTED as a whole.", " *"]
    else:
        banner += [
            " *", " * Builds as a whole module: " + ("NOT YET (%s)." % whole.error if whole else
                  "not built here (run `codebreaker build %s`)." % name),
            " * Functions are compiled and checked one at a time; the module's data (globals, tables,",
            " * resources) is referred to by its address in the original unless the whole module is built.",
            " * Runs in place of the original: NOT TESTED.", " *",
        " * VERIFIED = rebuilt from this text with %s and run beside the original in %s:" % (
            ("a PowerPC compiler", "a PowerPC interpreter") if kind == "powerpc" else ("OpenWatcom", "an x86 emulator")),
        " * same return value, same calls out of the module with the same arguments, same memory changed,",
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
        if spell:
            header = spell.sub(lambda m: sub[m.group(1)], header)
        if real:                                             # which module and number each import is
            header += "\n/* imports by number:\n%s */\n" % "".join(
                " *   %-28s %s.%s\n" % ((n,) + tuple(s.rsplit("_", 1))) for s, n in sorted(real.items(), key=lambda kv: kv[1]))
        header += _globals_note(path, header)
        with open(os.path.join(out_dir, stem + ".h"), "w", encoding="utf-8", newline="\n") as f:
            f.write("/* %s -- types, declarations and globals used by the functions in %s.c.\n"
                    " * Globals are bound to their addresses in the original until the module's data is\n"
                    " * reconstructed. */\n\n" % (name, stem) + header)
        banner.append('#include "%s.h"' % stem)
    banner.append("")
    out = os.path.join(out_dir, stem + ".c")
    pieces = [parts] if not split or len(parts) <= split else [parts[k:k + split] for k in range(0, len(parts), split)]
    for n, piece in enumerate(pieces):
        target = out if len(pieces) == 1 else os.path.join(out_dir, "%s_%d.c" % (stem, n + 1))
        head = banner if n == 0 else ["/* %s -- reconstructed source, part %d of %d (the status is in %s_1.c). */" % (
            name, n + 1, len(pieces), stem), ""] + [ln for ln in banner if ln.startswith("#include")] + [""]
        with open(target, "w", encoding="utf-8", newline="\n") as f:
            f.write("\n".join(head) + "\n" + "\n".join(piece))
    if len(pieces) > 1:
        out = os.path.join(out_dir, "%s_1.c" % stem)
    # the module's C++ classes, each virtual function tied to the function above that implements it
    from codebreaker.source import classes_writer

    def name_at(address):
        k = names.key(path, address)
        if k not in best and k not in known and kind == "pe":
            behind = classes_writer.import_behind(path, address)
            if behind:
                return behind
        cur = (best.get(k) or {}).get("name") or (known[k][0] if k in known else "sub_" + k)
        return sub.get(cur, cur)

    def status_at(address):
        row = best.get(names.key(path, address))
        return TITLE[row["class"]] if row else ""
    written = classes_writer.write(path, kind, out_dir, stem, name_at, status_at)
    if written:
        log("%s: the module's C++ classes" % written)
    log("%s: %s" % (out, ", ".join("%s %d" % (TITLE[c], counts[c]) for c in ORDER if counts[c])))
    return out, counts, total


def scoreboard(out_dir=None, log=print):
    """Rewrite SCOREBOARD.md from the store: modules judged under the CURRENT verifier settings
    of their kind (32-bit, 16-bit, PowerPC, driver)."""
    from codebreaker.verify import reconstruct
    fs = F.open()
    current = {d: k for k, d in reconstruct.digests().items() if d}
    digest = ", ".join("%s %s" % (k, d) for d, k in sorted(current.items(), key=lambda x: x[1]))
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
             "| Module | Kind | Functions | Verified | Verified (weak) | Identified (runtime library) | Unverified | Not judged | Does not compile | No source | Builds whole | Runs in place |",
             "|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|---|"]
    tot = dict.fromkeys(ORDER, 0)
    where = dict(fs.db.execute("SELECT sha, path FROM module").fetchall())
    mods = sorted((name, kind, sha, d) for d, kind in current.items() for sha, name in fs.modules(d)
                  if in_corpus(where.get(sha) or ""))          # this product's modules, no other's
    for name, kind, sha, d in mods:
        c = fs.scores(d, sha)
        n = sum(c.values())
        for k in ORDER:
            tot[k] += c[k]
        lines.append("| %s | %s | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | %d | not tested | not tested |" % (
            name, kind, n, c["verified"], 100.0 * c["verified"] / max(1, n), c["weak"], c["identified"],
            c["unverified"], c["not-judged"], c["no-compile"], c["no-source"]))
    n = sum(tot.values())
    if n:
        lines.append("| **all measured** | | %d | %d (%.0f%%) | %d | %d | %d | %d | %d | %d | | |" % (
            n, tot["verified"], 100.0 * tot["verified"] / n, tot["weak"], tot["identified"], tot["unverified"],
            tot["not-judged"], tot["no-compile"], tot["no-source"]))
    lines += ["", "%d modules measured so far (the corpus has %d 32-bit, %d 16-bit)." % (
        len(mods), len(corpus.modules("pe")), len(corpus.modules("ne"))), ""]
    out = os.path.join(out_dir or os.path.dirname(corpus.design_dir()), "SCOREBOARD.md")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    log("wrote %s (%d modules)" % (out, len(mods)))
    return out


def main(argv):
    out_dir = argv[argv.index("--out") + 1] if "--out" in argv else None
    split = int(argv[argv.index("--split") + 1]) if "--split" in argv else 0
    skip = {out_dir, str(split) if split else None}
    mods = [a for a in argv if not a.startswith("--") and a not in skip]
    if not mods and "--scoreboard" not in argv:
        print(__doc__)
        return 2
    for m in mods:
        write_module(m, verify="--no-verify" not in argv, out_dir=out_dir, split=split,
                     build_whole="--build" in argv)
    if "--scoreboard" in argv or any(not os.path.isfile(m) or in_corpus(m) for m in mods):
        scoreboard(out_dir)                      # (not for a file that is not the product's)
    return 0
