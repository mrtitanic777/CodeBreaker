#!/usr/bin/env python3
"""
linkdll.py — build a Windows 1.0 SYSTEM MODULE (a LIBRARY/DLL NE) from its
mechanical lift, the way link16.py builds an app EXE.

The 9 system modules (KERNEL/GDI/USER + the 6 drivers) have NO hand-written
reconstruction — only reconstructed/auto_lift/<mod>_lifted.c. So the module's
SOURCE here is that lifted file, with a FAR PASCAL entry `<MOD>_<ord>` appended
per exported ordinal that forwards to the lifted body at that offset (or returns
0 if that offset did not lift — the drivers' hardware/int routines). The combined
source is split (C 3.0 symbol-table limit), compiled, its unresolved internal
callees stubbed, and linked against a LIBRARY .DEF (mkdef.py emits `LIBRARY <mod>`
+ the ordinal exports).

Same STANDARD as the apps: a structurally-matching NE (library flag, segment
count, ordinal exports), NOT byte-identical — the C runtime and the lift
consolidation are the same flagged deviations the grafted apps carry.

    python linkdll.py SYSTEM
    python linkdll.py KERNEL --stem KRNL
"""
import os, re, sys, subprocess, shutil
import link16, mkdef, cfg, graft
from src.core import layout

ROOT, B16, DBX, CONF = link16.ROOT, link16.B16, link16.DBX, link16.CONF
HERE = os.path.dirname(os.path.abspath(__file__))   # sibling tools live here

# Short stems: DOS is 8.3, and a module name + segment + chunk (<STEM><seg><ch>)
# must stay <=8 chars, as must the build dir (<STEM>D). KEYBOARD (8) would
# overflow, so every system module gets a <=3-char stem.
STEM = {"KERNEL": "KRN", "GDI": "GDI", "USER": "USR", "SYSTEM": "SYS",
        "KEYBOARD": "KBD", "MOUSE": "MOU", "DISPLAY": "DSP", "SOUND": "SND",
        "COMM": "COM"}


def distribute_segments(lift_text, mod, budget=22000):
    """A big module's lift is ONE segment (KERNEL is 179 functions in seg1) whose
    verbose C compiles past the 64KB _TEXT limit even though the original hand-ASM
    fit. Split it into MULTIPLE pseudo-segments by rewriting each function's banner
    `<MOD> segN:` tag, so segsplit groups them into separate SEGn modules (each
    <=budget bytes of source -> under 64KB compiled) and FAR-marks the calls that
    now cross a segment. The function NAMES (mod_sN_off) are untouched, so
    cross-references still resolve. Returns (text, nsegs)."""
    # split at each top-level function banner
    parts = re.split(r"(?m)^(?=long %s_s\d+_[0-9a-f]+\()" % mod.lower(), lift_text)
    head, funcs = parts[0], parts[1:]
    seg, used, out, segmap = 1, 0, [head], {}
    for f in funcs:
        if used and used + len(f) > budget:
            seg += 1; used = 0
        m = re.match(r"long %s_s(\d+)_([0-9a-f]+)\(" % mod.lower(), f)
        if m:
            segmap[(int(m.group(1)), int(m.group(2), 16))] = seg
        # rewrite this function's banner segment tag to its pseudo-segment
        f = re.sub(r"(/\*\s*%s )seg\d+(:0x[0-9a-f]+)" % mod.upper(),
                   r"\g<1>seg%d\g<2>" % seg, f, count=1)
        out.append(f); used += len(f)
    return "".join(out), seg, segmap


