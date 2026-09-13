#!/usr/bin/env python3
"""industrialize95.py -- the C-reconstruction conductor: drive modules from the Ghidra decompile to
authored period C that compiles, links, and diffs close to the shipped binary -- at corpus scale.

Distinct from conduct.py (the asm / byte-exact path, which merely re-records the compiler's output).
This is the READABLE-C path -- the real reconstruction goal. It does NOT scale by hand-spawning an
agent per segment (one silently hung for 23 min during lzexpand). Instead:

  1. FIX-ENGINES -- deterministic, idempotent repairs that recurred by hand on lzexpand, so the LLM
     layer never re-does them: the MK_FP macro guard, the DGROUP data-definitions TU, the LINK
     response file, and the FAR-PASCAL / .def export reconciliation.
  2. REBUILD-AND-DIFF -- the universal gate AND the convergence metric. Nothing is "done" without a
     compile + link + per-segment diff vs the shipped binary behind it (which is exactly what would
     have caught the stalled lzread agent).
  3. LEDGER + feedback -- a resumable per-module JSON records what each engine changed, the build
     result, and the per-segment convergence, so recurring residue can be promoted into new engines.

    wt --target win95 industrialize95 <MODULE>            # curate + build + diff one module
    wt --target win95 industrialize95 --tier core         # a whole tier
    wt --target win95 industrialize95 <MODULE> --fix-only  # apply fix-engines, don't build
    wt --target win95 industrialize95 --selftest

The LLM-curation step (translate raw decompile bodies -> authored C) is orchestrated separately via
the Workflow tool; this module is the deterministic spine it plugs into (fix-engines before, build +
diff after), so an agent's output is always verified by a rebuild, never trusted on a "done" report.
"""
import glob
import json
import os
import re
import sys

from src.core import layout, paths

LEDGER_DIR = os.path.join(layout.runtime_dir(), "industrialize")

# ------------------------------------------------------------------ fix-engines
# Each engine is fix(recon_dir, module) -> {"engine","changed","notes"}; deterministic + idempotent,
# so re-running a converged module is a no-op. Harvested from the lzexpand curation.

_HDR_EXTERN = re.compile(
    r'^\s*extern\s+([A-Za-z_][\w\s\*]*?)\s+([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;', re.M)


def _shared_header(recon_dir):
    """The module's own internal header (smallest-named .h that the .c files #include)."""
    hs = [h for h in glob.glob(os.path.join(recon_dir, "*.h"))]
    return min(hs, key=lambda p: len(os.path.basename(p))) if hs else None


def ensure_mkfp_macro(recon_dir, module):
    """MSC 6.0a dos.h has FP_SEG/FP_OFF but NOT MK_FP -- without a macro it compiles as an implicit
    int call and silently truncates the far pointer's segment. Inject a guarded macro in the header
    whenever a .c builds a far pointer with MK_FP. (Caught as a real bug in lzexpand.)"""
    uses = any("MK_FP(" in open(c, errors="ignore").read()
               for c in glob.glob(os.path.join(recon_dir, "*.c")))
    hdr = _shared_header(recon_dir)
    if not (uses and hdr):
        return {"engine": "mkfp", "changed": False, "notes": "no MK_FP use / no header"}
    txt = open(hdr, errors="ignore").read()
    if "MK_FP" in txt and re.search(r'#\s*define\s+MK_FP', txt):
        return {"engine": "mkfp", "changed": False, "notes": "macro already present"}
    macro = ("\n/* MSC 6.0a dos.h lacks MK_FP; define it so far pointers aren't truncated. */\n"
             "#ifndef MK_FP\n"
             "#define MK_FP(seg, off)  ((void FAR *)(((DWORD)(WORD)(seg) << 16) | (WORD)(off)))\n"
             "#endif\n")
    m = list(re.finditer(r'^\s*#\s*include\s+[<"][^>"]+[>"].*$', txt, re.M))
    at = m[-1].end() if m else 0
    open(hdr, "w").write(txt[:at] + "\n" + macro + txt[at:])
    return {"engine": "mkfp", "changed": True, "notes": "injected MK_FP into %s" % os.path.basename(hdr)}


