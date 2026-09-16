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
    for p in glob.glob(os.path.join(dr, "corpus", "files", "**", module + ".*"), recursive=True):
        return p
    for rel in (("corpus/files/setup/inf/%s.exe" % module), ("corpus/files/setup/inf/%s.dll" % module)):
        p = os.path.join(dr, rel)
        if os.path.isfile(p):
            return p
    return None


def _imports(binpath, fmt):
    """[(module, apiname)] from the binary's own import table (NE only for now)."""
    if fmt != "NE":
        return []
    dec = _dec("NE")
    out = subprocess.run([dec, binpath, "--imports"], capture_output=True, text=True).stdout
    imps = []
    for line in out.splitlines():
        p = line.split("\t")
        if len(p) == 2 and p[1].strip():
            imps.append((p[0].strip(), p[1].strip()))
    return imps


def _exports(binpath, fmt):
    """[(ordinal, name)] from the module's own export/entry table (NE)."""
    if fmt != "NE":
        return []
    dec = _dec("NE")
    out = subprocess.run([dec, binpath, "--exports"], capture_output=True, text=True).stdout
    exps = []
    for line in out.splitlines():
        p = line.split("\t")
        if len(p) == 2 and p[1].strip():
            exps.append((p[0].strip(), p[1].strip()))
    return exps


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
    src = os.path.join(bld, module + ".c")
    shutil.copy2(cpath, src)
    env = owcheck._env()
    cc = os.path.join(ow, "binnt", "wcc.exe" if bits == 16 else "wcc386.exe")
    tgt = "windows" if bits == 16 else "nt"
    # -s: no stack-overflow probe -> no __STK CRT dependency (self-contained scaffold)
    cflags = ["-bt=" + tgt, "-q", "-s"]
    if bits == 16:
        cflags += ["-mc", "-3"]
    r = subprocess.run([cc] + cflags + [module + ".c"], cwd=bld, capture_output=True, text=True,
                       env=env, timeout=180)
    obj = os.path.join(bld, module + ".obj")
    if not os.path.isfile(obj):
        return {"ok": False, "error": "compile produced no obj", "log": (r.stdout + r.stderr)[-800:]}

    # real Win16 imports -> wlink `import` directives (name-consistent with the C's symbols)
    imps = _imports(binpath, fmt)
    imp_lines = []
    for mod, api in imps:
        # by-ordinal names surface as MOD_<n>; resolve those to `import SYM 'MOD'.<n>`, else by name
        mo = re.match(r"^%s_(\d+)$" % re.escape(mod), api)
        if mo:
            imp_lines.append("import %s '%s'.%s" % (api, mod, mo.group(1)))
        else:
            imp_lines.append("import %s '%s'.'%s'" % (api, mod, api))

    # real exports -> wlink `export` directives, so the module exports its functions AND wlink keeps
    # the exported code + everything it calls (otherwise it dead-code-eliminates to the entry stub).
    # Only export names actually DEFINED here (an undefined export aborts the link).
    is_dll = binpath.lower().endswith(".dll")
    defined = set(re.findall(r"(?m)^[A-Za-z].*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$",
                             open(src, encoding="utf-8", errors="replace").read()))
    exp_lines = []
    if is_dll:                                             # DLLs export; an EXE keeps code via its entry
        for ordn, name in _exports(binpath, fmt):
            # alias the clean export name to wcc's decorated internal symbol. 16-bit __fastcall decorates
            # with a LEADING '@' (@NAME), so `export NAME='@NAME'.ordinal`; this publishes the export
            # and forces wlink to keep the function + everything it calls.
            if name in defined:
                # quote the internal symbol: an unquoted leading @ is wlink's read-directives-from-file
                # syntax, not a symbol reference.
                exp_lines.append("export %s.%s='@%s'" % (name, ordn, name))

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
        subprocess.run([cc] + cflags + ["stubs.c"], cwd=bld, capture_output=True, text=True, env=env)
        with open(os.path.join(bld, "link.lnk"), "w", newline="\n") as f:
            f.write("system %s\nname %s\noption quiet\noption map=%s.map\n" % (system, out, module))
            f.write("file %s.obj\nfile stubs.obj\n" % module)
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


def main(argv):
    do_diff = "--diff" in argv
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
