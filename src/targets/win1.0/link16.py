#!/usr/bin/env python3
"""
link16.py — take one app's per-segment modules all the way to an NE executable
with the 1985 toolchain, and compare the result against the shipped binary.

splitbuild.py proves the reconstruction COMPILES. This proves it LINKS, which is
a different and stricter thing: a link resolves every cross-module reference and
lays the segments out, so it is the first step that can be measured against the
retail EXE structurally (segment count, attributes, entry table, header fields).

What is generated, and from what:
  <STEM>.DEF   mkdef.py — read out of the shipped NE (see that file)
  <STEM>.LNK   the object list + `slibw swlibc/NOD` (the Windows C runtime with
               NO default-library search — otherwise DOS crt0 and Windows
               winstart both get pulled in and __astart is defined twice)
  BUILD.BAT    compile each module into the segment it was RECOVERED from:
               module <STEM><n><chunk>.C is compiled `msc /NT SEG<n>`, so
               segment n of the rebuilt app holds segment n's functions.

Unresolved externals are not guessed at — the linker is asked. Anything the
reconstruction calls but has not transcribed shows up in the link output, and
that list is the honest inventory of what is still missing.

Usage:  python link16.py CARDFILE CARD          (app, stem)
        python link16.py CARDFILE CARD --keep   (don't re-split first)
"""
import os
import re
import shutil
import subprocess
import sys

import mkdef
from src.core import layout

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
B16 = os.path.join(ROOT, "build16")
DBX = os.path.join(ROOT, "refs", "tools", "dosbox-x", "bin", "x64", "Release",
                   "dosbox-x.exe")

CONF = r"""[dosbox]
memsize=32
[cpu]
cputype=386
core=normal
cycles=max
[dos]
ver=5.00
[sdl]
autolock=false
[autoexec]
mount c {c_root}
c:
set PATH=C:\BIN30;C:\BIN
set INCLUDE=C:\INC30
set LIB=C:\LIB30
set TMP=C:\{wd}
cd C:\{wd}
call BUILD.BAT
exit
"""

MOD_RE = re.compile(r"^(?P<stem>[A-Z]+?)(?P<seg>\d+)(?P<chunk>[A-Z]?)$")


def rmtree_retry(path, tries=12):
    """DOSBox-X can still hold the working directory for a moment after it
    exits, and Windows then refuses the rmdir. Wait it out rather than making
    the caller re-run the build."""
    import time
    for i in range(tries):
        if not os.path.isdir(path):
            return
        try:
            shutil.rmtree(path)
            return
        except PermissionError:
            time.sleep(1.0)
    raise SystemExit("cannot remove %s — a process is still holding it" % path)


def _write_lnk(path, stem, objs):
    """LINK4 response file: object list (2 per line, `+`-continued), run file,
    map, the Windows C runtime with no default-lib search, and the .DEF."""
    lines = []
    for i in range(0, len(objs), 2):
        lines.append(" ".join(objs[i:i + 2]) + (" +" if i + 2 < len(objs) else ""))
    lnk = "\n".join(lines) + "\n"
    lnk += "%s.exe /ALIGN:4\n%s.map/map\nslibw swlibc/NOD\n%s.def\n" % (
        stem, stem, stem)
    open(path, "w", newline="\r\n").write(lnk)


def seg_of_module(mod, stem):
    """<STEM><n><chunk> -> n. The globals module <STEM>G[x] has no code, so it
    is parked in segment 1 (it contributes DATA, which lives in DGROUP)."""
    rest = mod[len(stem):]
    m = re.match(r"^(\d+)", rest)
    if m:
        return int(m.group(1))
    return 1                      # G / GA / GB ...