def synth_data_tu(recon_dir, module):
    """The DGROUP globals are declared `extern` in the header; a 16-bit module will not LINK until one
    TU DEFINES them. If the module has no data-definitions TU, emit <mod>data.c defining every extern
    (zero-init). CONSERVATIVE: if a data TU already exists (a *data.c, or a .c that already defines a
    good share of the externs) it is a strict no-op, so it never creates a duplicate-symbol clash.
    (lzexpand needed this -- it had never linked because nothing defined the globals.)"""
    hdr = _shared_header(recon_dir)
    if not hdr:
        return {"engine": "data_tu", "changed": False, "notes": "no header"}
    decls = {}                                     # name -> full decl text
    for typ, name, arr in _HDR_EXTERN.findall(open(hdr, errors="ignore").read()):
        if name in ("FAR", "PASCAL") or not typ.strip():
            continue
        decls[name] = "%s %s%s;" % (typ.strip(), name, arr or "")
    if not decls:
        return {"engine": "data_tu", "changed": False, "notes": "no extern globals"}
    cs = glob.glob(os.path.join(recon_dir, "*.c"))
    if any(os.path.basename(c).endswith("data.c") for c in cs):
        return {"engine": "data_tu", "changed": False, "notes": "data TU already present"}
    # count externs that appear as a file-scope definition (a line with the name, no leading extern)
    defined = 0
    for c in cs:
        for ln in open(c, errors="ignore"):
            s = ln.strip()
            if not s or s.startswith(("extern", "//", "*", "/*")):
                continue
            for name in decls:
                if re.search(r'(^|[\s,\*])%s\s*(\[[^\]]*\])?\s*(,|;|=)' % re.escape(name), s):
                    defined += 1
                    break
    if defined >= max(1, len(decls) // 2):
        return {"engine": "data_tu", "changed": False,
                "notes": "globals already defined (%d/%d seen)" % (defined, len(decls))}
    out = os.path.join(recon_dir, "%sdata.c" % _stem(module))
    body = ["/* %s -- DGROUP data: definitions for the extern globals in the header (zero-init). */"
            % os.path.basename(out),
            '#include "%s"' % os.path.basename(hdr), ""]
    body += [decls[n] for n in decls]
    open(out, "w").write("\n".join(body) + "\n")
    return {"engine": "data_tu", "changed": True,
            "notes": "emitted %s defining %d globals" % (os.path.basename(out), len(decls))}


def gen_linkfile(recon_dir, module):
    """Generate the LINK response (.lnk): object modules in NE segment order (data TU last), the
    medium-model DLL C runtime + Windows import lib, and the .def. Segment order comes from
    DECOMPILE_MANIFEST.json when present, else the .c files alphabetically. (lzexpand's checked-in
    lz.lnk was stale -- old module names -- and had to be rewritten by hand.)"""
    stem = _stem(module)
    defs = glob.glob(os.path.join(recon_dir, "*.def"))
    if not defs:
        return {"engine": "linkfile", "changed": False, "notes": "no .def -- cannot link"}
    existing = glob.glob(os.path.join(recon_dir, "*.lnk"))
    has_manifest = os.path.exists(os.path.join(recon_dir, "DECOMPILE_MANIFEST.json"))
    # Conservative: don't reorder a hand-made link file without a manifest to order segments by.
    if existing and not has_manifest:
        return {"engine": "linkfile", "changed": False,
                "notes": "kept %s (no manifest to reorder from)" % os.path.basename(existing[0])}
    order = _segment_order(recon_dir)
    objs = "+".join("%s.obj" % o for o in order)
    lnk = ("/NOD /NOE /NOPACKCODE /ALIGN:16 %s\n%s.dll\n%s.map\nmdllcew.lib libw.lib\n%s\n"
           % (objs, stem, stem, os.path.basename(defs[0])))
    path = existing[0] if existing else os.path.join(recon_dir, "%s.lnk" % stem)
    old = open(path).read() if os.path.exists(path) else None
    if old == lnk:
        return {"engine": "linkfile", "changed": False,
                "notes": "%s up to date" % os.path.basename(path)}
    open(path, "w").write(lnk)
    return {"engine": "linkfile", "changed": True,
            "notes": "wrote %s (%d objs)" % (os.path.basename(path), len(order))}


def reconcile_exports(recon_dir, module):
    """Flag exported functions that are not FAR PASCAL: Win16 uppercases PASCAL symbols, which is how
    they match the .def's EXPORTS names. A cdecl export gets a `_name` symbol and silently fails to
    resolve. Report-only (renaming touches bodies -- left to the curation step)."""
    defs = glob.glob(os.path.join(recon_dir, "*.def"))
    if not defs:
        return {"engine": "exports", "changed": False, "notes": "no .def"}
    exports = re.findall(r'^\s*([A-Za-z_]\w*)\s+@\d+', open(defs[0], errors="ignore").read(), re.M)
    src = ""
    for c in glob.glob(os.path.join(recon_dir, "*.c")):
        src += open(c, errors="ignore").read()
    missing_pascal = []
    for e in exports:
        if e.startswith("_"):
            continue                               # linker/CRT stubs (e.g. ___EXPORTEDSTUB) are exempt
        # is this export DEFINED (name followed by '(' at a line start), and if so is it PASCAL?
        if not re.search(r'(?im)^[A-Za-z_][\w \t\*]*\b%s\s*\(' % re.escape(e), src):
            continue                               # not defined here (may live in another segment)
        if not re.search(r'(?i)\bPASCAL\s+%s\s*\(' % re.escape(e), src):
            missing_pascal.append(e)
    return {"engine": "exports", "changed": False,
            "notes": ("all exports FAR PASCAL" if not missing_pascal
                      else "NOT FAR PASCAL: %s" % ", ".join(sorted(set(missing_pascal))))}


FIX_ENGINES = [ensure_mkfp_macro, synth_data_tu, gen_linkfile, reconcile_exports]


# ------------------------------------------------------------------ helpers
def _stem(module):
    return os.path.splitext(os.path.basename(module))[0].lower()


def _segment_order(recon_dir):
    """NE-segment order of the object modules; data TU last."""
    cs = [os.path.splitext(os.path.basename(c))[0] for c in glob.glob(os.path.join(recon_dir, "*.c"))]
    man = os.path.join(recon_dir, "DECOMPILE_MANIFEST.json")
    rank = {}
    if os.path.exists(man):
        try:
            segs = json.load(open(man)).get("segments", {})
            items = segs.items() if isinstance(segs, dict) else \
                enumerate(segs, 1)                       # dict keyed by seg-num, or a list
            for key, e in items:
                f = os.path.splitext(os.path.basename(e.get("file", e.get("name", ""))))[0]
                if f:
                    try:
                        rank[f] = int(key)
                    except (ValueError, TypeError):
                        rank[f] = e.get("seg", e.get("idx", 999))
        except Exception:
            pass
    datatu = [c for c in cs if c.endswith("data")]
    code = [c for c in cs if c not in datatu]
    code.sort(key=lambda f: (rank.get(f, 998), f))
    return code + datatu


# ------------------------------------------------------------------ build + diff (the gate + metric)
def build_and_diff(recon_dir, module, oracle=None):
    """Compile + link the recon dir (MSC 6.0a + LINK 5.60, via build_dll) and diff the result against
    the shipped binary per segment. Returns a convergence report: linked?, sizes, per-segment deltas,
    and a score in [0,1]."""
    from src.engines.msc6 import build_dll
    if oracle is None:
        oracle = _oracle_for(module)
    built = build_dll.build(recon_dir)
    rep = {"module": _stem(module), "linked": bool(built and os.path.exists(built)),
           "oracle": oracle, "built": built}
    if not rep["linked"] or not (oracle and os.path.exists(oracle)):
        rep["score"] = 0.0
        return rep
    B = build_dll._segments(built)
    O = build_dll._segments(oracle)
    rep["built_size"] = os.path.getsize(built)
    rep["oracle_size"] = os.path.getsize(oracle)
    rep["built_segs"], rep["oracle_segs"] = len(B), len(O)
    segs, inter, union, ident = [], 0, 0, 0
    for i in range(max(len(B), len(O))):
        bl = B[i][0] if i < len(B) else 0
        ol = O[i][0] if i < len(O) else 0
        same = (i < len(B) and i < len(O) and B[i][1] == O[i][1])
        ident += 1 if same else 0
        inter += min(bl, ol)
        union += max(bl, ol)
        segs.append({"seg": i + 1, "built": bl, "oracle": ol,
                     "verdict": "IDENTICAL" if same else ("same-len" if bl == ol else "diff-len")})
    rep["segments"] = segs
    rep["byte_identical_segs"] = ident
    rep["size_ratio"] = round(rep["built_size"] / max(1, rep["oracle_size"]), 4)
    rep["score"] = round(inter / max(1, union), 4)    # segment-size overlap (1.0 == every seg matches)
    return rep


def _oracle_for(module):
    """The shipped binary for a module, from the Win95 corpus. Returns a FILE path (never a dir)."""
    if os.path.isabs(module) and os.path.isfile(module):
        return module
    win95 = os.path.join(os.path.dirname(paths.repo_root()), "Windows95")
    sysdir = os.path.join(win95, "design", "corpus", "files", "WINDOWS", "SYSTEM")
    cands = [os.path.join(sysdir, _stem(module) + ext) for ext in (".dll", ".exe", ".drv")]
    cands += [p for p in glob.glob(os.path.join(win95, "design", "corpus", "files", "**",
                                                 _stem(module) + ".dll"), recursive=True)]
    for c in cands:
        if os.path.isfile(c):
            return c
    return None


# ------------------------------------------------------------------ per-module + batch
def curate(recon_dir, module, fix_only=False, oracle=None):
    """Run the fix-engines, then (unless fix_only) build + diff. Persist a resumable ledger."""
    os.makedirs(LEDGER_DIR, exist_ok=True)
    fixes = [fx(recon_dir, module) for fx in FIX_ENGINES]
    result = {"module": _stem(module), "recon_dir": recon_dir, "fixes": fixes}
    if not fix_only:
        result["build"] = build_and_diff(recon_dir, module, oracle)
    open(os.path.join(LEDGER_DIR, "%s.json" % _stem(module)), "w").write(
        json.dumps(result, indent=2))
    return result


def _fmt_report(r):
    out = ["== industrialize %s ==" % r["module"]]
    for f in r["fixes"]:
        out.append("  fix %-9s %s  %s" % (f["engine"], "*" if f["changed"] else " ", f["notes"]))
    b = r.get("build")
    if b:
        if not b.get("linked"):
            out.append("  BUILD: did not link (check CL.TXT/LINK.TXT)")
        else:
            out.append("  BUILD linked: %d B vs %d B (ratio %.3f), score %.3f, %d/%d segs identical"
                       % (b["built_size"], b["oracle_size"], b["size_ratio"], b["score"],
                          b["byte_identical_segs"], b["oracle_segs"]))
            for s in b["segments"]:
                if s["verdict"] != "IDENTICAL":
                    out.append("    seg%-2d built=%-5d oracle=%-5d %s"
                               % (s["seg"], s["built"], s["oracle"], s["verdict"]))
    return "\n".join(out)


def main(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="industrialize95")
    ap.add_argument("module", nargs="?", help="module name or path (e.g. LZEXPAND)")
    ap.add_argument("--recon-dir", help="reconstruction dir (defaults to the module's source dir)")
    ap.add_argument("--oracle", help="shipped binary to diff against")
    ap.add_argument("--fix-only", action="store_true", help="apply fix-engines, skip the build")
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args(argv)
    if a.selftest:
        return _selftest()
    if not a.module:
        ap.print_help()
        return 2
    recon = a.recon_dir or _default_recon_dir(a.module)
    if not recon or not os.path.isdir(recon):
        print("no recon dir for %s (pass --recon-dir)" % a.module)
        return 1
    print(_fmt_report(curate(recon, a.module, fix_only=a.fix_only, oracle=a.oracle)))
    return 0


def _default_recon_dir(module):
    base = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "base", _stem(module))
    return base if os.path.isdir(base) else None


def _selftest():
    """The fix-engines must be idempotent no-ops on the already-converged lzexpand module."""
    recon = _default_recon_dir("lzexpand")
    if not recon:
        print("selftest: lzexpand recon dir not found"); return 1
    fixes = [fx(recon, "lzexpand") for fx in FIX_ENGINES]
    changed = [f for f in fixes if f["changed"]]
    for f in fixes:
        print("  %-9s changed=%s  %s" % (f["engine"], f["changed"], f["notes"]))
    ok = not changed
    print("selftest %s" % ("PASS (idempotent on converged module)" if ok
                           else "NOTE: engines changed a converged module -- review"))
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
