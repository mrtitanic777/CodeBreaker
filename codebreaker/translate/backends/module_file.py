"""module_file.py -- L2 to the source file people read: a module's functions, each under its status, the status at the head.

    from codebreaker.translate.backends import module_file
    files = module_file.c(doc, facts, split=0)    # [(file name, text)]: <stem>.c, or <stem>_1.c ... when split
    text = module_file.asm(doc, facts)            # a driver's or a real-mode program's one assembly file

The back end source/module_writer.py writes through. The module is one L2 tree (docs/IR_UNIFICATION.md):
for C, a Module whose units are its functions in address order --

    MethodDecl      a function with a reconstruction: `text` its C, `comments` [its status line]
    Comment         a function with none: its status line alone

-- and for a driver or a real-mode program a Module of one MachineFragment, its whole source, which
the assembly back end (backends/asm.py) writes. What is said at the head is not the tree's but the
binary's: the per-binary facts the function store and the build give (`facts`):

    name, stem          the module's file name and its stem
    date, digest        when, and under which verifier settings
    order, titles       the status classes in the order they are counted, and each one's title
    counts, total       functions in each class, and all of them
    kind                the lane (pe, ne, powerpc, le, dos)
    whole               the whole module's build, or None: linked, error, exports_same, exports_total,
                        rejected, absent, built (the image's path)
    real                how many imports by number are written with their real names
    header              whether <stem>.h is written (the file includes it)
    how                 (assembly) what the judge said of the module as a whole
    rows                (assembly) [(address, name, title, what)] each function not verified
"""
from codebreaker.translate.backends import asm as _asm


def _counts(facts, lead):
    total = facts["total"]
    return [lead + "  %-28s %5d  %5.1f%%" % (facts["titles"][c], facts["counts"][c],
                                            100.0 * facts["counts"][c] / max(1, total)) for c in facts["order"]]


def banner(facts):
    """The head of the C file: the status, how the module builds, what the labels mean."""
    name, stem, total, kind = facts["name"], facts["stem"], facts["total"], facts["kind"]
    out = ["/* %s -- reconstructed source." % name, " *",
           " * STATUS, generated %s by CodeBreaker (verifier settings %s)." % (facts["date"], facts["digest"]),
           " * %d functions:" % total]
    out += _counts(facts, " * ")
    whole = facts.get("whole")
    if whole and whole["linked"]:
        out += [" *", " * Builds as a whole module: YES (%s). Every original export is present; a function the" % (
            "all the original's exports match" if whole["exports_same"] == whole["exports_total"] else
            "%d of %d exports match" % (whole["exports_same"], whole["exports_total"])),
            " * compiler rejected, or a call to code with no reconstruction, is a stand-in that returns 0",
            " * (%d rejected, %d absent). The built image is %s." % (whole["rejected"], whole["absent"], whole["built"]),
            " * Runs in place of the original: NOT TESTED as a whole.", " *"]
    else:
        out += [
            " *", " * Builds as a whole module: " + ("NOT YET (%s)." % whole["error"] if whole else
                                                    "not built here (run `codebreaker build %s`)." % name),
            " * Functions are compiled and checked one at a time; the module's data (globals, tables,",
            " * resources) is referred to by its address in the original unless the whole module is built.",
            " * Runs in place of the original: NOT TESTED.", " *",
            " * VERIFIED = rebuilt from this text with %s and run beside the original in %s:" % (
                ("a PowerPC compiler", "a PowerPC interpreter") if kind == "powerpc" else ("OpenWatcom", "an x86 emulator")),
            " * same return value, same calls out of the module with the same arguments, same memory changed,",
            " * same stack and saved registers, on every input tried. It is evidence, not proof. Names of",
            " * internal functions (sub_...) and locals are placeholders; no original names are claimed.",
            " * Written in after judging, changing no code: the SDK's names for constants passed to",
            " * Windows (with their values in %s.h), and names for locals, parameters and globals" % stem,
            " * taken from the Windows functions they are passed to or hold the result of.",
            " * IDENTIFIED = the function's bytes are those of a function in a library that came with the",
            " * compiler; it is named after that function. It is reconstructed and judged all the same."]
    if facts.get("real"):
        out += [" * Imports by number are written with their real names (%d of them; %s.h lists the" % (facts["real"], stem),
                " * module and number of each). The text that was built and judged calls them MODULE_number."]
    out += [" */", ""]
    if facts.get("header"):
        out.append('#include "%s.h"' % stem)
    out.append("")
    return out


def unit(u):
    """One function's part of the file: its status line as a comment, then its C."""
    kind = u.get("node")
    if kind == "MethodDecl":
        return "/* %s */\n%s\n" % ((u.get("comments") or [""])[0], u.get("text") or "")
    if kind == "Comment":
        return "/* %s */\n" % (u.get("text") or "")
    raise _asm.Decline("document %s" % kind)


def c(doc, facts, split=0):
    """The module's C, as files: [(file name, text)] -- one, or parts of at most `split` functions
    each (the status in the first)."""
    if doc.get("node") != "Module":
        raise _asm.Decline("document %s" % doc.get("node"))
    parts = [unit(u) for u in doc.get("units") or []]
    head = banner(facts)
    name, stem = facts["name"], facts["stem"]
    pieces = [parts] if not split or len(parts) <= split else [parts[k:k + split] for k in range(0, len(parts), split)]
    out = []
    for n, piece in enumerate(pieces):
        target = stem + ".c" if len(pieces) == 1 else "%s_%d.c" % (stem, n + 1)
        top = head if n == 0 else ["/* %s -- reconstructed source, part %d of %d (the status is in %s_1.c). */" % (
            name, n + 1, len(pieces), stem), ""] + [ln for ln in head if ln.startswith("#include")] + [""]
        out.append((target, "\n".join(top) + "\n" + "\n".join(piece)))
    return out


def asm_banner(facts):
    """The head of a driver's or a real-mode program's assembly file, as comments."""
    out = ["%s -- reconstructed source." % facts["name"], "",
           "STATUS, generated %s by CodeBreaker (verifier settings %s)." % (facts["date"], facts["digest"]),
           "%d entry points or functions:" % facts["total"]]
    out += [x[2:] for x in _counts(facts, "; ")]
    if facts.get("how"):
        out += ["", facts["how"][:160]]
    for va, name, title, what in facts.get("rows") or ():
        out.append("  %-10s %-28s %s %s" % (va, name, title, what))
    return out + [""]


def asm(doc, facts):
    """A driver's or a real-mode program's one assembly file: the status at its head as comments,
    then the module's source (`; (no source could be written)` when it has none)."""
    units = [{"node": "Comment", "text": t} for t in asm_banner(facts)]
    units += [u for u in doc.get("units") or []] or [{"node": "MachineFragment", "asm": "; (no source could be written)\n"}]
    return _asm.module({"node": "Module", "units": units})