def subset_headers(outdir, stem, threshold=150):
    """The shared <STEM>.H holds every cross-module function prototype, and every
    module includes it. For a HIGHLY-CONNECTED module (USER: 325 protos) that
    loads hundreds of symbols into C 3.0's P1 table on EVERY file -> universal
    'Stack overflow'. Split the header: keep its small non-prototype CORE (guards,
    includes, comments), and inline into EACH module ONLY the prototypes for the
    functions IT calls — the same fix the WRITE graft preamble needed. Only fires
    when the header is bloated past `threshold` (so KERNEL/GDI/drivers, which
    compile fine, are untouched). Returns the prototype count moved (0 if skipped)."""
    hpath = os.path.join(outdir, stem + ".H")
    if not os.path.exists(hpath):
        return 0
    protos, core = {}, []
    for line in open(hpath, encoding="utf-8", errors="replace").read().splitlines():
        m = re.match(r'^\s*(?:extern\s+)?[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*'
                     r'\([^;{]*\)\s*;\s*$', line)
        if m and not line.lstrip().startswith(("typedef", "#", "/*", "*", "//")):
            protos[m.group(1)] = line.strip()
        else:
            core.append(line)
    if len(protos) < threshold:
        return 0
    open(hpath, "w", encoding="utf-8", newline="").write("\n".join(core) + "\n")
    inc = re.compile(r'(#include\s+"%s\.[Hh]")' % re.escape(stem))
    for fn in os.listdir(outdir):
        if not fn.lower().endswith(".c"):
            continue
        p = os.path.join(outdir, fn)
        txt = open(p, encoding="utf-8", errors="replace").read()
        called = set(re.findall(r'\b([A-Za-z_]\w*)\s*\(', txt))
        needed = [protos[n] for n in sorted(called) if n in protos]
        if needed and inc.search(txt):
            txt = inc.sub(lambda mm: mm.group(1) + "\n" + "\n".join(needed),
                          txt, count=1)
            open(p, "w", encoding="utf-8", newline="").write(txt)
    return len(protos)


def lifted_bodies(mod):
    """{(seg,off): (name, nargs)} for every lifted function of `mod`."""
    path = os.path.join(layout.recon_sub("auto_lift", ROOT), mod.lower() + "_lifted.c")
    text = open(path, encoding="utf-8", errors="replace").read()
    bodies = {}
    for m in re.finditer(r"^long %s_s(\d+)_([0-9a-f]+)\(([^)]*)\)" % mod.lower(),
                         text, re.M):
        seg, off, args = int(m.group(1)), int(m.group(2), 16), m.group(3)
        nargs = 0 if args.strip() in ("void", "") else args.count(",") + 1
        bodies[(seg, off)] = ("%s_s%d_%04x" % (mod.lower(), seg, off), nargs)
    return text, bodies


def export_wrappers(mod, ne, bodies, segmap=None, maxexp=None):
    """FAR PASCAL entry per exported ordinal, forwarding to its lifted body. Each
    wrapper's banner carries the body's PSEUDO-segment (segmap), so segsplit puts
    the wrapper in the same distributed segment as its body (a near forward call)
    instead of piling all wrappers into seg1 -> _TEXT overflow on a big module.
    maxexp caps the export count (a diagnostic knob for the LINK4 capacity wall)."""
    segmap = segmap or {}
    out, exports = [], []
    ents = [e for e in ne.entries() if e.get("kind") in ("MOVEABLE", "FIXED")]
    if maxexp is not None:
        ents = ents[:maxexp]
    for e in ents:
        ordv, seg, off = e["ordinal"], e["seg"], e["offset"]
        exp = "%s_%d" % (mod.upper(), ordv)
        pseg = segmap.get((seg, off), 1)
        body = bodies.get((seg, off))
        if body:
            nm, nargs = body
            decl = ", ".join("WORD a%d" % i for i in range(nargs)) or "void"
            call = ", ".join("a%d" % i for i in range(nargs))
            out.append("/* %s seg%d:0x%04x */\nlong FAR PASCAL %s(%s) "
                       "{ return %s(%s); }   /* @%d */"
                       % (mod.upper(), pseg, off, exp, decl, nm, call, ordv))
        else:
            out.append("/* %s seg%d:0x%04x */\nlong FAR PASCAL %s() "
                       "{ return 0; }   /* @%d NOT LIFTED (hw/int) */"
                       % (mod.upper(), pseg, off, exp, ordv))
        exports.append((exp, ordv, bool(body)))
    return "\n".join(out) + "\n", exports


def build_bat(outdir, stem, mods, compile_only=None):
    """compile_only=None recompiles every module; otherwise only that subset is
    (re)compiled and the rest keep their existing OBJ — so a relink pass that only
    changed the stub module recompiles ONE file, not all ~400 (USER is huge)."""
    bat = ["@echo off", "echo dll build > build.log"]
    for m in mods:
        if compile_only is not None and m not in compile_only:
            continue
        bat += ["del %s.obj" % m,
                "msc /u /AS /Gsw /Oas /Zpe /Ze /NT %s %s; > %s.out 2> %s.err"
                % (mkdef.seg_name(link16.seg_of_module(m, stem)), m, m, m),
                "if not exist %s.obj echo   NO-OBJ %s >> build.log" % (m, m)]
    bat += ["del %s.exe" % stem,
            "link4 @%s.LNK > link.txt 2> link.err" % stem,
            "if exist %s.exe echo   EXE-OK >> build.log" % stem,
            "if not exist %s.exe echo   NO-EXE >> build.log" % stem,
            "echo DONE >> build.log"]
    open(os.path.join(outdir, "BUILD.BAT"), "w",
         newline="\r\n").write("\n".join(bat) + "\n")


