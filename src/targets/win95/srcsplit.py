#!/usr/bin/env python3
r"""srcsplit.py -- split a single-file authored module into a proper multi-file source tree.

A Win95 DLL was never one translation unit -- it was dozens of functionally-grouped .c files the
linker combined. Our authored/c/<module>.c is the whole DLL in one file, which is both unfaithful and
the reason the largest modules don't build (wcc doesn't scale to a 120K-line unit; a 16-bit module
overruns the 64K code segment). This tool reconstructs a plausible source layout:

  authored/c/mapi.c  ->  authored/tree/mapi/mapi.h          (shared: externs, prototypes, structs)
                         authored/tree/mapi/mapi_logon.c    (#include "mapi.h" + a functional group)
                         authored/tree/mapi/mapi_addr.c
                         ...

Grouping is inferred (the original boundaries are gone): functions are taken in address order (the
link order approximates source-file order) and cut into files at low call-density boundaries, capped
in size; each file is named after the dominant export/name family in it. Names are labeled best-guess
reconstructions, per the reconstruction standard. Every emitted .c #includes the one module header so
cross-file calls resolve, and each is verified to compile with OpenWatcom.

    wt --target win95 srcsplit <module>        # split one authored module
    wt --target win95 srcsplit --all [-j N]    # split every authored/c/*.c
"""
import os
import re
import sys
import subprocess

from src.core import paths
from src.targets.win95 import authsrc

_TARGET_FUNCS = 60          # aim ~this many functions per source file (a typical .c)
_MAX_LINES = 6000           # hard cap per file so no unit is oversized for the compiler/segment


def _dr():
    return authsrc._dr()


def _parse(text):
    """Split authored C into (preamble_lines, structs, funcs). funcs = [(name, addr, [body lines])];
    preamble = the forward-decl block; structs = list of struct-def blocks (moved to the header)."""
    lines = text.split("\n")
    # the make_compilable forward-decl block sits at the very top (externs + prototypes)
    preamble = []
    i = 0
    if lines and lines[0].startswith("/*"):
        while i < len(lines) and not lines[i].startswith("//"):   # header comment + fwd-decls
            preamble.append(lines[i]); i += 1
    structs, funcs = [], []
    n = len(lines)
    fnstarts = {b[1]: b for b in authsrc._fn_blocks(lines)}    # sig line idx -> block
    while i < n:
        ln = lines[i]
        sm = re.match(r"^\s*struct\s+(\w+)\s*\{", ln)
        if sm:
            depth, j = 0, i
            while j < n:
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0 and "}" in lines[j]:
                    break
                j += 1
            structs.append("\n".join(lines[i:j + 1]))
            i = j + 1
            continue
        if i in fnstarts:
            _s, sig, opn, close = fnstarts[i]
            # the address comment is one of the few lines above the signature
            addr = None
            for k in range(max(0, i - 4), i):
                am = re.search(r"@ 0x([0-9A-Fa-f]+)", lines[k])
                if am:
                    addr = int(am.group(1), 16)
            m = authsrc._FN_OPEN.match(lines[sig].rstrip())
            name = m.group(2) if m else ("fn_%d" % i)
            # include the leading comment lines (from after the previous block) as part of the body
            body = lines[i:close + 1]
            funcs.append((name, addr if addr is not None else 0, body))
            i = close + 1
            continue
        i += 1
    return preamble, structs, funcs