def adaptive_split(outdir, stem, work, segargs, conf):
    """Split ONE compilation unit per real NE segment, then compile-probe under
    MS C 3.0 and sub-divide ONLY the segments (or the globals module, key 'G')
    whose single unit overflows P1 ("Stack overflow"). Leaves the settled module
    set + .OBJs in outdir for the normal link flow. This is the faithful
    granularity: a segment stays one .obj unless the 1985 compiler cannot hold it,
    exactly the reason Microsoft split a segment across several .objs.

    Returns the final segmax dict ({} means every segment was one unit)."""
    BIG, FLOOR = 999999, 1500
    segmax = {}
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    is_glob = re.compile(re.escape(stem) + r"G")
    workstem = os.path.splitext(os.path.basename(work))[0]   # "_src": NOT a module

    def do_split():
        for f in os.listdir(outdir):
            fl = f.lower()
            if (fl.endswith(".c") or fl.endswith(".h")) and f != os.path.basename(work):
                os.remove(os.path.join(outdir, f))
        args = [sys.executable, os.path.join(HERE, "segsplit.py"), work, outdir,
                "--stem", stem, "--maxbytes", str(BIG)] + segargs
        if segmax:
            args += ["--segmax", ",".join("%s=%d" % (k, v) for k, v in segmax.items())]
        subprocess.run(args, check=True, capture_output=True)
        for fn in sorted(os.listdir(outdir)):
            # NEVER ansi2knr the work file (_src.c): it is segsplit's INPUT and
            # must stay ANSI, or a later re-split pass (after an overflow) reads a
            # K&R'd source, fails to recognize the function headers, and dumps the
            # bodies into the shared header.
            if fn.lower().endswith((".c", ".h")) and fn != os.path.basename(work):
                subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"),
                                os.path.join(outdir, fn)], capture_output=True)

    for _pass in range(7):
        do_split()
        mods = sorted(os.path.splitext(f)[0] for f in os.listdir(outdir)
                      if f.lower().endswith(".c")
                      and os.path.splitext(f)[0] != workstem)
        bat = ["@echo off", "echo probe > build.log"]
        for m in mods:
            seg = 1 if is_glob.match(m) else seg_of_module(m, stem)
            bat += ["del %s.obj" % m,
                    "msc /u /AS /Gsw /Oas /Zpe /Ze /NT %s %s; > %s.out 2> %s.err"
                    % (mkdef.seg_name(seg), m, m, m)]
        bat += ["echo DONE >> build.log"]
        open(os.path.join(outdir, "BUILD.BAT"), "w",
             newline="\r\n").write("\n".join(bat) + "\n")
        try:
            subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                           timeout=1800, capture_output=True)
        except subprocess.TimeoutExpired:
            print("  *** adaptive probe timed out")
        ovf, hard = {}, []
        for m in mods:
            if os.path.exists(os.path.join(outdir, m + ".OBJ")):
                continue
            ep = os.path.join(outdir, m + ".ERR")
            err = open(ep, encoding="cp437", errors="replace").read() \
                if os.path.exists(ep) else ""
            if "Stack overflow" in err:
                ovf['G' if is_glob.match(m) else seg_of_module(m, stem)] = True
            else:
                hard.append(m)
        if hard:
            # a real compile error (not overflow) — let the normal flow surface it
            return segmax
        if not ovf:
            if segmax:
                print("  adaptive per-segment: settled at %d unit(s) over %d pass(es), "
                      "sub-divided seg(s) %s" % (len(mods), _pass + 1, sorted(segmax, key=str)))
            else:
                print("  adaptive per-segment: %d unit(s), one per segment (no overflow)"
                      % len(mods))
            return segmax
        for seg in ovf:
            cur = segmax.get(seg, BIG)
            segmax[seg] = 6000 if cur == BIG else max(FLOOR, cur // 2)
        print("  adaptive: P1 overflow in seg(s) %s -> sub-divide at %s, re-probing..."
              % (sorted(ovf, key=str), {k: segmax[k] for k in ovf}))
    print("  adaptive: reached pass limit; leaving segmax=%s" % segmax)
    return segmax


def main():
    if len(sys.argv) < 3:
        print(__doc__); return 1
    app, stem = sys.argv[1].upper(), sys.argv[2].upper()
    # --src overrides where the C comes from. PAINT and WRITE are each ONE
    # binary reconstructed across TWO source files, so the caller merges them
    # and points here; the app name still selects the binary that the .DEF and
    # .RC are read out of.
    if "--src" in sys.argv:
        src = sys.argv[sys.argv.index("--src") + 1]
    else:
        src = os.path.join(layout.recon_dir(ROOT), sys.argv[1].lower() + ".c")
    wd = stem + "LK"
    outdir = os.path.join(B16, wd)

    if "--keep" not in sys.argv:
        rmtree_retry(outdir)
        os.makedirs(outdir)
        if "--nosplit" in sys.argv:
            # SINGLE translation unit — compile the whole app as one .c, the way
            # Microsoft's own lean source did. No segsplit: the file's exported
            # functions are already public (FAR PASCAL About/ClockWndProc) and
            # its helpers already static, so one unit links exactly. Viable once
            # the source is small enough for MS C 3.0's P1 to digest in one pass.
            work = os.path.join(outdir, stem + "1.C")   # seg 1 -> _TEXT
            shutil.copyfile(src, work)
            subprocess.run([sys.executable, os.path.join(HERE, "hoistdecl.py"), work],
                           check=True, capture_output=True)
            subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"), work],
                           capture_output=True)
        else:
            work = os.path.join(outdir, "_src.c")
            shutil.copyfile(src, work)
            subprocess.run([sys.executable, os.path.join(HERE, "hoistdecl.py"), work],
                           check=True, capture_output=True)
            # Faithful per-segment split: the sidecar segmap (from gen_segmap.py)
            # restores each function's home segment (the inline segN: banners were
            # stripped). adaptive_split then makes ONE unit per segment and
            # sub-divides only the segments whose single unit overflows MS C 3.0 —
            # the minimal, most Microsoft-like granularity.
            segmap = os.path.join(layout.recon_sub("segmaps", ROOT), stem + ".segmap")
            segargs = ["--segmap", segmap] if os.path.exists(segmap) else []
            conf0 = os.path.join(B16, "_lk.conf")
            open(conf0, "w").write(CONF.format(c_root=B16, wd=wd))
            adaptive_split(outdir, stem, work, segargs, conf0)
            os.remove(work)

    # ASM helpers: some functions (DOS-int shims like GetDosTime) cannot be C
    # and are transcribed as <app>a.asm — exactly as Microsoft shipped CARDFILE
    # with INDOS.ASM. Copy any such module in beside the C, to be assembled by
    # MASM and linked. Its OBJ is an ordinary member of the link.
    asm_mods = []
    if "--keep" not in sys.argv:
        for cand in (os.path.basename(src)[:-2] + "a.asm",
                     sys.argv[1].lower() + "a.asm"):
            ap = os.path.join(layout.recon_dir(ROOT), cand)
            if os.path.exists(ap):
                base = (stem + "A")[:8]
                shutil.copyfile(ap, os.path.join(outdir, base + ".ASM"))
                asm_mods.append(base)
                break

    mods = sorted(os.path.splitext(f)[0] for f in os.listdir(outdir)
                  if f.lower().endswith(".c"))
    if not mods and not asm_mods:
        print("no modules in %s" % outdir); return 1
    n_stubs = 0

    # .DEF, straight from the shipped binary
    defpath = os.path.join(outdir, stem + ".DEF")
    binpath = os.path.join(ROOT, "files", "WF", app + ".EXE")
    text, info = mkdef.build(app, binpath, stem)
    open(defpath, "w", newline="\r\n").write(text)

    # .RC — the resources, transcribed out of the same binary (mkrc.py), then
    # compiled by the period RC and bound into the linked EXE. An app without
    # them links but is not the app: the menus, dialogs and strings ARE half of
    # what it is.
    rcname = None
    if "--rc" in sys.argv:
        import mkrc
        text, counts, nstr = mkrc.build(app, outdir)
        rcname = app[:8] + ".RC"
        open(os.path.join(outdir, rcname), "w", encoding="cp437",
             errors="replace", newline="\r\n").write(text)
        print("resources: %s, %d strings -> %s"
              % (", ".join("%s=%d" % kv for kv in sorted(counts.items())),
                 nstr, rcname))

    # .LNK — objects (C modules + any ASM helpers), target, map, libraries, def
    objs = [m + ".obj" for m in mods] + [m + ".obj" for m in asm_mods]
    _write_lnk(os.path.join(outdir, stem + ".LNK"), stem, objs)

    # BUILD.BAT — compile each module into its recovered segment, then link
    bat = ["@echo off", "echo link build > build.log"]
    for m in asm_mods:
        # MASM with the SDK's own switches: ?PLM = PASCAL calling convention,
        # ?WIN = Windows prologue/epilogue. Same line CARDBLD.BAT uses for
        # INDOS.ASM. cmacros.inc must be on the include path (it is, in INC30).
        bat += [
            "del %s.obj" % m,
            "masm %s; > %s.out 2> %s.err" % (m, m, m),
            "if not exist %s.obj echo   NO-OBJ %s (asm) >> build.log" % (m, m),
        ]
    for m in mods:
        seg = seg_of_module(m, stem)
        bat += [
            "del %s.obj" % m,
            "msc /u /AS /Gsw /Oas /Zpe /Ze /NT %s %s; > %s.out 2> %s.err"
            % (mkdef.seg_name(seg), m, m, m),
            "if not exist %s.obj echo   NO-OBJ %s >> build.log" % (m, m),
        ]
    bat += [
        "echo === LINK4 === >> build.log",
        "link4 @%s.LNK > link.txt 2> link.err" % stem,
        "type link.txt >> build.log",
    ]
    if rcname:
        bat += ["echo === RC === >> build.log",
                "rc -r %s > rc.txt" % rcname,
                "rc %s.RES %s.exe >> rc.txt" % (rcname[:-3], stem),
                "type rc.txt >> build.log"]
    bat += [
        "if exist %s.exe echo   EXE-OK >> build.log" % stem,
        "if not exist %s.exe echo   NO-EXE >> build.log" % stem,
        "echo DONE >> build.log",
    ]
    open(os.path.join(outdir, "BUILD.BAT"), "w",
         newline="\r\n").write("\n".join(bat) + "\n")

    conf = os.path.join(B16, "_lk.conf")
    open(conf, "w").write(CONF.format(c_root=B16, wd=wd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=1800, capture_output=True)
    except subprocess.TimeoutExpired:
        print("*** DOSBox-X timed out")

    # ---- report ----------------------------------------------------------
    nobj = [m for m in mods
            if not os.path.exists(os.path.join(outdir, m + ".OBJ"))]
    print("modules: %d compiled, %d failed%s"
          % (len(mods) - len(nobj), len(nobj),
             ("  (" + ", ".join(nobj) + ")") if nobj else ""))

    linktxt = os.path.join(outdir, "LINK.TXT")
    unresolved = []
    txt = ""
    for _p in (linktxt, os.path.join(outdir, "LINK.ERR")):
        if os.path.exists(_p):
            txt += open(_p, encoding="cp437", errors="replace").read()
    if txt:
        for m in re.finditer(r"unresolved external(?:s)?[^\n]*", txt, re.I):
            print("  " + m.group(0).strip())
        unresolved = re.findall(r"'([^']+)'\s*:\s*unresolved external", txt, re.I)
        if not unresolved:
            unresolved = re.findall(
                r"unresolved external(?:\s+symbol)?\s*:?\s*'?([A-Za-z_@][\w@]*)",
                txt, re.I)
        other = [ln.strip() for ln in txt.splitlines()
                 if "error" in ln.lower() and "unresolved" not in ln.lower()]
        for ln in other[:10]:
            print("  " + ln)
    if unresolved:
        print("\nUNRESOLVED EXTERNALS (%d) — the reconstruction calls these and\n"
              "has not transcribed them; this is the missing-work inventory:"
              % len(unresolved))
        for u in sorted(set(unresolved)):
            print("    " + u)

    # Second pass: the linker has now NAMED everything missing, so stub it and
    # rebuild. Two passes are unavoidable — the stub declarations turn the call
    # sites into far calls, so every module has to be recompiled.
    if "--stubs" in sys.argv and n_stubs == 0:
        import mkstubs
        syms = mkstubs.parse_unresolved(os.path.join(outdir, "LINK.TXT"))
        if syms:
            n, winmain = mkstubs.generate(outdir, stem, syms)
            # --graft: before stubbing, GRAFT lift.py bodies for the stubs that
            # have them, so those functions do the RECOVERED thing instead of
            # returning 0. mkstubs already wrote the stub module; graft resolves
            # what it can, strips those from it, and hands back the graft modules
            # (with their segments) to link in.
            graft_segs = {}
            if "--graft" in sys.argv:
                graft_segs = apply_graft(outdir, stem, sys.argv[1].lower())
            print("\nstubbing %d untranscribed function(s)%s%s and relinking..."
                  % (n, " (including WinMain)" if winmain else "",
                     (" [%d grafted]" % len(graft_segs)) if graft_segs else ""))
            sys.argv = [a for a in sys.argv if a != "--stubs"] + ["--relink"]
            return relink(outdir, stem, app, binpath, n, rcname, asm_mods,
                          graft_segs)

    exe = os.path.join(outdir, stem + ".EXE")
    if os.path.exists(exe):
        # 0-stub apps (e.g. clock) bind here rather than in relink(). Same
        # verified-clean gate before the resource bind.
        if rcname:
            link_clean = (len(unresolved) == 0
                          and "Intersegment self-relative fixup" not in txt)
            _bind_resources(outdir, stem, rcname, exe, link_clean)
        print("\nLINKED: %s (%d bytes)" % (exe, os.path.getsize(exe)))
        compare(exe, binpath)
    else:
        print("\nno EXE produced")
    return 0


def _strip_func_def(text, nm):
    """Remove the entire function DEFINITION of `nm` from a stub module — its
    header, any K&R param declarations, and the brace-matched body. Handles
    parameterized and multi-line stubs (WinMain, FormatAlarmList(a1,a2)) that the
    old empty-paren single-line regex missed, leaving a duplicate of a grafted fn."""
    m = re.search(r"(?m)^[A-Za-z_][\w \t\*]*?\b" + re.escape(nm) + r"\s*\([^;{)]*\)",
                  text)
    if not m:
        return text
    i = text.find("{", m.end())
    if i < 0 or i - m.end() > 300:      # body brace should be near (past K&R decls)
        return text
    depth, j = 0, i
    while j < len(text):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                j += 1
                break
        j += 1
    start = text.rfind("\n", 0, m.start()) + 1
    return text[:start] + text[j:]


def apply_graft(outdir, stem, appname):
    """Run graft.py for this app, process the emitted graft modules
    (hoistdecl + ansi2knr), strip the stubs it resolved from <STEM>X.C, and
    return {graft_module: segment} so relink compiles each into the right
    segment. Returns {} if graft produced nothing."""
    r = subprocess.run([sys.executable, os.path.join(HERE, "graft.py"),
                        appname, "--emit", outdir],
                       capture_output=True, text=True)
    print("  " + (r.stdout or "").strip().replace("\n", "\n  "))
    lst = os.path.join(outdir, stem + "GRAFT.LST")
    if not os.path.exists(lst):
        return {}
    segs, resolved = {}, set()
    for line in open(lst):
        parts = line.split()
        if len(parts) >= 2:
            segs[parts[0]] = int(parts[1])
            if len(parts) >= 3:
                resolved.add(parts[2])
    for m in segs:
        p = os.path.join(outdir, m + ".C")
        subprocess.run([sys.executable, os.path.join(HERE, "hoistdecl.py"), p],
                       capture_output=True)
        subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"), p],
                       capture_output=True)
    # strip the resolved stubs' definitions from the stub module (the graft now
    # provides them — leaving both would be a redefinition).
    xf = os.path.join(outdir, stem + "X.C")
    if os.path.exists(xf) and resolved:
        x = open(xf, encoding="utf-8", errors="replace").read()
        for nm in resolved:
            x = _strip_func_def(x, nm)          # whole def: header+K&R decls+body
        open(xf, "w", encoding="utf-8", newline="").write(x)
    # Reconcile the shared header to the GRAFT's signature. The app declares a
    # grafted function with its understood type (`HANDLE PutUpDB(WORD)`); the
    # graft defines it with the lift's (`long FAR PutUpDB(...)`). A different
    # return type is a "redefinition" to C 3.0, so rewrite every declaration of
    # a grafted name in the header to `<graft-ret> FAR NAME();`, which every
    # module (callers and the graft) then agrees on.
    hf = os.path.join(outdir, stem + ".H")
    if os.path.exists(hf) and resolved:
        h = open(hf, encoding="utf-8", errors="replace").read()
        for m, sg in segs.items():
            gt = open(os.path.join(outdir, m + ".C"),
                      encoding="utf-8", errors="replace").read()
            for hm in re.finditer(r"(?m)^([A-Za-z_][\w \t\*]*?)\bFAR\s+"
                                  r"([A-Za-z_]\w*)\s*\(", gt):
                nm = hm.group(2)
                if nm not in resolved:
                    continue
                ret = " ".join(hm.group(1).split())
                # `[^;{)]*` STOPS at the first `)` — the old `[^;{]*` was greedy
                # and, when the declaration had no early `);`, spanned across
                # the file's typedefs to a later `);`, DELETING them (CARDHEADER
                # vanished from CARD.H). Match only up to `;` too, so a trailing
                # multi-line comment is preserved.
                h = re.sub(r"(?m)^[A-Za-z_][\w \t\*]*?\b" + re.escape(nm)
                           + r"\s*\([^;{)]*\)\s*;",
                           "%s FAR %s();" % (ret, nm), h)
        open(hf, "w", encoding="utf-8", newline="").write(h)
    return segs


