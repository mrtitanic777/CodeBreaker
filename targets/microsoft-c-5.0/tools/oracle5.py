#!/usr/bin/env python3
"""oracle5.py -- Microsoft C 5.0 compile oracle, driven headless under DOSBox-X.

The target is its own oracle: we run the *extracted* MS C 5.0 (CL -> C1/C2/C3) under
DOSBox-X to compile probe C, giving byte-exact ground truth for its own codegen (and a
recompile-verify floor). Mirrors CodeBreaker's oracle16.py pattern.

    python oracle5.py init                 # (re)build the oracle tree from extracted/
    python oracle5.py compile <file.c>     # cl /AS /c  -> <stem>.OBJ   (prints log)
    python oracle5.py build   <file.c>     # cl /AS     -> <stem>.EXE   (compile+link)
    python oracle5.py cc "<C source>"      # compile a string, dump the .OBJ path

Config: DOSBox-X path from CodeBreaker/config.json (dosbox_x) or $DBX.
"""
import os, sys, json, shutil, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ORACLE = os.path.join(ROOT, "work", "oracle")
EXTRACT = os.path.join(ROOT, "extracted")
CB = r"C:\Users\mrtit\Documents\GitHub\CodeBreaker"

def dbx_path():
    p = os.environ.get("DBX")
    if p and os.path.exists(p): return p
    try:
        cfg = json.load(open(os.path.join(CB, "config.json")))
        if cfg.get("dosbox_x") and os.path.exists(cfg["dosbox_x"]): return cfg["dosbox_x"]
    except Exception: pass
    raise SystemExit("DOSBox-X not found; set $DBX or CodeBreaker/config.json dosbox_x")

def init():
    os.makedirs(os.path.join(ORACLE, "INCLUDE"), exist_ok=True)
    os.makedirs(os.path.join(ORACLE, "LIB"), exist_ok=True)
    os.makedirs(os.path.join(ORACLE, "TMP"), exist_ok=True)
    def cp(src, dst):
        s = os.path.join(EXTRACT, src)
        if os.path.exists(s): shutil.copy(s, os.path.join(ORACLE, dst))
    for f in ("SETUP/C1.EXE","COMPILER/C2.EXE","COMPILER/C3.EXE","SETUP/CL.EXE",
              "UTILITES/LINK.EXE","UTILITES/LIB.EXE","COMPILER/C1.ERR","COMPILER/C23.ERR",
              "SETUP/CL.ERR","ADDNUTIL/C1L.EXE"):
        cp(f, os.path.basename(f))
    inc = os.path.join(EXTRACT, "INCLIBSM", "INCLUDE")
    for h in os.listdir(inc):
        s = os.path.join(inc, h)
        if os.path.isfile(s): shutil.copy(s, os.path.join(ORACLE, "INCLUDE", h))
        elif os.path.isdir(s):
            shutil.copytree(s, os.path.join(ORACLE, "INCLUDE", h), dirs_exist_ok=True)
    for lib in ("INCLIBSM/SLIBC.LIB","INCLIBSM/SLIBFP.LIB","LIB_M_C/SLIBFA.LIB"):
        cp(lib, os.path.join("LIB", os.path.basename(lib)))
    libd = os.path.join(EXTRACT, "INCLIBSM", "LIB")
    if os.path.isdir(libd):
        for f in os.listdir(libd):
            shutil.copy(os.path.join(libd, f), os.path.join(ORACLE, "LIB", f))
    # Merge the small-model combined runtime SLIBCE.LIB (base + FP + emulator) that CL's
    # default -defaultlib record names, so compile+link (build) resolves cleanly.
    if not os.path.exists(os.path.join(ORACLE, "SLIBCE.LIB")):
        _run([r'copy LIB\SLIBC.LIB SLIBCE.LIB >NUL',
              r'lib SLIBCE.LIB +LIB\SLIBFP.LIB +LIB\EM.LIB; >NUL'])
    print("oracle tree ready at", ORACLE,
          "(SLIBCE.LIB present)" if os.path.exists(os.path.join(ORACLE,"SLIBCE.LIB")) else "")

def _run(cmds, timeout=120):
    conf = os.path.join(ORACLE, "dbx.conf")
    lines = ["[cpu]", "core=auto", "cputype=386", "[autoexec]",
             'mount c "%s"' % ORACLE, "c:",
             "path C:\\;C:\\",                       # avoid trailing-backslash EOL hazards
             "set INCLUDE=C:\\INCLUDE", "set LIB=C:\\LIB", "set TMP=C:\\TMP"]
    lines += cmds + ["exit", ""]
    with open(conf, "w", newline="\r\n") as f: f.write("\n".join(lines))
    subprocess.run([dbx_path(), "-conf", conf, "-fastlaunch"],
                   capture_output=True, timeout=timeout,
                   env=dict(os.environ, SDL_VIDEODRIVER="dummy"))

def compile_file(cfile, link=False):
    stem = os.path.splitext(os.path.basename(cfile))[0]
    src = os.path.join(ORACLE, os.path.basename(cfile))
    if os.path.abspath(cfile) != os.path.abspath(src):
        shutil.copy(cfile, src)
    for ext in (".OBJ",".EXE",".LOG"):
        p = os.path.join(ORACLE, stem+ext)
        if os.path.exists(p): os.remove(p)
    cmd = ("cl /AS %s%s.c >%s.LOG" % ("" if link else "/c ", stem, stem))
    _run([cmd])
    log = ""
    lp = os.path.join(ORACLE, stem+".LOG")
    if os.path.exists(lp): log = open(lp, "rb").read().decode("latin1").replace("\r","")
    out = os.path.join(ORACLE, stem + (".EXE" if link else ".OBJ"))
    return (out if os.path.exists(out) else None), log

if __name__ == "__main__":
    if len(sys.argv) < 2: print(__doc__); sys.exit(1)
    cmd = sys.argv[1]
    if cmd == "init": init()
    elif cmd in ("compile","build"):
        out, log = compile_file(sys.argv[2], link=(cmd=="build"))
        print(log.strip())
        print(("OK -> " + out) if out else "FAILED (no output)")
        sys.exit(0 if out else 1)
    elif cmd == "cc":
        tmp = os.path.join(ORACLE, "_probe.c")
        open(tmp, "w", newline="\r\n").write(sys.argv[2])
        out, log = compile_file(tmp)
        print(log.strip()); print(out or "FAILED")
    else: print(__doc__)
