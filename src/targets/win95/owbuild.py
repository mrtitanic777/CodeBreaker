#!/usr/bin/env python3
"""owbuild.py -- the OpenWatcom build lane: compile a Win95 module's reconstructed
source into its binary with the FROM-SOURCE OpenWatcom toolchain (no Microsoft
tools). This is the forward half of the reconstruction premise -- the recovered C
must actually build -- and the compiler CodeBreaker will use for Win95 going forward.

    wt --target win95 owbuild <module> [--diff] [--keep]

The module directory (under the Win95 source tree, e.g. base/<module>) carries an
`owbuild.json` manifest describing its build:

    class   "ne" (16-bit, wcc)  |  "pe" (32-bit, wcc386)
    target  wcc -bt= value      ("windows" for Win16, "nt" for Win32)
    model   Watcom memory-model letter for -m<x> (ne only: s/m/c/l/h)
    system  wlink `system` directive (windows_dll | nt_dll)
    output  the DLL/EXE filename
    def     Microsoft .def whose EXPORTS are turned into wlink directives via wdef
    modules list of .c basenames to compile
    asm     (optional) list of .asm basenames assembled with wasm (e.g. thunks)
    link_extra (optional) list of raw wlink directive lines appended verbatim
              (e.g. flat-thunk IMPORT lines for a PE veneer)

Compile -> resolve exports (wdef) -> link (wlink). Reports pass/fail, the output
binary + size, and with --diff a byte comparison against the retail binary.
"""
import argparse
import json
import os
import shutil
import subprocess
import sys

from src.core import paths

_OW = os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2", "rel")
_PP = os.path.join(paths.repo_root(), "experiments", "premise-proof")
_WDEF = os.path.join(_PP, "wdef", "wdef.exe")
_WTHUNK = os.path.join(_PP, "wthunk", "wthunk.exe")


def _win95_root():
    """The one Win95 source tree (sibling of CodeBreaker under the data root)."""
    r = os.path.join(paths.data_root(), "Windows95")
    return r if os.path.isdir(r) else paths.data_root()


def _find_module_dir(module):
    """Locate <module>'s source dir under the Win95 tree (base/ first, then a walk)."""
    root = _win95_root()
    cand = os.path.join(root, "base", module)
    if os.path.isfile(os.path.join(cand, "owbuild.json")):
        return cand
    for dp, dns, fs in os.walk(root):
        if os.path.basename(dp).lower() == module.lower() and "owbuild.json" in fs:
            return dp
    return cand if os.path.isdir(cand) else None


def _ow_env(cls):
    """Environment for the from-source OpenWatcom release tree. Win16 build uses the
    h\\win headers; Win32 uses h\\nt."""
    env = dict(os.environ)
    env["NoDefaultCurrentDirectoryInExePath"] = ""     # OW runs its own tools from cwd
    env["WATCOM"] = _OW
    env["PATH"] = os.path.join(_OW, "binnt64") + os.pathsep + env.get("PATH", "")
    h = os.path.join(_OW, "h")
    if cls == "ne":
        env["INCLUDE"] = os.pathsep.join([h, os.path.join(h, "win")])
    else:
        env["INCLUDE"] = os.pathsep.join([h, os.path.join(h, "nt")])
    return env


def _tool(name):
    """Absolute path to an OW tool. Windows CreateProcess searches the CALLER's PATH,
    not the env we pass, so tools must be named by full path."""
    return os.path.join(_OW, "binnt64", name + ".exe")


def _run(cmd, cwd, env, log):
    if cmd and not os.path.isabs(cmd[0]) and not cmd[0].lower().endswith(".exe"):
        cmd = [_tool(cmd[0])] + cmd[1:]
    r = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True, text=True)
    log.append("$ " + " ".join(cmd))
    if r.stdout:
        log.append(r.stdout.rstrip())
    if r.stderr:
        log.append(r.stderr.rstrip())
    return r.returncode


def _ensure_tool(exe, bat_name):
    if os.path.exists(exe):
        return True
    bat = os.path.join(_PP, bat_name)
    if os.path.exists(bat):
        subprocess.run([bat], cwd=_PP, capture_output=True, text=True, shell=True)
    return os.path.exists(exe)


def _ensure_wdef():
    return _ensure_tool(_WDEF, "build_wdef.bat")