def _map_homes(mapfile):
    """{func_name: segment} from a LINK map's `SEG:off _NAME` publics — ground
    truth for where each function LANDED, unlike parsing K&R source. Skips
    imports (SEG 0000 with `Imp`)."""
    home = {}
    if not os.path.exists(mapfile):
        return home
    for line in open(mapfile, encoding="cp437", errors="replace"):
        m = re.match(r"\s*([0-9A-Fa-f]{4}):[0-9A-Fa-f]+\s+_([A-Za-z_]\w*)\s*$",
                     line)
        if m:
            seg = int(m.group(1), 16)
            if seg:
                home[m.group(2)] = seg
    return home


def farfix(outdir, stem, graft_segs=None):
    """Re-derive FAR across the whole program using the LINK MAP as the home
    map (ground truth: which segment each function landed in), then FAR-ify any
    function CALLED from a different segment — in its definition AND the shared
    header. Grafting adds cross-segment call edges segsplit never saw, so those
    calls otherwise emit near "Intersegment self-relative fixup"s -> 0x2006 ->
    RC won't bind. Returns the count FAR-fixed (0 if nothing to do)."""
    import segsplit
    home = _map_homes(os.path.join(outdir, stem + ".MAP"))
    if not home:
        return 0
    mods = [os.path.splitext(f)[0] for f in os.listdir(outdir)
            if f.lower().endswith(".c")]
    text = {m: open(os.path.join(outdir, m + ".C"),
                    encoding="utf-8", errors="replace").read() for m in mods}
    # SAFE OVER-APPROXIMATION: FAR every function that a GRAFT module calls (and
    # every graft function itself). Precisely deciding which calls cross a
    # segment from the K&R source proved unreliable — the source-derived edges
    # did not match the linker's actual near-cross-segment fixups. A FAR call to
    # a SAME-segment target is valid (CS unchanged), just slightly larger, so
    # over-FAR-ing the graft-touched functions is safe and eliminates ALL
    # graft-induced fixups. Every caller sees the FAR decl via the shared
    # header/preamble, so app callers stay consistent too.
    graft_mods = set(graft_segs or {})
    far = set()
    for m in mods:
        called_here = set(re.findall(r"(?<![\w.])([A-Za-z_]\w*)\s*\(",
                                     _strip(text[m])))
        if m in graft_mods:
            far |= (called_here & set(home))       # everything grafts call
    # plus the graft functions themselves (called by app code cross-segment)
    for m in graft_mods:
        for u in _func_units(text[m]):
            nm = segsplit.func_name_of(u)
            if nm in home:
                far.add(nm)
    far = {nm for nm in far if nm in home}
    if not far:
        return 0
    def far_all(s):
        """FAR-ify EVERY column-0 `TYPE nm(` of a far function — both its
        prototype (`nm();` in the graft preambles AND the header) and its
        DEFINITION (`nm(...) TYPE a; {`). Doing both in one regex avoids relying
        on the K&R function-unit parser, which missed some definitions (leaving
        the near definition to override the far declaration -> the calls stayed
        near). Insert FAR right before the name, only if the qualifier is not
        already far/pascal after its last `*`."""
        for nm in far:
            s = re.sub(r"(?m)^([A-Za-z_][\w \t\*]*?)\b" + re.escape(nm)
                       + r"(\s*\()",
                       lambda mm: mm.group(1) + ("" if segsplit._fn_already_far(
                           mm.group(1)) else "FAR ") + nm + mm.group(2), s)
        return s

    for m in mods:
        s = text[m]
        ns = far_all(s)
        if ns != s:
            open(os.path.join(outdir, m + ".C"), "w",
                 encoding="utf-8", newline="").write(ns)
    hf = os.path.join(outdir, stem + ".H")
    if os.path.exists(hf):
        # READ first, THEN open for write. `open(hf,"w").write(f(open(hf).read()))`
        # truncates hf before the inner read runs (Python evaluates the call
        # target first), so f() got "" and CARD.H was emptied — everything then
        # "undefined".
        hh = far_all(open(hf, encoding="utf-8", errors="replace").read())
        open(hf, "w", encoding="utf-8", newline="").write(hh)
    print("  FAR-fixed %d functions (link-map home) across the graft graph" % len(far))
    return len(far)


