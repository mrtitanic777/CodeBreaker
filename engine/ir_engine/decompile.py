#!/usr/bin/env python3
r"""
decompile.py -- batch-run the IL engine (il.py) over a byte-exact reconstructed
asm corpus and emit readable C, one file per module, into reconstructed/auto/.

HONESTY RULE: machine-generated C is
SCAFFOLDING, never human reconstruction. Every function is prefixed with an
`[AUTO]` marker; ledger.py's auto_coverage() counts these in their own panel and
NEVER in the headline. A function is "promoted" only when a human reads it and
removes the `[AUTO] ` from its marker (which reveals the `name  MODULE seg...`
form verify.py/ledger.py recognise). So this scales COVERAGE of readable C
without inflating the honest reconstructed figure.

    python decompile.py                 # whole corpus
    python decompile.py --module access.cpl
    python decompile.py --limit 50      # first N modules (smoke test)
"""
import collections
import glob
import os
import re
import sys
import time

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
from engine.ir_engine import il
from engine.foundation import layout
try:
    from engine.foundation import progress
except ImportError:                                   # engine stays UI-agnostic; --ui just no-ops
    class _NoProgress:
        def __getattr__(self, _):
            return lambda *a, **k: None
    progress = _NoProgress()

ASM_ROOT = layout.asm_dir()                          # byte-exact asm  (legacy: reconstructed/asm)
OUT_ROOT = layout.generated_dir()                    # [AUTO] C        (legacy: reconstructed/auto)


def _meta(fname):
    """sec1_002d0.asm (PE) / seg1_00ba.asm (NE) / obj1_000b0.asm (LE/VxD) -> (seg, off).
    Falls back to (1, 0). Each format names its per-function asm differently -- PE `sec<n>_`,
    NE `seg<n>_`, LE `obj<n>_` (LE uses OBJECTS, not segments). Matching only some of these
    silently degenerated the others to (1, 0) -> all headers seg1:0x00000 (20k+ LE funcs)."""
    m = re.match(r"(?:se[cg]|obj)(\d+)_([0-9a-fA-F]+)", fname)
    return (int(m.group(1)), int(m.group(2), 16)) if m else (1, 0)


def _load_names(mod):
    """Binary-only names for this module, from nameoracle (reports/nameoracle/<mod>.json,
    keyed 'seg:off' -> {name, kind}). Returns {(seg, off): (name, kind)} or {} if the
    module hasn't been named. These are 100% binary-derived (export name table + the
    module's own call graph) -- no leaked source. Purely a readability layer over the
    [AUTO] scaffolding; a function stays UNREAD until a human promotes it."""
    import json
    for cand in (mod, mod.rsplit(".", 1)[0]):
        p = os.path.join(layout.runtime_dir(), "nameoracle", cand + ".json")
        if os.path.isfile(p):
            out = {}
            for k, v in json.load(open(p)).items():
                s, o = k.split(":")
                out[(int(s), int(o, 16))] = (v.get("name", ""), v.get("kind", ""))
            return out
    return {}


def _cident(name, seg, off, used):
    """Sanitize a proposed name to a UNIQUE C identifier within the module (the
    [AUTO] marker token must be a promotable \\w+). (seg, off) is unique, so the
    fallback suffix always disambiguates collisions (repeated thunk/dos/export names)."""
    base = re.sub(r"\W", "_", name) or "sub"
    if base[0].isdigit():
        base = "_" + base
    cand = base
    if cand in used:
        cand = "%s_%d_%05x" % (base, seg, off)
    used.add(cand)
    return cand


def decompile_module(moddir):
    mod = os.path.basename(moddir.rstrip("/\\"))
    modtok = re.sub(r"\W", "_", mod)                  # access_cpl  (C identifier)
    tag = mod.upper().replace(".", "_")               # ACCESS_CPL  (\w+, promotable)
    st = collections.Counter()
    unhandled = collections.Counter()
    blocks = []
    names = _load_names(mod)                           # binary-only idiomatic names (nameoracle)
    used = set()
    for f in sorted(glob.glob(os.path.join(moddir, "*.asm"))):
        seg, off = _meta(os.path.basename(f))
        nm, kind = names.get((seg, off), ("", ""))
        name = _cident(nm, seg, off, used) if nm else "%s_%05x" % (modtok, off)
        if nm:
            st["named"] += 1
        st["func"] += 1
        try:
            c = il.lift_asm_function(f, name)
        except Exception as e:
            st["error"] += 1
            blocks.append("/* [AUTO] %s  %s seg%d:0x%05x  ERROR:%s */"
                          % (name, tag, seg, off, str(e)[:60].replace("*/", "* /")))
            continue
        nraw = c.count("__asm(")
        ncall = len(re.findall(r"fnptr\)|\bsub_[0-9a-f]+\(", c))
        st["calls"] += ncall
        kind = "clean" if nraw == 0 else "raw%d" % nraw
        st["clean" if nraw == 0 else "raw"] += 1
        for op in re.findall(r"__asm\('([a-z0-9]+)", c):
            unhandled[op] += 1
        blocks.append("/* [AUTO] %s  %s seg%d:0x%05x  %s */\n%s"
                      % (name, tag, seg, off, kind, c))

    os.makedirs(OUT_ROOT, exist_ok=True)
    with open(layout.module_out_path(mod + ".c", "generated"), "w", encoding="utf-8") as w:
        # NB: no header line may begin with `/* [AUTO] ` -- ledger.py counts those
        # per-function; a header that did would inflate the scaffolding tally by 1.
        w.write("/* Auto-decompiled from the byte-exact %s reconstruction by "
                "il.py (the IR engine). */\n" % tag)
        if names:
            w.write("/* Function names are binary-derived (nameoracle: export name "
                    "table + this module's own call graph) — no leaked source. */\n")
        w.write("/* Machine-generated, UNREAD scaffolding: each function below "
                "carries an AUTO marker.\n   Promote one by reading it and "
                "deleting its `[AUTO] ` tag; not counted until then. */\n")
        w.write("/* Note: al/ah/bl../ax/bx.. denote the byte/word LANE of the "
                "32-bit register\n   (al = low byte of eax, ah = next byte, ax = "
                "low word); writing a lane\n   leaves the rest of the parent "
                "unchanged, exactly as on the hardware. */\n\n")
        w.write("\n\n".join(blocks))
    return st, unhandled


