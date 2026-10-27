"""java_writer.py -- a Java program's C++ source in a folder, every class labelled with what is known.

    python -m codebreaker module path/to/java/src          transpile, judge, then write the folder
    python -m codebreaker module path/to/java/src --out DIR

Reached through module_writer.write_module for a module of the java kind. The C++ is transpiled
again into the folder (codebreaker/java is deterministic: the same source gives the same files the
judge compiled), and each class's .cpp (or its .h, for a class with no .cpp) gets one status line
at its head, read from the store. STATUS.md lists every class with its status and the reason.

A file in the folder that CodeBreaker did not write is never replaced: the whole tree then goes
to state/source/<name>/ instead.
"""
import os

BANNER = "// %s -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [%s] %s\n"


def write_java(path, fs, sha, digest, out_dir, log=print):
    from codebreaker.base import settings
    from codebreaker.java import transpile
    from codebreaker.source.module_writer import made_by_us
    from codebreaker.store import functions as F
    from codebreaker.verify.lanes import java
    name = os.path.basename(os.path.normpath(path))
    if os.path.isdir(out_dir) and any(not made_by_us(os.path.join(out_dir, f))
                                      for f in os.listdir(out_dir) if f.endswith((".h", ".cpp"))):
        log("%s: holds files written by hand -- the C++ goes to the state folder instead" % out_dir)
        out_dir = settings.state("source", name)
    transpile.convert(java.source_dir(path), out_dir, platform=java.PLATFORM, write_report=False)
    best = fs.best(sha, digest)
    counts = dict.fromkeys(F.ORDER, 0)
    rows = []
    for cls in sorted(fs.addresses(sha)):
        row = best.get(cls)
        status = row["class"] if row else "no-source"
        why = (row.get("reason") or "") if row else "not judged on the current verifier"
        detail = (row.get("detail") or "") if row else ""
        counts[status] += 1
        said = (why if status == "no-compile" else (detail or why)).splitlines()
        said = said[0][:200] if said else ""
        rows.append((cls, status, said))
        for ext in (".cpp", ".h"):
            target = os.path.join(out_dir, cls + ext)
            if os.path.isfile(target):
                with open(target, encoding="utf-8", errors="replace") as f:
                    text = f.read()
                with open(target, "w", encoding="utf-8") as f:
                    f.write(BANNER % (cls + ext, F.TITLE[status], said) + text)
                break
    total = len(rows)
    lines = ["# %s -- C++03 transpiled from Java" % name, "",
             "%d classes: %s" % (total, ", ".join("%s %d" % (F.TITLE[k], counts[k]) for k in F.ORDER if counts[k])),
             "", "| class | status | why |", "|---|---|---|"]
    lines += ["| %s | %s | %s |" % (c, F.TITLE[s], w[:160].replace("|", "\\|")) for c, s, w in rows]
    with open(os.path.join(out_dir, "STATUS.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    log("%s: %d classes -> %s  (%s)" % (name, total, out_dir,
                                        ", ".join("%s %d" % (F.TITLE[k], counts[k]) for k in F.ORDER if counts[k])))
    return out_dir