def _strip(s):
    return re.sub(r"/\*.*?\*/", " ", s, flags=re.S)


def _func_units(text):
    """Yield each top-level function DEFINITION text. K&R-aware: the build-dir
    modules are post-ansi2knr, so a definition is `TYPE NAME(a,b) TYPE a; { .. }`
    — its header line does NOT end in `)` (segsplit's ANSI scanner misses it).
    We take each column-0 `TYPE NAME(` header and, if a `{` body follows before
    the next such header, treat the span as a definition."""
    heads = list(re.finditer(r"(?m)^([A-Za-z_][\w \t\*]*?)\b([A-Za-z_]\w*)"
                             r"\s*\([^;{)]*\)", _strip(text)))
    for i, h in enumerate(heads):
        end = heads[i + 1].start() if i + 1 < len(heads) else len(text)
        unit = text[h.start():end]
        # a definition has a body `{`; a bare prototype/decl before the next
        # header does not.
        b = _strip(unit)
        if "{" in b and (b.index("{") < (b.index(";") if ";" in b else 1 << 30)
                         or re.search(r"\)\s*[A-Za-z_][\w \t\*]*;", b)):
            yield unit


def _defined_funcs(cpath):
    """The top-level function(s) a graft module DEFINES, as {name: is_far}.
    A graft module is one function per file (plus the _mkpt helper), emitted
    K&R after ansi2knr: a column-0 `TYPE [FAR] NAME(params)` line with a body,
    not a `;`-terminated prototype. Used to stub what a module that overflowed
    MS C 3.0's P1 table would have defined."""
    out = {}
    try:
        src = open(cpath, encoding="utf-8", errors="replace").read()
    except OSError:
        return out
    for ln in src.splitlines():
        if not ln or not (ln[0].isalpha() or ln[0] == "_"):
            continue
        if "(" not in ln or ln.rstrip().endswith(";"):
            continue
        m = re.match(r"[A-Za-z_][\w \t]*?\b(FAR\s+)?([A-Za-z_]\w*)\s*\(", ln)
        if not m:
            continue
        name = m.group(2)
        if name in ("_mkpt", "if", "for", "while", "switch", "return"):
            continue
        out[name] = bool(m.group(1))
    return out