def write_dll_lnk(outdir, stem, objs):
    """LINK4 response file for a DLL: object list, run file, map, the Windows C
    runtime helpers (swlibc; NO slibw app-startup — a DLL has no WinMain), and
    the LIBRARY .DEF."""
    lines = []
    for i in range(0, len(objs), 2):
        lines.append(" ".join(objs[i:i + 2]) + (" +" if i + 2 < len(objs) else ""))
    lnk = "\n".join(lines) + "\n"
    lnk += "%s.exe /ALIGN:4 /NOD\n%s.map/map\nswlibc\n%s.def\n" % (stem, stem, stem)
    open(os.path.join(outdir, stem + ".LNK"), "w", newline="\r\n").write(lnk)


def run_dosbox(outdir):
    conf = os.path.join(B16, "_dll.conf")
    open(conf, "w").write(CONF.format(c_root=B16, wd=os.path.basename(outdir)))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=1800, capture_output=True)
    except subprocess.TimeoutExpired:
        print("*** DOSBox-X timed out")


def dll_compare(built_path, mod, shipped_ne):
    """Structural compare: the built DLL NE vs the shipped raw module NE
    (loaded via cfg.open_any, which handles the MZ-less boot-image format)."""
    from src.formats import ne as nemod
    b = nemod.NEFile(built_path)
    s = shipped_ne
    def exps(n):
        return sorted(e["ordinal"] for e in n.entries()
                      if e.get("kind") in ("MOVEABLE", "FIXED"))
    rows = [
        ("library flag 0x8000", b.h["flags"] & 0x8000, s.h["flags"] & 0x8000),
        ("errors-in-image 0x2000", b.h["flags"] & 0x2000, s.h["flags"] & 0x2000),
        ("segments", len(b.segments()), len(s.segments())),
        ("exported ordinals", exps(b), exps(s)),
    ]
    print("  %-26s %-16s %-16s" % ("FIELD", "BUILT", "SHIPPED"))
    for name, bv, sv in rows:
        mark = "" if bv == sv else "   <-- differs"
        print("  %-26s %-16s %-16s%s" % (name, str(bv), str(sv), mark))


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 1
    mod = sys.argv[1].upper()
    stem = sys.argv[sys.argv.index("--stem") + 1].upper() \
        if "--stem" in sys.argv else STEM.get(mod, mod[:3])
    maxexp = int(sys.argv[sys.argv.index("--maxexp") + 1]) \
        if "--maxexp" in sys.argv else None    # diagnostic: cap the export count
    budget = int(sys.argv[sys.argv.index("--budget") + 1]) \
        if "--budget" in sys.argv else 22000   # source bytes per pseudo-segment
    if mod not in cfg.SYSMODS:
        print("%s is not a system module (%s)" % (mod, ", ".join(cfg.SYSMODS)))
        return 1
    ne = cfg.open_any(mod)
    outdir = os.path.join(B16, stem + "D")           # <=8 chars for DOS 8.3
    link16.rmtree_retry(outdir)
    os.makedirs(outdir)

    # SOURCE = lifted file + FAR PASCAL export wrappers, split BEFORE compiling so
    # segsplit makes the wrapper->body calls cross-module-visible.
    lift_text, bodies = lifted_bodies(mod)
    if "--maxfuncs" in sys.argv:          # diagnostic: keep only the first N funcs
        N = int(sys.argv[sys.argv.index("--maxfuncs") + 1])
        parts = re.split(r"(?m)^(?=long %s_s\d+_[0-9a-f]+\()" % mod.lower(), lift_text)
        lift_text = parts[0] + "".join(parts[1:N + 1])
        print("  [diag] capped to first %d lifted functions" % N)
    # A module whose single lifted segment would overflow 64KB compiled is spread
    # across pseudo-segments (KERNEL/GDI/USER); small drivers pass through.
    nsegs, segmap = 1, {}
    if len(lift_text) > 30000:
        lift_text, nsegs, segmap = distribute_segments(lift_text, mod, budget)
        print("  %s lift is large -> distributed across %d pseudo-segments"
              % (mod, nsegs))
    wrappers, exports = export_wrappers(mod, ne, bodies, segmap, maxexp=maxexp)
    # A DLL has no app entry, but LINK4's default startup (from swlibc, which we
    # need for the 32-bit-arith helpers __ulmul etc.) references _main. Satisfy
    # it with a stub so the image links CLEAN (no 0x2000 errors-in-image bit) —
    # a flagged deviation: the retail driver is hand ASM with a LibEntry, not a
    # C startup. (Structural build, same standard as the apps.)
    # split multi-declarator push snapshots (`WORD _p0, _p1;` mid-block, which
    # C 3.0 rejects) — the lift emits them, and unlike the app RECONSTRUCTION the
    # DLL source IS the lift, so it needs the same fix the graft applies.
    combined = (graft.split_multidecl(lift_text.rstrip())
                + "\n\n/* ---- exported ordinals ---- */\n" + wrappers
                + "\nint main() { return 0; }   /* stub: satisfies the C startup */\n")
    work = os.path.join(outdir, "_src.c")
    open(work, "w", encoding="utf-8", newline="").write(combined)
    subprocess.run([sys.executable, os.path.join(HERE, "hoistdecl.py"), work],
                   check=True, capture_output=True)
    subprocess.run([sys.executable, os.path.join(HERE, "segsplit.py"), work,
                    outdir, "--stem", stem, "--maxbytes", "3000"],
                   check=True, capture_output=True)
    os.remove(work)
    for fn in sorted(os.listdir(outdir)):
        if fn.lower().endswith((".c", ".h")):
            subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"),
                            os.path.join(outdir, fn)], capture_output=True)
    moved = subset_headers(outdir, stem)
    if moved:
        print("  header had %d cross-module protos -> subsetted per module "
              "(P1-overflow fix)" % moved)

    deftext, info = mkdef.build(mod, os.path.join("modules", mod + ".ne"),
                                stem=stem, ne=ne)
    # rewrite the .DEF EXPORTS to EXACTLY the wrapper set (so a --maxexp cap keeps
    # .DEF and code consistent; also drops any garbage the name tables held).
    head = deftext.split("EXPORTS", 1)[0].rstrip()
    exp_lines = "\n".join("    %-18s @%d" % (exp, ordv)
                          for exp, ordv, _ in sorted(exports, key=lambda x: x[1]))
    deftext = head + "\n\nEXPORTS\n" + exp_lines + "\n"
    # a .DEF is plain DOS text; a stripped module's description (and mkdef's own
    # em-dash comment) can carry non-ASCII -> encode ASCII with replacement so
    # LINK4 gets clean text.
    open(os.path.join(outdir, stem + ".DEF"), "wb").write(
        deftext.replace("\n", "\r\n").encode("ascii", "replace"))

    nlift = sum(1 for _, _, b in exports if b)
    print("%s: %d exports (%d lifted, %d stub)"
          % (mod, len(exports), nlift, len(exports) - nlift))

    def do_build(compile_only=None):
        mods = sorted(os.path.splitext(f)[0] for f in os.listdir(outdir)
                      if f.lower().endswith(".c"))
        write_dll_lnk(outdir, stem, [m + ".obj" for m in mods])
        build_bat(outdir, stem, mods, compile_only=compile_only)
        run_dosbox(outdir)
        return mods

    def link_text():
        t = ""
        for lt in ("LINK.TXT", "LINK.ERR"):
            p = os.path.join(outdir, lt)
            if os.path.exists(p):
                t += open(p, encoding="cp437", errors="replace").read()
        return t

    mods = do_build()
    # STUB LOOP: the lifted bodies call things that are not provided — internal
    # callees that did not themselves lift (sub_XXXX, mod_sN_XXXX, far_XXXX),
    # Windows APIs (no import lib in this standalone build), and OTHER system
    # modules' ordinals (KERNEL/GDI/USER cross-imports). Anything STILL unresolved
    # after swlibc is, by definition, unprovided, so stub it — no duplicate risk.
    # (The C-runtime helpers __ulmul etc. ARE resolved by swlibc, so they never
    # reach here.) Relink until clean; `_main` is the source stub already.
    stubbed = set()
    for _pass in range(6):
        # DROP modules that failed to compile — a lifted function too big for MS
        # C 3.0's P1 table ("Stack overflow"). Its missing OBJ would sink the
        # link; delete the .C so the functions it defined become unresolved, and
        # the stub logic below resolves them (a flagged limit, as in the apps).
        newdrop = 0
        for m in sorted(os.path.splitext(f)[0] for f in os.listdir(outdir)
                        if f.lower().endswith(".c")):
            if m == stem + "S":
                continue
            if not os.path.exists(os.path.join(outdir, m + ".OBJ")):
                cp = os.path.join(outdir, m + ".C")
                if os.path.exists(cp):
                    os.remove(cp); newdrop += 1
        if newdrop:
            print("  dropped %d P1-overflow module(s), re-linking..." % newdrop)
            mods = do_build(compile_only=[])      # just relink; OBJs unchanged
        import mkstubs
        unres = [s for s in mkstubs.parse_unresolved(os.path.join(outdir, "LINK.TXT"))
                 if s.upper() != "WINMAIN"]
        fresh = [s for s in unres if s not in stubbed]
        if not fresh and not newdrop:
            break
        stubbed.update(fresh)
        # NO includes in the stub module: a stubbed name may collide with a
        # windows.h MACRO (GLOBALALLOC etc.) — without the header it is just a
        # plain identifier. Convention matters for the SYMBOL NAME: an unresolved
        # in the LINK.TXT is already the DECORATED name — an underscore prefix
        # (`_sub_0200`) means C convention (strip it, re-add via a C stub); an
        # UNDECORATED all-caps name (`GLOBALALLOC`, a FAR PASCAL Windows API /
        # cross-module ordinal) needs a FAR PASCAL stub so C 3.0 emits the same
        # bare uppercase symbol — a plain C stub would emit `_GLOBALALLOC` and
        # leave the far-pascal reference unresolved.
        lines = []
        for s in sorted(stubbed):
            if s.startswith("_"):
                nm = s[1:]
                if re.match(r"^[A-Za-z_]\w*$", nm):
                    lines.append("long %s() { return 0; }" % nm)
            elif re.match(r"^[A-Za-z_]\w*$", s):
                # lowercase KEYWORDS (/Ze), not the FAR/PASCAL windows.h macros —
                # the stub module has no includes.
                lines.append("long far pascal %s() { return 0; }" % s)
        open(os.path.join(outdir, stem + "S.C"), "w", encoding="utf-8",
             newline="").write("\n".join(lines) + "\n")
        for fn in (stem + "S.C",):
            subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"),
                            os.path.join(outdir, fn)], capture_output=True)
        print("  stubbing %d un-lifted callee(s), relinking..." % len(stubbed))
        mods = do_build(compile_only=[stem + "S"])   # only the stub module changed

    nobj = [m for m in mods if not os.path.exists(os.path.join(outdir, m + ".OBJ"))]
    print("modules: %d compiled, %d failed%s" % (len(mods) - len(nobj), len(nobj),
          ("  (" + ", ".join(nobj) + ")") if nobj else ""))
    txt = link_text()
    unres = re.findall(r"^\s*([A-Za-z_@][\w@]*)\s+in file\(s\)", txt, re.M)
    if unres:
        print("UNRESOLVED (%d): %s%s" % (len(unres), ", ".join(unres[:12]),
              " ..." if len(unres) > 12 else ""))
    exe = os.path.join(outdir, stem + ".EXE")
    if os.path.exists(exe):
        # LINK4 sometimes sets the errors-in-image bit (0x2000) even on a
        # verified-clean link (same false positive the apps hit). Clear it when
        # there are no unresolved and no intersegment fixups.
        if not unres and "Intersegment self-relative fixup" not in txt:
            if link16.clear_error_flag_if_set(exe):
                print("  (cleared spurious errors-in-image flag)")
        print("\nLINKED DLL: %s (%d bytes)" % (exe, os.path.getsize(exe)))
        dll_compare(exe, mod, ne)
    else:
        print("\nno DLL produced")
        # surface the first compile error to guide the next iteration
        for m in nobj[:1]:
            ep = os.path.join(outdir, m + ".ERR")
            if os.path.exists(ep):
                print("--- %s.ERR ---" % m)
                print(open(ep, encoding="cp437", errors="replace").read()[:800])
    return 0


if __name__ == "__main__":
    sys.exit(main())
