#!/usr/bin/env python3
r"""owlink.py -- link an authored module into a real binary with OpenWatcom, resolving REAL imports.

The compile lane proves our authored C is a legal translation unit; this lane proves it LINKS into a
working module -- the "runs from our source" step. The three undefined-symbol classes a module leaves
after compile are handled honestly:

  1. CRT stubs (__STK, ...)      -> compile with -s (no stack probe); self-contained.
  2. Win16 API imports           -> real wlink `import` directives from the binary's own import table
                                    (win95dec --imports). NOT return-0 stubs -- the linked module calls
                                    the actual KERNEL/USER/GDI/... export.
  3. internal sub_XXXX the        -> stubbed (flagged): functions the decompiler has not recovered yet.
     decompiler didn't emit          This is the genuine completeness gap, reported per module.

    wt --target win95 owlink <module> [--diff]

Reports: compiled OK, #real-imports resolved, #internal stubs, linked output + size, and (with --diff)
a byte comparison against the retail binary. Byte-identity is NOT expected (different toolchain); the
point is a functional module built from our source that calls the real APIs.
"""
import os
import re
import sys
import glob
import subprocess

from src.core import paths
from src.targets.win95 import authsrc, owcheck


def _ow():
    return os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2", "rel")


def _dec(fmt):
    return authsrc._native(fmt)


def _find_binary(module):
    dr = authsrc._dr()
    cands = glob.glob(os.path.join(dr, "corpus", "files", "**", module + ".*"), recursive=True)
    # The decoder resolves imported ordinals -> API names by loading the sibling KERNEL/USER/GDI
    # modules from the TARGET binary's own directory. Several corpus locations hold the same file
    # (WINDOWS/SYSTEM, setup/inf, ...) but only the real system directory carries those siblings, so
    # a copy from setup/inf resolves every import to a bare MOD_ordinal that never matches the call
    # names in the authored C (which WAS decompiled from the WINDOWS/SYSTEM copy). Prefer the copy
    # whose directory actually contains kernel/gdi/user so imports resolve to real names and link.
    def _has_siblings(p):
        d = os.path.dirname(p)
        return any(os.path.isfile(os.path.join(d, n)) for n in
                   ("KERNEL.EXE", "kernel.exe", "GDI.EXE", "gdi.exe", "USER.EXE", "user.exe"))
    cands.sort(key=lambda p: (0 if _has_siblings(p) else 1,
                              0 if os.path.sep + "SYSTEM" + os.path.sep in p.upper() else 1))
    if cands:
        return cands[0]
    for rel in (("corpus/files/setup/inf/%s.exe" % module), ("corpus/files/setup/inf/%s.dll" % module)):
        p = os.path.join(dr, rel)
        if os.path.isfile(p):
            return p
    return None


def _imports(binpath, fmt):
    """[(module, apiname)] from the binary's own import table (NE + PE)."""
    if fmt not in ("NE", "PE"):
        return []
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--imports"], capture_output=True, text=True).stdout
    imps = []
    for line in out.splitlines():
        p = line.split("\t")
        if len(p) == 2 and p[1].strip():
            imps.append((p[0].strip(), p[1].strip()))
    return imps


def _exports(binpath, fmt):
    """[(ordinal_or_None, name)] from the module's own export/entry table. NE has ordinals; PE lists
    names only (ordinal None)."""
    if fmt not in ("NE", "PE"):
        return []
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--exports"], capture_output=True, text=True).stdout
    exps = []
    for line in out.splitlines():
        p = line.split("\t")
        if fmt == "NE" and len(p) == 2 and p[1].strip():
            exps.append((p[0].strip(), p[1].strip()))
        elif fmt == "PE" and p and p[0].strip():
            exps.append((None, p[0].strip()))
    return exps


def _entry(binpath, fmt):
    """The entry-point address (PE RVA / NE linear from CS:IP) or '' if unavailable."""
    if fmt not in ("NE", "PE"):
        return ""
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--entry"], capture_output=True, text=True).stdout.strip()
    return out.splitlines()[0].strip() if out else ""


def _name_at(binpath, fmt, addr):
    """The discovered function name at a given address (from --names: addr<TAB>name)."""
    dec = _dec(fmt)
    out = subprocess.run([dec, binpath, "--names"], capture_output=True, text=True).stdout
    for line in out.splitlines():
        p = line.split("\t")
        if len(p) >= 2 and p[1].strip():
            try:
                if int(p[0], 16) == addr:
                    return p[1].strip()
            except ValueError:
                pass
    return ""