def relink(outdir, stem, app, binpath, n_stubs, rcname=None, asm_mods=(),
           graft_segs=None):
    """Recompile every module (the stub declarations changed the call sites)
    and link again. ASM helpers are re-assembled too — they contribute OBJs
    like any module and must be in the object list."""
    mods = sorted(os.path.splitext(f)[0] for f in os.listdir(outdir)
                  if f.lower().endswith(".c"))
    objs = [m + ".obj" for m in mods] + [m + ".obj" for m in asm_mods]
    _write_lnk(os.path.join(outdir, stem + ".LNK"), stem, objs)

    bat = ["@echo off", "echo relink > build.log"]
    for m in asm_mods:
        bat += ["del %s.obj" % m,
                "masm %s; > %s.out 2> %s.err" % (m, m, m)]
    graft_segs = graft_segs or {}
    for m in mods:
        # GRAFTS compile into their RECOVERED segment (graft_segs[m], from the
        # lift's segment-aware naming), so a big multi-segment app's grafted logic
        # is DISTRIBUTED across its real segments instead of overflowing one _TEXT.
        # They compile in the MEDIUM model (/AM = far code, near data) -- the model
        # Microsoft used for multi-segment apps -- so the compiler emits FAR calls
        # to the 32-bit math helpers (__aFulmul/__aFuldiv, pulled from MLIBC) and to
        # other segments' functions, instead of near calls that can't leave _TEXT.
        # Hand modules stay small (/AS, near code) matching the single-segment apps.
        seg = graft_segs[m] if m in graft_segs else seg_of_module(m, stem)
        # Default is the faithful /AS (small, near) path; farfix FAR-declares the
        # cross-segment graft edges. /AM (medium, far code) is an opt-in override
        # (W1_GRAFT_MODEL=medium) that far-ifies grafts wholesale -- kept as a
        # fallback but NOT default: it over-far-ifies and risks graft->hand near/
        # far mismatches the linker can't catch. seg != 1 so _TEXT grafts stay near.
        model = ("/AM" if (m in graft_segs and seg != 1
                 and os.environ.get("W1_GRAFT_MODEL") == "medium") else "/AS")
        bat += ["del %s.obj" % m,
                "msc /u %s /Gsw /Oas /Zpe /Ze /NT %s %s; > %s.out 2> %s.err"
                % (model, mkdef.seg_name(seg), m, m, m)]
    bat += ["del %s.exe" % stem,
            "link4 @%s.LNK > link.txt 2> link.err" % stem]
    if rcname:
        # Compile the .RC to a .RES now (this is safe regardless of the EXE's
        # flags). The actual BIND into the EXE is a SEPARATE dosbox session
        # below, because RC refuses an EXE carrying the errors-in-image flag and
        # LINK4 sometimes sets that flag spuriously (TERMINAL: 0 unresolved, 0
        # fixups, clean relocations, yet 0x2000) — we clear that verified-false
        # flag on the host between the two sessions.
        bat += ["rc -r %s > rc.txt 2>&1" % rcname]
    bat += ["echo DONE >> build.log"]
    open(os.path.join(outdir, "BUILD.BAT"), "w",
         newline="\r\n").write("\n".join(bat) + "\n")
    conf = os.path.join(B16, "_lk.conf")
    open(conf, "w").write(CONF.format(c_root=B16, wd=os.path.basename(outdir)))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=1800, capture_output=True)
    except subprocess.TimeoutExpired:
        print("*** DOSBox-X timed out")

    # A graft module that FAILED TO COMPILE (MS C 3.0 P1 "Stack overflow" on an
    # over-large lifted function) leaves no OBJ. Its missing OBJ would sink the
    # whole link, so DROP the module and stub what it defined instead — an
    # honest limit: that function exceeds the compiler's single-unit capacity.
    # Detected here (not just as a link "unresolved") because a missing OBJ in
    # the response file is a LINK4 file error, not a clean unresolved-external.
    overflow_stubs = {}
    if graft_segs:
        dropped_ovf, dropped_other = [], []
        for m in list(mods):
            if m in graft_segs and \
                    not os.path.exists(os.path.join(outdir, m + ".OBJ")):
                cpath = os.path.join(outdir, m + ".C")
                overflow_stubs.update(_defined_funcs(cpath))
                ep = os.path.join(outdir, m + ".ERR")
                etxt = open(ep, encoding="cp437", errors="replace").read() \
                    if os.path.exists(ep) else ""
                (dropped_ovf if "Stack overflow" in etxt
                 else dropped_other).append(m)
                if os.path.exists(cpath):
                    os.remove(cpath)
                mods.remove(m)
                graft_segs.pop(m, None)
        if dropped_ovf:
            print("  %d graft module(s) exceeded MS C 3.0 P1 (Stack overflow), "
                  "stubbed: %s" % (len(dropped_ovf), ", ".join(dropped_ovf)))
        if dropped_other:
            print("  *** %d graft module(s) FAILED to compile (not overflow) — "
                  "stubbed, INVESTIGATE: %s"
                  % (len(dropped_other), ", ".join(dropped_other)))

    # ONE-SHOT FAR correction for grafted builds. The first link produced the
    # map (ground truth for every function's segment); grafting introduced
    # cross-segment call edges that show up as "Intersegment self-relative
    # fixup"s. Re-derive FAR from the map, apply it, and rebuild ONCE — the
    # rebuild is the same BAT (recompiles with the FAR changes, relinks).
    if graft_segs:
        import mkstubs
        yf = stem + "Y0"
        # {name: is_far}, accumulates. Graft-introduced unresolved are near
        # internal helpers; overflow-dropped modules carry their detected
        # FAR-ness (a dropped stub TARGET is far-called by the app, so its stub
        # MUST be far or the app's far call lands on a near entry and crashes).
        stub_far = dict(overflow_stubs)

        def _link_text():
            t = ""
            for lt in (os.path.join(outdir, "LINK.TXT"),
                       os.path.join(outdir, "LINK.ERR")):
                if os.path.exists(lt):
                    t += open(lt, encoding="cp437", errors="replace").read()
            return t

        def _unresolved_count(t):
            return len(re.findall(r"^\s*[A-Za-z_@][\w@]*\s+in file\(s\)\s*:",
                                  t.split("Unresolved externals:", 1)[-1], re.M)) \
                if "Unresolved externals:" in t else 0

        def _rebuild():
            # regenerate BOTH the BAT (to compile any new module, e.g. the
            # graft-stub module, into its segment) and the .LNK, then rebuild.
            allmods = sorted(os.path.splitext(f)[0] for f in os.listdir(outdir)
                             if f.lower().endswith(".c"))
            rb = ["@echo off", "echo rebuild > build.log"]
            for m in asm_mods:
                rb += ["del %s.obj" % m, "masm %s; > %s.out 2> %s.err" % (m, m, m)]
            for m in allmods:
                sg = graft_segs[m] if m in graft_segs else seg_of_module(m, stem)
                md = ("/AM" if (m in graft_segs and sg != 1
                      and os.environ.get("W1_GRAFT_MODEL") == "medium") else "/AS")
                rb += ["del %s.obj" % m,
                       "msc /u %s /Gsw /Oas /Zpe /Ze /NT %s %s; > %s.out 2> %s.err"
                       % (md, mkdef.seg_name(sg), m, m, m)]
            rb += ["del %s.exe" % stem,
                   "link4 @%s.LNK > link.txt 2> link.err" % stem]
            if rcname:
                rb += ["rc -r %s > rc.txt 2>&1" % rcname]
            rb += ["echo DONE >> build.log"]
            open(os.path.join(outdir, "BUILD.BAT"), "w",
                 newline="\r\n").write("\n".join(rb) + "\n")
            _write_lnk(os.path.join(outdir, stem + ".LNK"), stem,
                       [m + ".obj" for m in allmods]
                       + [m + ".obj" for m in asm_mods])
            try:
                subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16,
                               env=env, timeout=1800, capture_output=True)
            except subprocess.TimeoutExpired:
                print("*** DOSBox-X timed out")

        # ONE-SHOT FAR correction. The first link produced the map (ground truth
        # for every function's segment); grafting introduced cross-segment call
        # edges that show up as "Intersegment self-relative fixup"s. Re-derive
        # FAR from the map and apply it; it is folded into the first rebuild.
        did_far = "Intersegment self-relative fixup" in _link_text() \
            and farfix(outdir, stem, graft_segs) > 0
        if did_far:
            print("  applying FAR corrections from the link map...")

        # STUB-AND-REBUILD LOOP. Grafting introduces unresolved the first mkstubs
        # never saw — a transitive callee with no lift body (_sub_098f), a lift
        # artifact that calls the module NAME (_TERMINAL), a P1-overflow-dropped
        # module. Stub them in a SEPARATE _TEXT module (mkstubs.generate would
        # OVERWRITE the main stub module). ONE link's unresolved list can be
        # INCOMPLETE when an OBJ was missing (an overflow drop), so LOOP: stub,
        # rebuild, re-parse, until the link is clean or nothing new appears.
        last = -1
        for _pass in range(5):
            for s in mkstubs.parse_unresolved(os.path.join(outdir, "LINK.TXT")):
                if s.upper() != "WINMAIN":
                    stub_far.setdefault(s[1:] if s.startswith("_") else s, False)
            if not stub_far and not did_far:
                break                              # nothing to do at all
            if len(stub_far) == last and not did_far:
                break                              # converged (or stuck)
            last = len(stub_far)
            if stub_far:
                open(os.path.join(outdir, yf + ".C"), "w", encoding="utf-8",
                     newline="").write(
                    '#include <windows.h>\n#include "%s.h"\n\n' % stem
                    + "".join(
                        "long %s%s() { return 0; }   /* NOT RECONSTRUCTED (%s) */\n"
                        % ("FAR " if stub_far[n] else "", n,
                           "P1 stack-overflow" if n in overflow_stubs
                           else "graft-introduced")
                        for n in sorted(stub_far)))
                graft_segs[yf] = 1               # compiled into _TEXT, like grafts
                print("  pass %d: stubbing %d unresolved (%d P1-overflow), "
                      "rebuilding..." % (_pass + 1, len(stub_far),
                                         len(overflow_stubs)))
            _rebuild()
            did_far = False
            if _unresolved_count(_link_text()) == 0:
                break

    nobj = [m for m in mods
            if not os.path.exists(os.path.join(outdir, m + ".OBJ"))]
    print("modules: %d compiled, %d failed%s"
          % (len(mods) - len(nobj), len(nobj),
             ("  (" + ", ".join(nobj) + ")") if nobj else ""))
    txt = ""
    for lt in (os.path.join(outdir, "LINK.TXT"), os.path.join(outdir, "LINK.ERR")):
        if os.path.exists(lt):
            txt += open(lt, encoding="cp437", errors="replace").read()
    left = len(re.findall(r"^\s*[A-Za-z_@][\w@]*\s+in file\(s\)\s*:",
                          txt.split("Unresolved externals:", 1)[-1], re.M)) \
        if "Unresolved externals:" in txt else 0
    fixups = len(re.findall(r"Intersegment self-relative fixup", txt))
    print("unresolved after stubbing: %d   intersegment near-call fixups: %d"
          % (left, fixups))
    exe = os.path.join(outdir, stem + ".EXE")
    if os.path.exists(exe):
        # Bind resources in a second session. If the link was VERIFIED clean
        # (no unresolved, no fixups, no error lines) but the EXE still carries
        # the errors-in-image bit, clear that false positive first so RC binds.
        if rcname:
            link_clean = (left == 0 and fixups == 0
                          and "error" not in txt.lower())
            _bind_resources(outdir, stem, rcname, exe, link_clean)
        print("\nLINKED: %s (%d bytes)   [%d stubbed functions inside]"
              % (exe, os.path.getsize(exe), n_stubs))
        compare(exe, binpath)
    else:
        print("\nno EXE produced")
    return 0