def main():
    a = sys.argv[1:]
    only = a[a.index("--module") + 1] if "--module" in a else None
    limit = int(a[a.index("--limit") + 1]) if "--limit" in a else None
    if "--ui" in a:
        progress.enable()                             # light the native Win95 window
    mods = ([layout.asm_module_dir(only)] if only else layout.asm_module_dirs())
    mods = [m for m in mods if os.path.isdir(m)]
    if "--resume" in a:                                   # skip modules already lifted
        def _done(md):
            out = layout.module_path(os.path.basename(md.rstrip("/\\")) + ".c", "generated")
            return os.path.isfile(out) and os.path.getsize(out) > 0
        before = len(mods)
        mods = [m for m in mods if not _done(m)]
        print("resume: %d/%d modules remain (%d already lifted)"
              % (len(mods), before, before - len(mods)), flush=True)
    if limit:
        mods = mods[:limit]

    grand = collections.Counter()
    unhandled = collections.Counter()
    t0 = time.time()
    progress.caption("CodeBreaker — Decompiler (asm → C)")
    progress.op("Auto-decompiling to C", len(mods))
    progress.modules_left(len(mods))
    for i, m in enumerate(mods, 1):
        name = os.path.basename(m)
        progress.next_module(name)
        st, un = decompile_module(m)
        grand.update(st)
        unhandled.update(un)
        progress.bar(i, len(mods))
        progress.modules_left(len(mods) - i)
        f = grand["func"] or 1
        # live Coverage tab: the honest scaffolding tally + clean-C rate. Guarded
        # because a target may carry an older progress module without coverage()
        # (e.g. win1.0's ported copy) — no-op there, real tab where it exists.
        getattr(progress, "coverage", lambda *a, **k: None)(
            "Auto-decompiled (scaffolding, [AUTO]):\n"
            "  modules:     %d / %d\n"
            "  functions:   %d\n"
            "  clean C:     %d  (%.1f%%)\n"
            "  with stubs:  %d\n"
            "  errored:     %d\n"
            "  calls -> C:  %d\n\n"
            "Top un-lifted opcodes (next targets):\n%s"
            % (i, len(mods), grand["func"], grand["clean"],
               100 * grand["clean"] / f, grand["raw"], grand["error"],
               grand["calls"],
               "\n".join("  %-8s %d" % (op, n) for op, n in unhandled.most_common(8))))
        if i % 25 == 0 or i == len(mods):
            line = ("[%d/%d] %-22s funcs=%d clean=%d raw=%d err=%d"
                    % (i, len(mods), name, grand["func"], grand["clean"],
                       grand["raw"], grand["error"]))
            progress.detail(line)
            print("  " + line, flush=True)

    dt = time.time() - t0
    f = grand["func"] or 1
    print("=" * 66)
    print("BATCH DECOMPILE  (%d modules, %.1fs)" % (len(mods), dt))
    print("  functions:         %d" % grand["func"])
    print("  idiomatic names:   %d  (%.1f%%, nameoracle; rest modtok_off)"
          % (grand["named"], 100 * grand["named"] / f))
    print("  fully clean C:     %d  (%.1f%%)" % (grand["clean"], 100 * grand["clean"] / f))
    print("  with raw stubs:    %d  (%.1f%%)" % (grand["raw"], 100 * grand["raw"] / f))
    print("  errored:           %d" % grand["error"])
    print("  calls lifted to C: %d" % grand["calls"])
    print("  -> %s/*.c  (counted as [AUTO] scaffolding)" % OUT_ROOT)
    if unhandled:
        print("  top un-lifted opcodes (what to teach il.py next):")
        for op, n in unhandled.most_common(15):
            print("     %-8s %d" % (op, n))
    if only is None and limit is None:               # persist a full-run summary for
        import json                                  # the unified ledger's readable-C panel
        os.makedirs(layout.runtime_dir(), exist_ok=True)
        json.dump({"functions": grand["func"], "clean": grand["clean"],
                   "clean_pct": round(100 * grand["clean"] / f, 1), "raw": grand["raw"],
                   "error": grand["error"], "calls": grand["calls"],
                   "modules": len(mods), "seconds": round(dt, 1), "ts": int(t0),
                   "top_unlifted": [[op, n] for op, n in unhandled.most_common(10)]},
                  open(os.path.join(layout.runtime_dir(), "auto_summary.json"), "w"), indent=1)
        print("  -> reports/auto_summary.json  (readable-C panel source)")
    progress.finish("%d funcs · %d clean C (%.0f%%) · %d calls lifted"
                    % (grand["func"], grand["clean"],
                       100 * grand["clean"] / (grand["func"] or 1), grand["calls"]))


if __name__ == "__main__":
    main()