# Conventional short (<=8 char) source-file stems by functional area, matching how Microsoft actually
# named Win16/Win32 sources under the DOS 8.3 limit (documented in Pietrek/Schulman; mirrored by
# ReactOS): a file is named for its subsystem, not a single long API. Keyed by an API-name substring,
# longest-match-first so `imagelist` beats `image`.
_STEM_MAP = {
    # comctl32 controls
    "imagelist": "imglist", "statuswindow": "status", "status": "status", "toolbar": "toolbar",
    "listview": "listview", "treeview": "treeview", "updown": "updown", "progress": "progress",
    "tooltip": "tooltip", "trackbar": "trackbar", "propertysheet": "propsht", "propsheet": "propsht",
    "header": "header", "animate": "animate", "hotkey": "hotkey", "tabcontrol": "tab", "rebar": "rebar",
    "flatsb": "flatsb", "drawinsert": "draw", "commoncontrol": "comctl",
    # kernel32
    "virtualalloc": "virtual", "virtualfree": "virtual", "virtualquery": "virtual", "virtual": "virtual",
    "heap": "heap", "globalalloc": "heap", "localalloc": "heap", "criticalsection": "sync",
    "semaphore": "sync", "mutex": "sync", "waitforsingle": "sync", "waitformultiple": "sync",
    "event": "sync", "thread": "thread", "createprocess": "process", "process": "process",
    "loadlibrary": "module", "getprocaddress": "module", "module": "module", "freelibrary": "module",
    "createfile": "file", "readfile": "file", "writefile": "file", "findfirst": "find",
    "findclose": "find", "findnext": "find", "tape": "tape", "comm": "comm", "atom": "atom",
    "profile": "profile", "getprivateprofile": "profile", "locale": "nls", "codepage": "nls",
    "multibyte": "nls", "widechar": "nls", "lstr": "lstr", "console": "console", "toolhelp": "toolhelp",
    "thunk": "thunk", "mapls": "thunk", "dosmem": "dosmem", "vxd": "vxdcall", "getlasterror": "error",
    "setlasterror": "error", "getcommand": "cmdline", "environment": "environ",
    # user32
    "createwindow": "window", "defwindow": "window", "showwindow": "window", "destroywindow": "window",
    "window": "window", "dialog": "dialog", "dlgitem": "dialog", "menu": "menu", "editwnd": "edit",
    "listbox": "listbox", "combo": "combo", "button": "button", "static": "static", "scroll": "scroll",
    "caret": "caret", "cursor": "cursor", "icon": "icon", "hook": "hook", "clipboard": "clipbrd",
    "message": "message", "dispatchmessage": "message", "peekmessage": "message", "sendmessage": "message",
    "keyboard": "keybd", "getkey": "keybd", "mouse": "mouse", "timer": "timer", "getprop": "prop",
    "setprop": "prop", "registerclass": "class", "wndclass": "class", "rect": "rect", "paint": "paint",
    "beginpaint": "paint", "messagebox": "msgbox", "systemmetrics": "sysmet",
    # gdi32
    "createdc": "dc", "deletedc": "dc", "getdc": "dc", "bitblt": "bitblt", "stretchblt": "bitblt",
    "bitmap": "bitmap", "createbrush": "brush", "brush": "brush", "createpen": "pen", "pen": "pen",
    "createfont": "font", "font": "font", "textout": "text", "drawtext": "text", "region": "region",
    "rgn": "region", "path": "path", "metafile": "metafile", "palette": "palette", "getpixel": "pixel",
    "setpixel": "pixel", "polygon": "shape", "ellipse": "shape", "rectangle": "shape", "arc": "arc",
    "escape": "print", "startdoc": "print", "enumfont": "enumfont",
    # cross-cutting / other DLLs
    "mapi": "mapi", "ole": "ole", "storage": "storage", "moniker": "moniker", "rpc": "rpc",
    "crypt": "crypt", "socket": "socket", "wsa": "socket", "http": "http", "internet": "wininet",
    "reg": "registry", "shell": "shell", "sh": "shell", "commdlg": "commdlg", "getopenfile": "filedlg",
    "printdlg": "printdlg", "choosefont": "fontdlg", "choosecolor": "colordlg",
}
_VERBS = ("create", "destroy", "delete", "free", "alloc", "get", "set", "init", "open", "close",
          "is", "register", "unregister", "new", "make", "begin", "end", "load", "enum", "query",
          "internal", "default", "dll", "helper", "proc", "do", "on", "handle", "the")


def _family(name):
    """A functional-family key for grouping: the leading run of a name before its address/dup suffix.
    (Used for cohesion in grouping; final file naming goes through _stem for the conventional form.)"""
    base = re.sub(r"_r\d+$", "", name)                       # drop dup-rename suffix
    base = re.sub(r"_?[0-9A-Fa-f]{4,}$", "", base)           # drop trailing address
    base = base.strip("_")
    if not base or base.lower() in ("sub", "helper", "loc", "fn"):
        return "core"
    return base.lower()