def clear_error_flag_if_set(exe):
    """Clear the NE 'errors in image' bit (0x2000). Returns True if it was set.
    Call ONLY when the link is independently verified clean — this overrides a
    LINK4 false positive, it must never mask a real error."""
    import struct
    d = bytearray(open(exe, "rb").read())
    neoff = struct.unpack_from("<H", d, 0x3c)[0]
    flags = struct.unpack_from("<H", d, neoff + 0x0c)[0]
    if flags & 0x2000:
        struct.pack_into("<H", d, neoff + 0x0c, flags & ~0x2000)
        open(exe, "wb").write(d)
        return True
    return False


def _bind_resources(outdir, stem, rcname, exe, link_clean):
    """Second dosbox session: `rc <name>.RES <stem>.EXE`. Clears a spurious
    errors-in-image flag first when the link was verified clean."""
    if link_clean and clear_error_flag_if_set(exe):
        print("  (cleared spurious errors-in-image flag — link verified clean)")
    # Compile the .RC and bind it in ONE rc pass, now that the flag is cleared.
    # The compile+bind already attempted inside BUILD.BAT runs before Python can
    # clear the flag, so RC there refuses the still-flagged EXE (0-stub apps hit
    # this — there is no relink pass to retry it). rc <name>.RC <stem>.EXE both
    # recompiles the .RES and binds, so a failed earlier .RES is not needed.
    open(os.path.join(outdir, "BIND.BAT"), "w", newline="\r\n").write(
        "@echo off\r\nrc %s %s.EXE > bind.txt 2>&1\r\ntype bind.txt\r\n"
        % (rcname, stem))
    conf = os.path.join(B16, "_bind.conf")
    open(conf, "w").write(
        CONF.format(c_root=B16, wd=os.path.basename(outdir))
        .replace("call BUILD.BAT", "call BIND.BAT"))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=300, capture_output=True)
    except subprocess.TimeoutExpired:
        print("  (resource bind timed out)")