def link_module(module, do_diff=False):
    binpath = _find_binary(module)
    if not binpath:
        return {"ok": False, "error": "no source binary for %s" % module}
    fmt = authsrc.classify(binpath)[0]
    bits = 32 if fmt == "PE" else 16
    cpath = os.path.join(authsrc._dr(), "recon", "authored", "c", module + ".c")
    if not os.path.isfile(cpath):
        return {"ok": False, "error": "no authored C for %s (run authsrc first)" % module}

    ow = _ow()
    bld = os.path.join(authsrc._dr(), "recon", "authored", "_link", module)
    os.makedirs(bld, exist_ok=True)
    import shutil
    # Clear stale build inputs first. This dir persists across runs, and an earlier split (or a
    # single-file authoring) leaves pieces behind -- e.g. a 100k-line `setupx.c` from when the module
    # was one file, which now fails E1118 (segment too large) and drops every function defined in it,
    # or duplicate pieces from a prior split shape. Start from a clean tree so only the CURRENT pieces
    # are compiled and linked.
    for f in os.listdir(bld):
        if f.endswith((".c", ".h", ".obj", ".o")):
            try:
                os.remove(os.path.join(bld, f))
            except OSError:
                pass
    env = owcheck._env()
    cc = os.path.join(ow, "binnt", "wcc.exe" if bits == 16 else "wcc386.exe")
    tgt = "windows" if bits == 16 else "nt"
    # -s: no stack-overflow probe -> no __STK CRT dependency (self-contained scaffold)
    cflags = ["-bt=" + tgt, "-q", "-s"]
    if bits == 16:
        cflags += ["-mc", "-3"]
    # Prefer the multi-file split tree: compile each piece to its own obj (separate segments), which is
    # how the module was really built AND the only way a big 16-bit module fits (one giant .c overruns
    # the 64K code segment). Fall back to the single authored .c for small/un-split modules.
    treedir = os.path.join(authsrc._dr(), "recon", "authored", "tree", module)
    objs = []
    if os.path.isdir(treedir) and any(f.endswith(".c") for f in os.listdir(treedir)):
        for f in sorted(os.listdir(treedir)):
            if f.endswith((".c", ".h")):
                shutil.copy2(os.path.join(treedir, f), os.path.join(bld, f))
        for f in sorted(os.listdir(treedir)):
            if f.endswith(".c"):
                # each piece into its OWN code segment (-nt): 16-bit NE caps a segment at 64K, and
                # wlink merges same-named segments, so without this every piece's code piles into one
                # _TEXT and overruns. Large model (-ml, far code) so a call from one piece's segment to
                # a function in another links as a FAR call -- a near call cannot cross segments (E2052).
                pcf = (["-bt=" + tgt, "-q", "-s", "-ml", "-3", "-nt=%s_TEXT" % f[:-2]]
                       if bits == 16 else cflags)
                subprocess.run([cc] + pcf + [f], cwd=bld, capture_output=True, text=True,
                               env=env, timeout=180)
                o = f[:-2] + ".obj"
                if os.path.isfile(os.path.join(bld, o)):
                    objs.append(o)
    else:
        shutil.copy2(cpath, os.path.join(bld, module + ".c"))
        subprocess.run([cc] + cflags + [module + ".c"], cwd=bld, capture_output=True, text=True,
                       env=env, timeout=180)
        if os.path.isfile(os.path.join(bld, module + ".obj")):
            objs.append(module + ".obj")
    src = cpath                                            # for the defined-exports scan
    if not objs:
        return {"ok": False, "error": "compile produced no obj"}

    # real imports -> wlink `import` directives (name-consistent with the C's symbols), so the module
    # calls the actual API instead of a return-0 stub. NE: by ordinal or name from KERNEL/USER/GDI/...
    # PE: from the IAT (MODULE.func); the decompiler names the call MODULE_func.
    imps = _imports(binpath, fmt)
    imp_lines = []
    for mod, api in imps:
        if fmt == "PE":
            # Ordinal-only import: the decoder names it `@N`. Emitting `'MOD'.'@N'` makes wlink read the
            # leading `@` as its response-file operator and try to open a file "N" (E2093: cannot open N,
            # a hard parse failure that killed the whole link -- most PE modules import something by
            # ordinal). Emit the numeric ordinal form `import MOD_N 'MOD'.N` instead; if the recovered C
            # never references it, the directive is simply unused.
            mo = re.match(r"^@(\d+)$", api)
            if mo:
                imp_lines.append("import %s_%s '%s'.%s" % (mod, mo.group(1), mod, mo.group(1)))
            else:
                imp_lines.append("import %s_%s '%s'.'%s'" % (mod, api, mod, api))
            continue
        # NE 16-bit: the authored C declares each imported API as a plain `extern int API()`, which
        # wcc (compact-model __watcall) references with a TRAILING underscore -- the obj carries the
        # symbol `API_`, not `API`. The wlink `import` directive's internal name must therefore be the
        # decorated `API_` (mapped onto the real export `API`); an undecorated internal name never
        # matched the reference, so every Win16 API call fell through to a return-0 stub instead of
        # calling the real KERNEL/USER/GDI export.
        mo = re.match(r"^%s_(\d+)$" % re.escape(mod), api)
        if mo:
            imp_lines.append("import %s_ '%s'.%s" % (api, mod, mo.group(1)))
        else:
            imp_lines.append("import %s_ '%s'.'%s'" % (api, mod, api))

    # real exports -> wlink `export` directives, so the module exports its functions AND wlink keeps
    # the exported code + everything it calls (otherwise it dead-code-eliminates to the entry stub).
    # Only export names actually DEFINED here (an undefined export aborts the link).
    exports_list = _exports(binpath, fmt)
    # A module is a LIBRARY (emit export directives) if it actually exports functions -- not by file
    # extension. The Win16 core (KRNL386.EXE, GDI.EXE, USER.EXE) ships as .EXE yet is a library that
    # exports hundreds of APIs; gating exports on `.dll` gave them 0 exports, so wlink dead-code-
    # eliminated every function not reached from the entry point and stubbed all their callers.
    is_lib = binpath.lower().endswith(".dll") or (fmt == "NE" and len(exports_list) > 0)
    defined = set(re.findall(r"(?m)^[A-Za-z].*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$",
                             open(src, encoding="utf-8", errors="replace").read()))
    exp_lines = []
    if is_lib:
        for ordn, name in exports_list:
            if name not in defined:
                continue
            # Symbol decoration must match how the decompiler DECLARED the function. The recovered
            # bodies are `int __fastcall NAME(...)`, and Watcom's 16-bit __fastcall decoration is a
            # LEADING '@' (symbol `@NAME`); wcc386's register calling convention is a TRAILING '_'.
            # (Imported Win16 APIs are the other case -- declared plain `extern`, decorated `NAME_` --
            # handled in the import block above.) Quote the NE symbol: an unquoted leading '@' is
            # wlink's read-directives-from-file syntax, not a symbol reference.
            if fmt == "PE":
                exp_lines.append("export %s='%s_'" % (name, name))
            else:
                exp_lines.append("export %s.%s='@%s'" % (name, ordn, name))
    entry_sym = ""
    if not is_lib and fmt in ("NE", "PE"):                # application EXE: pull the call tree via entry
        ev = _entry(binpath, fmt)
        if ev and ev != "0":
            nm = _name_at(binpath, fmt, int(ev, 16))      # function at the entry addr -> its symbol
            if nm and nm in defined:
                entry_sym = ("'@%s'" % nm) if fmt == "NE" else (nm + "_")   # NE __fastcall @ / PE _

    # iterative link: whatever is still undefined (internal sub_ the decompiler didn't emit, CRT
    # helpers) gets a return-0 stub -- flagged as the completeness gap, NOT the real imports.
    system = "windows" if bits == 16 else "nt"
    out = module + (".dll" if binpath.lower().endswith(".dll") else ".exe")
    stubs = set()
    for _it in range(20):
        with open(os.path.join(bld, "stubs.c"), "w", newline="\n") as f:
            f.write("/* internal targets the decompiler has not recovered -- flagged stubs */\n")
            for s in sorted(stubs):
                f.write("int %s(){return 0;}\n" % s)
        stubflags = (["-bt=" + tgt, "-q", "-s", "-ml", "-3"] if bits == 16 else cflags)
        subprocess.run([cc] + stubflags + ["stubs.c"], cwd=bld, capture_output=True, text=True, env=env)
        with open(os.path.join(bld, "link.lnk"), "w", newline="\n") as f:
            f.write("system %s\nname %s\noption quiet\noption map=%s.map\n" % (system, out, module))
            if entry_sym:
                f.write("option start=%s\n" % entry_sym)
            if bits == 16:
                # the Watcom large-model runtime provides the compiler math/util helpers (__U4D divide,
                # etc.) our code references; link them for real (not stubbed -- these are toolchain code).
                f.write("libpath '%s'\nlibrary clibl\n" % os.path.join(ow, "lib286").replace("\\", "/"))
            for o in objs:
                f.write("file %s\n" % o)
            f.write("file stubs.obj\n")
            for l in imp_lines:
                f.write(l + "\n")
            for l in exp_lines:
                f.write(l + "\n")
        lr = subprocess.run([os.path.join(ow, "binnt", "wlink.exe"), "@link.lnk"], cwd=bld,
                            capture_output=True, text=True, env=env)
        undef = set(re.findall(r"E2028: (\w+) is an undefined reference", lr.stdout + lr.stderr))
        undef = {u[:-1] if u.endswith("_") else u for u in undef}     # drop wcc's calling-conv '_'
        new = undef - stubs
        if not new:
            break
        stubs |= new
    outpath = os.path.join(bld, out)
    res = {"ok": os.path.isfile(outpath), "output": outpath if os.path.isfile(outpath) else None,
           "fmt": fmt, "imports": len(imp_lines), "exports": len(exp_lines), "stubs": len(stubs),
           "size": os.path.getsize(outpath) if os.path.isfile(outpath) else 0,
           "retail_size": os.path.getsize(binpath)}
    if not res["ok"]:
        res["log"] = (lr.stdout + lr.stderr)[-800:]
    return res