def _stem(name):
    """A conventional short (<=8 char) source-file stem for a function's functional area, the way
    Microsoft named the file (status.c, heap.c, imglist.c) -- not the full API name. Returns '' if the
    name is an unnamed placeholder (caller groups those as util/core)."""
    fam = _family(name)
    if fam == "core":
        return ""
    key = re.sub(r"[^a-z0-9]", "", fam)
    for k in sorted(_STEM_MAP, key=len, reverse=True):       # longest substring wins
        if k in key:
            return _STEM_MAP[k]
    # fallback: strip a leading verb, drop a trailing 'a'/'w' ANSI/wide tag, cap at 8 chars
    s = key
    for v in sorted(_VERBS, key=len, reverse=True):
        if s.startswith(v) and len(s) > len(v) + 2:
            s = s[len(v):]; break
    s = re.sub(r"(a|w)$", "", s) if len(s) > 4 else s
    return s[:8] or ""


def _group(funcs):
    """Partition funcs (in address order) into files. Cut at a size target, preferring a boundary where
    the family changes (a proxy for a source-file boundary). Returns [(filename_stem, [funcs])]."""
    funcs = sorted(funcs, key=lambda f: f[1])                # address order ~ link/source order
    groups, cur = [], []
    for f in funcs:
        cur.append(f)
        lines_so_far = sum(len(x[2]) for x in cur)
        big = len(cur) >= _TARGET_FUNCS or lines_so_far >= _MAX_LINES
        # cut at a family change once we're near the target, or hard at the caps
        fam_change = len(cur) >= 2 and _family(cur[-1][0]) != _family(cur[-2][0])
        if (big and fam_change) or lines_so_far >= _MAX_LINES or len(cur) >= _TARGET_FUNCS * 2:
            if fam_change and lines_so_far < _MAX_LINES:
                cur.pop()                                    # start the new family in the next file
                groups.append(cur); cur = [f]
            else:
                groups.append(cur); cur = []
    if cur:
        groups.append(cur)
    # name each group after the functional AREA it is really about, using the conventional short
    # (<=8 char) stem -- the most common recovered stem in the group (placeholders don't vote). An
    # all-placeholder group is `util`. Collisions get a numeric suffix.
    import collections
    named, seen = [], {}
    for g in groups:
        stems = collections.Counter(s for s in (_stem(x[0]) for x in g) if s)
        stem = stems.most_common(1)[0][0] if stems else "util"
        seen[stem] = seen.get(stem, 0) + 1
        if seen[stem] > 1:
            stem = ("%s%d" % (stem[:7], seen[stem]))[:8]
        named.append((stem, g))
    return named


def split_module(cpath, module, outroot):
    """Split one authored .c into <outroot>/<module>/: a shared header + grouped source files."""
    text = open(cpath, encoding="utf-8", errors="replace").read()
    preamble, structs, funcs = _parse(text)
    if not funcs:
        return None
    moddir = os.path.join(outroot, module)
    if os.path.isdir(moddir):                                # clear stale pieces from a prior split
        for f in os.listdir(moddir):
            if f.endswith((".c", ".h")):
                os.remove(os.path.join(moddir, f))
    os.makedirs(moddir, exist_ok=True)
    hdr = module + ".h"
    guard = "_%s_H" % re.sub(r"[^A-Za-z0-9]", "_", module).upper()
    # shared header: provenance + struct defs + externs + every prototype (so any file calls any fn)
    hlines = ["/* %s.h -- shared declarations for the reconstructed %s source tree (CodeBreaker)." % (module, module),
              " * Authored multi-file layout: functional grouping is an evidence-based reconstruction;" ,
              " * the original file boundaries are not recoverable from the binary. */",
              "#ifndef %s" % guard, "#define %s" % guard, ""]
    hlines += structs + [""]
    # only real declarations from the preamble (extern/prototype lines end in ';'); never comment prose
    hlines += [ln for ln in preamble if ln.rstrip().endswith(";")]
    hlines += ["", "#endif"]
    open(os.path.join(moddir, hdr), "w", encoding="utf-8", errors="replace").write("\n".join(hlines))

    groups = _group(funcs)
    files = []
    for stem, g in groups:
        fn = "%s.c" % stem                                   # bare short name inside <module>/ (no prefix)
        out = ['/* %s -- %s (functional group, reconstructed). */' % (fn, module),
               '#include "%s"' % hdr, ""]
        for name, addr, body in g:
            out += body + [""]
        open(os.path.join(moddir, fn), "w", encoding="utf-8", errors="replace").write("\n".join(out))
        files.append(fn)
    return {"dir": moddir, "header": hdr, "files": files, "funcs": len(funcs)}