def build(module, do_diff=False, keep=False):
    src = _find_module_dir(module)
    if not src:
        return {"ok": False, "error": "no source dir for module %r under %s" % (module, _win95_root())}
    manifest = os.path.join(src, "owbuild.json")
    if not os.path.isfile(manifest):
        return {"ok": False, "error": "no owbuild.json in %s" % src}
    m = json.load(open(manifest, encoding="utf-8"))
    if not os.path.isdir(os.path.join(_OW, "binnt64")):
        return {"ok": False, "error": "OpenWatcom not built at %s" % _OW}

    cls = m.get("class", "ne")
    bld = os.path.join(src, "_build")
    os.makedirs(bld, exist_ok=True)
    for f in os.listdir(src):
        if f.lower().endswith((".c", ".h", ".asm", ".def", ".thk", ".rc")):
            shutil.copy2(os.path.join(src, f), bld)
    env = _ow_env(cls)
    log = []

    # --- compile C ---
    cc = "wcc" if cls == "ne" else "wcc386"
    bt = m.get("target", "windows" if cls == "ne" else "nt")
    cflags = ["-bt=" + bt, "-zW", "-q"] if cls == "ne" else ["-bt=" + bt, "-q"]
    if cls == "ne" and m.get("model"):
        cflags.insert(1, "-m" + m["model"])
    fails = []
    for mod in m.get("modules", []):
        if _run([cc] + cflags + [mod + ".c"], bld, env, log) != 0:
            fails.append(mod)
    # --- assemble asm (thunks etc.) ---
    for a in m.get("asm", []):
        if _run(["wasm", "-q", "-zcm=masm", "-zzo", a + ".asm"], bld, env, log) != 0:
            fails.append(a)
    # --- thunk: <thk> -> wthunk -> <asm> -> wasm -> <obj> (flat-thunk half) ---
    thunk_obj = None
    th = m.get("thunk")
    if th:
        if not _ensure_tool(_WTHUNK, "build_wthunk.bat"):
            return {"ok": False, "error": "wthunk.exe unavailable (build_wthunk.bat failed)", "log": log}
        asm = th.get("asm", "thunk.asm"); thunk_obj = th.get("obj", "thunk.obj")
        r = subprocess.run([_WTHUNK, "-o", asm, th["thk"]], cwd=bld, capture_output=True, text=True)
        log.append("$ wthunk -o %s %s" % (asm, th["thk"]))
        if r.returncode != 0:
            log.append(r.stdout + r.stderr)
            return {"ok": False, "error": "wthunk failed on " + th["thk"], "log": log, "dir": bld}
        wa = ["wasm", "-zcm=masm", "-zzo"] + ["-d" + d for d in th.get("defs", [])] + ["-fo=" + thunk_obj, asm]
        if _run(wa, bld, env, log) != 0:
            fails.append(asm)
    # --- resources: <rc> -> wrc -> <base>.res ---
    res_file = None
    if m.get("rc"):
        rc = m["rc"]; res_file = os.path.splitext(rc)[0] + ".res"
        if _run(["wrc", "-q", "-r", "-bt=" + bt, rc], bld, env, log) != 0:
            fails.append(rc)
    if fails:
        return {"ok": False, "error": "compile failed: " + ", ".join(fails), "log": log, "dir": bld}

    # --- exports: MS .def -> wlink directives via wdef ---
    exports_lnk = None
    if m.get("def"):
        if not _ensure_wdef():
            return {"ok": False, "error": "wdef.exe unavailable (build_wdef.bat failed)", "log": log}
        exports_lnk = "exports.lnk"
        # PE STDCALL decoration is resolved by feeding wdef the .obj symbols.
        objargs = [o if o.lower().endswith(".obj") else o + ".obj" for o in m.get("def_objs", [])]
        r = subprocess.run([_WDEF, os.path.join(src, m["def"])] + objargs,
                           cwd=bld, capture_output=True, text=True)
        with open(os.path.join(bld, exports_lnk), "w", newline="\n") as f:
            f.write(r.stdout)
        log.append("$ wdef %s %s  (%d export lines)" % (m["def"], " ".join(objargs), r.stdout.count("\n")))

    # --- link ---
    out = m.get("output", module + (".dll"))
    lnk = os.path.join(bld, "link.lnk")
    objs = list(m.get("modules", [])) + list(m.get("asm", []))
    if thunk_obj:
        objs.append(os.path.splitext(thunk_obj)[0])
    with open(lnk, "w", newline="\n") as f:
        f.write("system %s\n" % m.get("system", "windows_dll" if cls == "ne" else "nt_dll"))
        f.write("name %s\n" % out)
        f.write("option quiet\n")
        f.write("option map=%s.map\n" % os.path.splitext(out)[0])
        if res_file:
            f.write("option resource=%s\n" % res_file)
        f.write("file " + ",".join(objs) + "\n")
        for line in m.get("link_extra", []):
            f.write(line + "\n")
        if exports_lnk:
            f.write("@" + exports_lnk + "\n")
    _run(["wlink", "@link.lnk"], bld, env, log)

    outpath = os.path.join(bld, out)
    if not os.path.isfile(outpath):
        return {"ok": False, "error": "link produced no %s" % out, "log": log, "dir": bld}
    res = {"ok": True, "output": outpath, "size": os.path.getsize(outpath), "log": log, "dir": bld}

    if do_diff:
        res["diff"] = _bindiff(out, outpath)
    if not keep:
        pass  # leave _build for inspection; nothing to clean that's outside it
    return res