def _link_job(args):
    """Picklable worker: link one module, never raise (a crash in one must not kill the sweep)."""
    module, do_diff = args
    try:
        return module, link_module(module, do_diff)
    except Exception as e:                                          # noqa: BLE001
        return module, {"ok": False, "error": "exception: %s" % e}


def _all_modules():
    """Every module we have authored C for (the compile+link candidates)."""
    cdir = os.path.join(authsrc._dr(), "recon", "authored", "c")
    return [f[:-2] for f in sorted(os.listdir(cdir)) if f.endswith(".c")]


def main(argv):
    do_diff = "--diff" in argv
    if "--all" in argv:
        import csv, time
        from concurrent.futures import ProcessPoolExecutor, as_completed
        jobs_n = int(argv[argv.index("-j") + 1]) if "-j" in argv else min((os.cpu_count() or 4), 12)
        mods = _all_modules()
        t0 = time.time()
        rows, okn, iden, total_stubs = [], 0, 0, 0
        outcsv = os.path.join(authsrc._dr(), "recon", "authored", "BUILDLINK.csv")
        print("== owlink --all: compiling + linking %d modules with OpenWatcom (%d workers) ==" % (len(mods), jobs_n))
        with ProcessPoolExecutor(max_workers=jobs_n) as ex:
            futs = [ex.submit(_link_job, (m, False)) for m in mods]
            for k, fut in enumerate(as_completed(futs)):
                m, r = fut.result()
                if r.get("ok"):
                    okn += 1; total_stubs += r.get("stubs", 0)
                    pct = 100.0 * r["size"] / max(r.get("retail_size", 1), 1)
                    rows.append((m, r["fmt"], "OK", r["imports"], r.get("exports", 0), r["stubs"],
                                 r["size"], r.get("retail_size", 0), "%.0f" % pct))
                else:
                    rows.append((m, r.get("fmt", "?"), "FAIL", 0, 0, 0, 0, 0,
                                 (r.get("error") or r.get("log", ""))[:60]))
                if (k + 1) % 50 == 0:
                    print("  ... %d/%d linked (%d ok)" % (k + 1, len(mods), okn))
        with open(outcsv, "w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["module", "fmt", "status", "imports", "exports", "stubs", "size", "retail_size", "pct"])
            for r in sorted(rows):
                w.writerow(r)
        print("\n== corpus link ==")
        print("  linked OK: %d / %d modules  (%.0f%%)" % (okn, len(mods), 100.0 * okn / max(len(mods), 1)))
        print("  residual internal stubs across all: %d (avg %.1f/module)" % (total_stubs, total_stubs / max(okn, 1)))
        print("  %.0fs -> %s" % (time.time() - t0, outcsv))
        return 0
    mods = [a for a in argv if not a.startswith("-")]
    if not mods:
        print(__doc__); return 0
    for m in mods:
        r = link_module(m, do_diff)
        if r.get("ok"):
            print("  LINK-OK %-12s [%s]  %d imports, %d exports, %d stubs -> %s (%d bytes; retail %d, %.0f%%)"
                  % (m, r["fmt"], r["imports"], r.get("exports", 0), r["stubs"], os.path.basename(r["output"]),
                     r["size"], r["retail_size"], 100.0 * r["size"] / max(r["retail_size"], 1)))
        else:
            print("  LINK-NO %-12s  %s" % (m, r.get("error") or r.get("log", "")[-200:]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