def _verify_one(job):
    """Compile every .c piece of a split module against its header; return (module, ok, total)."""
    from src.targets.win95 import owcheck
    moddir, bits = job
    ow = _ow_bin = os.path.join(_ow(), "binnt")
    cc = os.path.join(_ow_bin, "wcc.exe" if bits == 16 else "wcc386.exe")
    env = owcheck._env()
    tgt = "windows" if bits == 16 else "nt"
    ok = tot = 0
    for f in sorted(os.listdir(moddir)):
        if not f.endswith(".c"):
            continue
        tot += 1
        args = [cc, os.path.join(moddir, f), "-bt=" + tgt, "-q", "-i=" + moddir,
                "-fo=" + os.path.join(os.environ.get("TEMP", "/tmp"), "v_%d.o" % os.getpid())]
        if bits == 16:
            args[1:1] = ["-mc", "-3"]
        try:
            r = subprocess.run(args, capture_output=True, text=True, env=env, timeout=90)
            if r.returncode == 0 and "Error!" not in (r.stdout + r.stderr):
                ok += 1
        except subprocess.TimeoutExpired:
            pass
    return (os.path.basename(moddir), ok, tot)


def _ow():
    return os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2", "rel")


def main(argv):
    outroot = os.path.join(_dr(), "recon", "authored", "tree")
    cdir = os.path.join(_dr(), "recon", "authored", "c")
    do_all = verify = False
    jobs_n = min((os.cpu_count() or 4), 12)
    mods = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--all":
            do_all = True
        elif a == "--verify":
            verify = True
        elif a in ("--jobs", "-j") and i + 1 < len(argv):
            jobs_n = int(argv[i + 1]); i += 1
        elif not a.startswith("-"):
            mods.append(a)
        i += 1

    if verify:
        # compile every split piece across the tree, in parallel; report per-module and totals.
        import json
        cache = json.load(open(os.path.join(_dr(), "recon", "authored", ".authcache.json")))
        fmt_by = {os.path.basename(k).split(".")[0].lower(): v.get("fmt", "PE") for k, v in cache.items()}
        moddirs = [d for d in sorted(os.listdir(outroot)) if os.path.isdir(os.path.join(outroot, d))]
        jobs = [(os.path.join(outroot, d), 32 if fmt_by.get(d, "PE") == "PE" else 16) for d in moddirs]
        tok = ttot = full = 0
        from concurrent.futures import ProcessPoolExecutor, as_completed
        rows = []
        with ProcessPoolExecutor(max_workers=jobs_n) as ex:
            futs = [ex.submit(_verify_one, j) for j in jobs]
            for k, fut in enumerate(as_completed(futs)):
                mod, ok, tot = fut.result(); rows.append((mod, ok, tot))
                tok += ok; ttot += tot; full += 1 if ok == tot and tot else 0
                if (k + 1) % 100 == 0:
                    print("  ... %d/%d modules" % (k + 1, len(jobs)))
        with open(os.path.join(outroot, "VERIFY.csv"), "w", newline="") as f:
            import csv
            w = csv.writer(f); w.writerow(["module", "pieces_ok", "pieces_total"])
            for r in sorted(rows):
                w.writerow(r)
        print("\n== split-tree compile verification ==")
        print("  modules fully compiling: %d / %d" % (full, len(jobs)))
        print("  pieces compiling: %d / %d (%.1f%%)" % (tok, ttot, 100.0 * tok / max(ttot, 1)))
        print("  -> %s" % os.path.join(outroot, "VERIFY.csv"))
        return 0
    if do_all:
        mods = [f[:-2] for f in sorted(os.listdir(cdir)) if f.endswith(".c")]
    elif not mods:
        print(__doc__); return 0
    total_files = 0
    for m in mods:
        cp = os.path.join(cdir, m + ".c")
        if not os.path.isfile(cp):
            print("  %-14s (no authored .c)" % m); continue
        r = split_module(cp, m, outroot)
        if r:
            total_files += len(r["files"])
            if not do_all or len(mods) < 30:
                print("  %-14s %d funcs -> %d files (%s)" % (m, r["funcs"], len(r["files"]),
                      ", ".join(r["files"][:4]) + (" ..." if len(r["files"]) > 4 else "")))
    print("\nsource tree -> %s   (%d files across %d modules)" % (outroot, total_files, len(mods)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