def _bindiff(name, built):
    """Byte comparison against the retail binary of the same name in the corpus."""
    roots = [os.path.join(_win95_root(), "design", "corpus", "files"),
             os.environ.get("WT_DATA_ROOT", "")]
    matches = []
    for corpus in roots:
        if not corpus or not os.path.isdir(corpus):
            continue
        for dp, _, fs in os.walk(corpus):
            for f in fs:
                if f.lower() == name.lower():
                    matches.append(os.path.join(dp, f))
        if matches:
            break
    # Prefer the INSTALLED copy (WINDOWS\SYSTEM) over setup/inf or other variants --
    # that is the DLL the OS actually loads and the reconstruction target.
    retail = None
    for m in matches:
        if "system" in m.lower().replace("\\", "/").split("/"):
            retail = m; break
    if not retail and matches:
        retail = matches[0]
    if not retail:
        return {"retail": None, "note": "no retail %s in corpus" % name}
    a = open(built, "rb").read(); b = open(retail, "rb").read()
    same = sum(1 for i in range(min(len(a), len(b))) if a[i] == b[i])
    return {"retail": retail, "built_size": len(a), "retail_size": len(b),
            "bytes_equal": same, "pct": round(100.0 * same / max(len(b), 1), 2)}


# CRT symbols the OpenWatcom runtime resolves at link -- never stubbed.
_CRT_SYMS = set()  # stub everything undefined -> self-contained scaffold link
_C_KW = {"if", "while", "for", "switch", "return", "sizeof", "do", "else", "goto",
         "case", "default", "break", "continue", "struct", "union", "int", "char",
         "void", "unsigned", "static", "const"}
import re as _re


