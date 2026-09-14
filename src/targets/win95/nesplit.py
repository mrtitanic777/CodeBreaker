#!/usr/bin/env python3
"""nesplit.py -- multi-TU split for oversized 16-bit (NE) modules.

A 16-bit code segment tops out at 64 KB. A few Win95 NE DLLs decompile to one huge
translation unit that no single segment can hold (mapi.dll: 2,177 functions, ~1.2 MB
of C) -- wcc rejects it with E1118 "segment too large". This is a hard architectural
limit, not an emitter defect: the real DLL was itself a multi-segment NE.

nesplit partitions the auto-cured C into several .c files, each small enough for its
own code segment, sharing:
  * a common header  -- the prelude #defines, struct definitions and function
    prototypes, plus the register/temp globals as `extern` (declared once),
  * one globals .c   -- the single definition of those shared globals (medium model
    keeps them in the one 64 KB DGROUP, so they must be defined exactly once).

Each chunk `#include`s the header, so cross-chunk calls and shared temps resolve.
`compile_split` compiles every TU (medium-model wcc) and reports clean iff all do.
The authentic multi-segment structure falls out for free; curation later gives the
files meaningful names/splits.
"""
import os
import re

from src.targets.win95 import owbuild

# `static <type> <names...>;` at file scope -- the register/flag/temp globals the
# cure prelude defines. [^;] spans newlines, so the multi-line v0.. list is one match.
_GLOBAL_RE = re.compile(r"(?m)^static\s+((?:unsigned\s+)?[A-Za-z_]\w*)\s+([^;]*);")
_FUNC_HEAD = re.compile(r"(?m)^int\s+[A-Za-z_]\w*\(\)\s*$")
# a whole `struct Tag { ... };` definition (autocure already merged/deduped these,
# but the emitter left them scattered inline before each tag's first use).
_STRUCT_DEF = re.compile(r"(?ms)^struct\s+(\w+)\s*\{.*?^\};\s*$")


def _split_functions(bodies):
    """Brace-matched split of the K&R `int name()` function definitions."""
    funcs = []
    for m in _FUNC_HEAD.finditer(bodies):
        br = bodies.find("{", m.end())
        if br < 0:
            continue
        depth, j = 0, br
        while j < len(bodies):
            ch = bodies[j]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        funcs.append(bodies[m.start():j + 1])
    return funcs


def prepare(cured, base, outdir, budget=40000):
    """Write the header, globals TU and chunk TUs. Returns the list of .c basenames."""
    os.makedirs(outdir, exist_ok=True)

    # Hoist every struct definition into the shared header: the emitter leaves them
    # scattered inline before each tag's first use, so a chunk that uses a struct
    # defined in another chunk would fail (E1031). Collect (unique by tag) and strip
    # them from the source. Pointer-only use means field order across tags is free.
    struct_defs, seen_tags = [], set()

    def _grab(m):
        if m.group(1) not in seen_tags:
            seen_tags.add(m.group(1)); struct_defs.append(m.group(0))
        return ""

    cured = _STRUCT_DEF.sub(_grab, cured)

    fm = _FUNC_HEAD.search(cured)
    if not fm:
        return None
    head, bodies = cured[:fm.start()], cured[fm.start():]

    # Header: externalize the shared global definitions so including it in every TU
    # does not multiply-define them.
    global_defs = []

    def _extern(m):
        global_defs.append(m.group(0)[len("static "):])   # `unsigned int r0,...;`
        return "extern " + m.group(0)[len("static "):]

    header = _GLOBAL_RE.sub(_extern, head)
    hname = base + "_common.h"
    with open(os.path.join(outdir, hname), "w", encoding="utf-8", newline="\n") as f:
        f.write("/* nesplit shared declarations for %s */\n" % base)
        # prelude/#defines + externed globals + prototypes first, then all structs
        f.write(header)
        f.write("\n/* hoisted struct definitions */\n")
        f.write("\n".join(struct_defs) + "\n")

    files = []
    gc = base + "_globals.c"
    with open(os.path.join(outdir, gc), "w", encoding="utf-8", newline="\n") as f:
        f.write('#include "%s"\n' % hname)
        f.write("/* single definition of the shared temporaries (one DGROUP copy) */\n")
        f.write("\n".join(global_defs) + "\n")
    files.append(gc)

    # Chunk the function bodies by cumulative source size.
    funcs = _split_functions(bodies)
    chunks, cur, sz = [], [], 0
    for fn in funcs:
        if cur and sz + len(fn) > budget:
            chunks.append(cur); cur, sz = [], 0
        cur.append(fn); sz += len(fn)
    if cur:
        chunks.append(cur)

    for i, ch in enumerate(chunks):
        cn = "%s_%03d.c" % (base, i)
        with open(os.path.join(outdir, cn), "w", encoding="utf-8", newline="\n") as f:
            f.write('#include "%s"\n' % hname)
            f.write("\n".join(ch) + "\n")
        files.append(cn)
    return files


def compile_split(cured, base, outdir, budget=40000):
    """Split then compile every TU with medium-model wcc. Clean iff all compile."""
    files = prepare(cured, base, outdir, budget)
    if not files:
        return {"ok": False, "error": "no functions to split"}
    env = owbuild._ow_env("ne")
    log = []
    flags = ["-bt=windows", "-mm", "-q", "-s"]
    bad = []
    for cf in files:
        rc = owbuild._run(["wcc"] + flags + [cf], outdir, env, log)
        if rc != 0:
            # find this file's error tail
            errs = [l for l in log if "Error!" in l]
            bad.append((cf, errs[-1] if errs else "rc=%d" % rc))
    return {"ok": not bad, "chunks": len(files), "failed": bad,
            "log": log, "dir": outdir}


if __name__ == "__main__":
    import sys
    from src.targets.win95 import nativedec, autocure, buildall
    name = sys.argv[1]
    path = name if os.path.isfile(name) else None
    if not path:
        for dp, _, fs in os.walk(buildall._corpus()):
            for f in fs:
                if f.lower() == name.lower():
                    path = os.path.join(dp, f)
        # prefer system copy
    c = nativedec.decompile_module(path, timeout=400, sigs=False)
    cured, _ = autocure.cure_text(c)
    base = os.path.splitext(os.path.basename(path))[0]
    out = os.path.join(buildall._design(), "scaffold", base, "split")
    r = compile_split(cured, base, out)
    print("nesplit %s: ok=%s chunks=%d" % (base, r["ok"], r.get("chunks", 0)))
    for cf, e in r.get("failed", [])[:10]:
        print("   FAIL", cf, e)