def compare(built, shipped):
    """Put the rebuilt NE beside the shipped one, header field by header field."""
    from src.formats.ne import NEFile
    try:
        a, b = NEFile(built), NEFile(shipped)
    except Exception as e:
        print("  (cannot compare: %s)" % e); return
    print("\n%-22s %-14s %-14s" % ("NE FIELD", "REBUILT", "SHIPPED"))
    print("-" * 52)
    rows = [("segments", a.h["n_segs"], b.h["n_segs"]),
            ("movable entries", a.h["n_movable"], b.h["n_movable"]),
            ("entry table bytes", a.h["entry_len"], b.h["entry_len"]),
            ("heap", a.h["heap"], b.h["heap"]),
            ("stack", a.h["stack"], b.h["stack"]),
            ("flags", hex(a.h["flags"]), hex(b.h["flags"])),
            ("autodata seg", a.h["autodata_seg"], b.h["autodata_seg"]),
            ("module refs", a.h["n_modrefs"], b.h["n_modrefs"])]
    for name, x, y in rows:
        mark = "" if str(x) == str(y) else "   <-- differs"
        print("%-22s %-14s %-14s%s" % (name, x, y, mark))
    sa, sb = a.segments(), b.segments()
    print("\n%-6s %-10s %-10s %s" % ("SEG", "REBUILT", "SHIPPED", "kind"))
    for i in range(max(len(sa), len(sb))):
        x = ("%d" % sa[i]["length"]) if i < len(sa) else "-"
        y = ("%d" % sb[i]["length"]) if i < len(sb) else "-"
        k = sb[i]["kind"] if i < len(sb) else (sa[i]["kind"] if i < len(sa) else "")
        print("%-6d %-10s %-10s %s" % (i + 1, x, y, k))
    print("\nsize: rebuilt %d B, shipped %d B"
          % (os.path.getsize(built), os.path.getsize(shipped)))


if __name__ == "__main__":
    sys.exit(main())
