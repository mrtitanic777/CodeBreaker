#!/usr/bin/env python3
"""compile_errors.py -- why a module's decompiled functions do not compile, by kind, with a line.

    python tools/compile_errors.py <16-bit module> [--each N]

Compiles the whole decompiled module once, without dropping anything, and groups the compiler's
errors by message with example source lines. Fixing the decompiler's C output goes fastest when
the most common rejection is taken first.
"""
import collections
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from codebreaker.base import corpus, settings
    from codebreaker.build import watcom as period_tools
    from codebreaker.build import functions as module16
    from codebreaker.decompile import c_text
    path = corpus.retail(argv[0]) or argv[0]
    each = int(argv[argv.index("--each") + 1]) if "--each" in argv else 2
    work = settings.state("compile_errors", os.path.basename(path).lower())
    os.makedirs(work, exist_ok=True)
    m = c_text.compilable16(path, work)
    starts, parts, line = [], [m.header], m.header.count("\n") + 2
    for n in m.order:
        starts.append((line, n))
        parts.append(m.texts[n])
        line += m.texts[n].count("\n") + 2
    src = "\n\n".join(parts) + "\n"
    with open(os.path.join(work, "all.c"), "w", newline="\n", encoding="utf-8") as f:
        f.write(src)
    try:
        os.remove(os.path.join(work, "all.err"))
    except OSError:
        pass
    subprocess.run([period_tools.ow_tool("wcc")] + module16.CFLAGS16 + ["all.c"], cwd=work,
                   env=period_tools.ow_env("ne"), capture_output=True, text=True, errors="replace")
    try:
        log = open(os.path.join(work, "all.err"), errors="replace").read()
    except OSError:
        log = ""
    lines = src.split("\n")
    kinds, examples, owners = collections.Counter(), collections.defaultdict(list), collections.defaultdict(set)
    for mm in re.finditer(r"\((\d+)\): Error! (E\d+): ([^\r\n]*)", log):
        ln = int(mm.group(1))
        msg = re.sub(r"'[^']*'", "'..'", mm.group(3))[:70]
        owner = None
        for start, n in starts:
            if start <= ln:
                owner = n
            else:
                break
        key = mm.group(2) + ": " + msg
        if owner in owners[key]:
            continue                                        # one count per function
        owners[key].add(owner)
        kinds[key] += 1
        if len(examples[key]) < each:
            examples[key].append("%s: %s" % (owner, lines[ln - 1].strip()[:150]))
    bad = set().union(*owners.values()) if owners else set()
    print("%s: %d functions, %d with errors" % (os.path.basename(path), len(m.order), len(bad)))
    for key, n in kinds.most_common():
        print("%4d  %s" % (n, key))
        for e in examples[key]:
            print("        " + e)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