def autolink(module, fmt, export_names, imports, cured_c, outdir):
    """Link an auto-cured single-.c module toward a DLL: resolve its imports from the
    import table, stub the engine intrinsics it references (indirect/VxDCall_/etc.),
    let the CRT resolve mem/str, and export its own .def. Surface-level -- proves the
    link closes; behavior needs curation. PE for now. Returns {ok, output|error}."""
    if fmt != "PE":
        return {"ok": False, "error": "autolink: PE only for now"}
    env = _ow_env("pe")
    text = open(cured_c, encoding="utf-8", errors="replace").read()
    base = module.split(".")[0]
    obj = base + ".obj"
    log = []
    # -s: no stack-overflow probe (drops the __CHK reference the CRT would supply),
    # keeping the scaffold self-contained (no CRT lib needed).
    ccflags = ["-bt=nt", "-q", "-wcd=138", "-s"]
    if _run([_tool("wcc386")] + ccflags + [os.path.basename(cured_c)],
            outdir, env, log) != 0:
        return {"ok": False, "error": "compile failed", "log": log}

    # symbols the code CALLS, minus those it defines, minus imports, CRT, keywords.
    called = set(_re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    defined = set(_re.findall(r"(?m)^int ([A-Za-z_]\w*)\(\)", text))
    import_names = set()
    imp_lines = []
    for lib in imports:
        libbase = lib["name"].split(".")[0].upper()
        for s in lib.get("symbols", []):
            if s.get("name"):
                # wcc386's default register calling convention appends '_' to the
                # symbol, so the object references KERNEL32_lstrcpyA_ -- match that.
                internal = "%s_%s_" % (libbase, s["name"])
                import_names.add("%s_%s" % (libbase, s["name"]))
                # A C++ mangled import name (e.g. ??2@YAPAXI@Z = operator new) carries
                # '?'/'@' that wlink's directive parser splits on -> E2093 "cannot open
                # YAPAXI@Z_". Quote the internal symbol so it stays one token. The recovered
                # C can only reference valid-identifier imports, so a mangled one is an
                # (unused) available import -- quoting keeps it harmless. Only names with
                # non-identifier chars are quoted, so every KERNEL32_*/plain import line is
                # byte-for-byte unchanged and the modules that already link are unaffected.
                iname = "'%s'" % internal if _re.search(r"[^A-Za-z0-9_$]", internal) else internal
                imp_lines.append("import %s '%s'.'%s'" % (iname, libbase, s["name"]))
    stubs = sorted(called - defined - import_names - _CRT_SYMS - _C_KW)
    # Floating-point runtime markers: a module that uses FP makes wcc386 reference the
    # data symbols _fltused_ (FP-used flag) and __8087 (387-present flag), normally
    # supplied by the Watcom math library. autolink stays CRT/math-lib-free, so define
    # them as inert data when the compiled object references them -- C `fltused_`/`_8087`
    # get the leading-underscore prepend -> exactly _fltused_ / __8087. Gated on the
    # reference so FP-free modules (and the ones that already link) emit nothing new.
    try:
        _mo = open(os.path.join(outdir, obj), "rb").read()
    except OSError:
        _mo = b""
    fp_defs = []
    if b"_fltused_" in _mo or b"fltused" in _mo:
        fp_defs.append("int fltused_ = 0x9875;")
    if b"__8087" in _mo or b"8087" in _mo:
        fp_defs.append("int _8087 = 0;")
    with open(os.path.join(outdir, "stubs.c"), "w", newline="\n") as f:
        f.write("/* auto stubs for engine intrinsics + unresolved leaf refs */\n")
        for d in fp_defs:
            f.write(d + "\n")
        for s in stubs:
            f.write("int %s(){return 0;}\n" % s)
    if _run([_tool("wcc386"), "-bt=nt", "-q", "-wcd=138", "-s", "stubs.c"], outdir, env, log) != 0:
        return {"ok": False, "error": "stub compile failed", "log": log}

    # exports: the auto-cured functions are register-calling (symbol NAME_), so alias
    # each real export name to its decorated symbol -- `export DllMain=DllMain_`. Only
    # export names that are actually defined as functions in this object.
    exp_lines = []
    for name in export_names:
        if name in defined:
            exp_lines.append("export %s=%s_" % (name, name))

    out = base + ".dll"
    with open(os.path.join(outdir, "link.lnk"), "w", newline="\n") as f:
        f.write("system nt_dll\nname %s\noption quiet\noption map=%s.map\n" % (out, base))
        f.write("file %s\nfile stubs.obj\n" % obj)
        for l in imp_lines:
            f.write(l + "\n")
        for l in exp_lines:
            f.write(l + "\n")
    _run([_tool("wlink"), "@link.lnk"], outdir, env, log)
    outpath = os.path.join(outdir, out)
    if os.path.isfile(outpath):
        return {"ok": True, "output": outpath, "size": os.path.getsize(outpath),
                "stubs": len(stubs), "imports": len(imp_lines)}
    return {"ok": False, "error": "link produced no output", "log": log[-6:]}


def autolink_exe(module, fmt, imports, cured_c, outdir):
    """Link an auto-cured module toward an EXECUTABLE (not a DLL): `system nt`
    (PE) / `system windows` (NE) / `system dos` (MZ), a synthesized entry point
    (the decompiled bag of sub_* has no real one), imports resolved from the table,
    everything else stubbed, no export table. Surface-level -- proves the link
    closes; behavior needs curation. Returns {ok, output|error}."""
    sysmap = {"PE": ("nt", "wcc386", ["-bt=nt", "-q", "-wcd=138", "-s"], "pe"),
              "NE": ("windows", "wcc", ["-bt=windows", "-mm", "-q", "-s"], "ne"),
              "MZ": ("dos", "wcc", ["-bt=dos", "-ms", "-q", "-s"], "ne")}
    if fmt not in sysmap:
        return {"ok": False, "error": "autolink_exe: unsupported %s" % fmt}
    system, cc, ccflags, envcls = sysmap[fmt]
    env = _ow_env(envcls)
    text = open(cured_c, encoding="utf-8", errors="replace").read()
    base = module.split(".")[0]
    obj = base + ".obj"
    log = []
    if _run([_tool(cc)] + ccflags + [os.path.basename(cured_c)], outdir, env, log) != 0:
        return {"ok": False, "error": "compile failed", "log": log}

    called = set(_re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    defined = set(_re.findall(r"(?m)^int ([A-Za-z_]\w*)\(\)", text))
    import_names = set()
    imp_lines = []
    if fmt == "PE":                         # NE/MZ imports resolve differently; scaffold stubs them
        for lib in imports:
            libbase = lib["name"].split(".")[0].upper()
            for s in lib.get("symbols", []):
                if s.get("name"):
                    internal = "%s_%s_" % (libbase, s["name"])
                    import_names.add("%s_%s" % (libbase, s["name"]))
                    imp_lines.append("import %s '%s'.'%s'" % (internal, libbase, s["name"]))

    # a synthesized entry point: the decompiled functions are a bag of sub_*, so the
    # scaffold supplies its own start symbol and points the linker at it.
    dec = "_" if fmt == "PE" else "_"   # wcc/wcc386 register calling both append '_'
    stubs = sorted(called - defined - import_names - _CRT_SYMS - _C_KW)
    with open(os.path.join(outdir, "stubs.c"), "w", newline="\n") as f:
        f.write("/* auto stubs + synthesized entry for the exe scaffold */\n")
        f.write("int resexe_start(){return 0;}\n")
        for s in stubs:
            f.write("int %s(){return 0;}\n" % s)
    if _run([_tool(cc)] + ccflags + ["stubs.c"], outdir, env, log) != 0:
        return {"ok": False, "error": "stub compile failed", "log": log}

    out = base + ".exe"
    with open(os.path.join(outdir, "link.lnk"), "w", newline="\n") as f:
        f.write("system %s\nname %s\noption quiet\noption map=%s.map\n" % (system, out, base))
        f.write("option start=resexe_start%s\n" % dec)
        f.write("file %s\nfile stubs.obj\n" % obj)
        for l in imp_lines:
            f.write(l + "\n")
    _run([_tool("wlink"), "@link.lnk"], outdir, env, log)
    outpath = os.path.join(outdir, out)
    if os.path.isfile(outpath):
        return {"ok": True, "output": outpath, "size": os.path.getsize(outpath),
                "stubs": len(stubs), "imports": len(imp_lines)}
    return {"ok": False, "error": "link produced no output", "log": log[-8:]}


def main(argv):
    ap = argparse.ArgumentParser(prog="owbuild", description=__doc__.splitlines()[0])
    ap.add_argument("module")
    ap.add_argument("--diff", action="store_true", help="byte-compare against retail")
    ap.add_argument("--keep", action="store_true", help="(reserved) keep intermediates")
    a = ap.parse_args(argv)
    r = build(a.module, do_diff=a.diff, keep=a.keep)
    if r.get("log"):
        print("\n".join(r["log"]))
    if not r["ok"]:
        print("\nBUILD FAILED: %s" % r.get("error"))
        if r.get("dir"):
            print("  intermediates: %s" % r["dir"])
        sys.exit(1)
    print("\nBUILD OK: %s  (%d bytes)" % (r["output"], r["size"]))
    if "diff" in r:
        d = r["diff"]
        if d.get("retail"):
            print("  vs retail %s: built %d / retail %d bytes, %d equal (%.2f%%)"
                  % (os.path.basename(d["retail"]), d["built_size"], d["retail_size"],
                     d["bytes_equal"], d["pct"]))
            print("  (byte-diff is a weak signal; the standard is builds + behaves-as-original)")
        else:
            print("  diff: %s" % d.get("note"))


if __name__ == "__main__":
    main(sys.argv[1:])
