#!/usr/bin/env python3
"""
graft.py — MECHANICAL app transcription: resolve an app's stubs by grafting in
lift.py's per-instruction bodies for them.

A stub (mkstubs.py) is a function the hand-reconstruction CALLS but has not
transcribed, currently an empty body returning 0. lift.py has, for many of them,
a full per-instruction transliteration in reconstructed/auto_lift/<app>_lifted.c
(functions named <app>_s<seg>_<offset>). This tool pulls those bodies in so the
stub does the RECOVERED thing instead of nothing — turning a hollow app closer to
a working one, mechanically.

What it does, per app:
  1. Read the stub list (Call_0xNNNN and named stubs) and their addresses.
  2. Read the lift file: {offset: body}, and each body's callees (other lifted
     offsets it references).
  3. Read the app source: the set of offsets the app ALREADY defines (from the
     `segN:0xoffset` banners) — the closure must STOP at these (they resolve to
     hand-written code, and re-defining them would clash).
  4. Transitive closure from each stub target, following callees, stopping at
     app-defined offsets. That is the exact set of lifted functions to graft.
  5. Emit <STEM>_graft.c: the closure's bodies, each aliased to the name the app
     calls (Call_0xNNNN), with lift globals g_XXXX mapped to the app's DS-offset
     convention (ds_0xNNNN), ready for hoistdecl + ansi2knr + compile.

HONEST LIMITS, stated up front:
  - Lifted globals g_XXXX and the hand-code's ds_0xNNNN are DIFFERENT symbols for
    the same DS cell. We map g_00ce -> ds_0x00ce so grafted functions share state
    with EACH OTHER and, where the app declares that ds_ global, with the app.
    Where the app uses a NAMED global (hInstCal) for a cell, the graft still uses
    ds_ and the two diverge — flagged, not silently wrong.
  - Only FULLY-lifted functions exist (lift.py skips unsupported opcodes), so a
    stub with no lifted body is left as a stub and reported.
  - This is mechanical recovery, NOT human interpretation: the grafted code is
    correct-by-construction but unreadable (loc_N, goto). A function still only
    "graduates" when a person names and understands it. This just makes the app
    behave.

Usage:  python graft.py reversi           # print the plan
        python graft.py reversi --emit build16/REVLK   # write REV_graft.c
"""
import os
import re
import sys

from engine.foundation import layout
HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE

# app -> stem, for locating the sidecar graftmap (backup-derived name->seg:off,
# recovered by gen_graftmap.py because the comment-strip deleted the inline
# `segN:0xXXXX` annotations parse_stubs used to read).
GRAFT_STEMS = {"calc": "CALC", "calendar": "CAL", "cardfile": "CARD",
               "clipbrd": "CLIP", "clock": "CLK", "control": "CTL",
               "notepad": "NOTE", "paint": "PNT", "reversi": "REV",
               "spooler": "SPL", "terminal": "TERM", "write": "WRT"}


def _win_names():
    """Every identifier declared (or macro-defined) in the period windows.h /
    winexp.h — the graft preamble already includes those, so they must NOT be
    re-declared FAR (would clash). Everything ELSE a graft body calls but doesn't
    declare is an unresolved cross-segment target to FAR-declare."""
    names = set()
    for h in ("windows.h", "winexp.h"):
        p = os.path.join(ROOT, "build16", "INC30", h)
        if os.path.exists(p):
            t = open(p, encoding="cp437", errors="replace").read()
            names |= set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", t))
    return names


WIN_NAMES = _win_names()
_NOFAR = {"if", "for", "while", "switch", "return", "sizeof", "do", "else",
          "MAKELONG", "LOWORD", "HIWORD", "LOBYTE", "HIBYTE", "RGB", "MK_FP",
          "LOCALSIZE", "MAKEPOINT", "PALETTERGB", "MAKEINTRESOURCE", "FIELDOFFSET"}


def load_graftmap(app):
    stem = GRAFT_STEMS.get(app, app[:4].upper())
    p = os.path.join(layout.recon_sub("graftmaps", ROOT), stem + ".graftmap")
    m = {}
    if os.path.exists(p):
        for line in open(p, encoding="utf-8"):
            parts = line.split()
            if len(parts) == 3:
                m[parts[0]] = (int(parts[1]), int(parts[2], 16))
    return m

# Cap on graft modules placed in one _TEXT segment. Grafts go to _TEXT (seg1) for
# the __ulmul-near reason, so their COMPILED code must fit ONE 64KB segment. The
# 1985 LINK4 linked terminal's 108 and calendar's 121 grafts clean (calendar's
# 121 = 452KB of lifted source -> under 64KB compiled), but WRITE's functions are
# far DENSER (it's the biggest app, with 16KB-body monsters): 120 of them overflow
# _TEXT and LINK4 emits no EXE. Source-byte budgeting doesn't transfer across apps
# (WRITE's ~350KB failed where calendar's 452KB linked), so WRITE — the sole
# outlier — gets an explicit lower cap; every other app stays effectively uncapped.
# The over-cap tail is capped breadth-first (logic nearest the entry points kept)
# and falls through to Y0 stubs.
MAX_CLOSURE = 200                       # generous default: never bites normal apps
PER_APP_CAP = {}                        # grafts now DISTRIBUTE to their real segments
                                        # (link16 uses graft_segs), so the old single-
                                        # _TEXT 64KB ceiling is gone. MAX_CLOSURE alone
                                        # bounds the total; re-add a per-app cap only if
                                        # some individual segment still overflows.
LIFT = layout.recon_sub("auto_lift", ROOT)


def lift_path(app):
    for c in [os.path.join(LIFT, app + "_lifted.c")]:
        if os.path.exists(c):
            return c
    import glob
    g = glob.glob(os.path.join(LIFT, app[:5] + "*_lifted.c"))
    return g[0] if g else None


def parse_lift(text):
    """-> {(seg,off): (name, full_text, {(seg,off) callees})}. Segment-aware, so
    a multi-segment app (two segments can share an offset) matches correctly."""
    fns = {}
    for m in re.finditer(r"([A-Za-z_][\w \t\*]*?\b[A-Za-z_]\w*_s(\d+)_([0-9a-f]+)"
                         r"\s*\([^;{]*\))\s*(?:/\*.*?\*/\s*)*\{", text, re.S):
        name = re.search(r"([A-Za-z_]\w*_s\d+_[0-9a-f]+)", m.group(1)).group(1)
        seg, off = int(m.group(2)), int(m.group(3), 16)
        depth, j = 0, m.end() - 1
        while j < len(text):
            if text[j] == "{":
                depth += 1
            elif text[j] == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        body = text[m.start():j + 1]
        explicit = set()
        for cs, co in re.findall(r"_s(\d+)_([0-9a-f]+)\s*\(", body):
            explicit.add((int(cs), int(co, 16)))
        short = set(int(co, 16) for co in
                    re.findall(r"\bsub_([0-9a-f]+)\s*\(", body))
        fns[(seg, off)] = [name, body, explicit, short]
    # Second pass: resolve each short `sub_XXXX` to a REAL (seg,off). The lift's
    # short form drops the segment, so a CROSS-segment call (a seg2 function
    # calling seg4:0x160b) would wrongly be read as intra-segment (2,0x160b) —
    # which is not a function, so the closure misses it AND far-derivation never
    # marks the real target far (near cross-segment call -> fixup). Resolve to
    # the caller's own segment if it defines that offset, else any segment that
    # does.
    all_offs = {}
    for (s, o) in fns:
        all_offs.setdefault(o, []).append(s)
    out = {}
    for (seg, off), (name, body, explicit, short) in fns.items():
        callees = set(explicit)
        for co in short:
            segs = all_offs.get(co, [])
            if seg in segs:
                callees.add((seg, co))
            elif segs:
                callees.add((segs[0], co))         # cross-segment short call
        callees.discard((seg, off))
        out[(seg, off)] = (name, body, callees)
    return out


def app_defined(src):
    """{(seg,off)} the hand-reconstruction actually DEFINES a function at — the
    home segN:0xoffset of every real function body it contains. The closure
    stops here: the app provides these, so re-grafting them would duplicate the
    logic (and a name clash for exports). A bare seg mention in prose is NOT a
    definition, so we read the home off each actual function unit, not every
    seg:offset in the file."""
    import segsplit
    out = set()
    for kind, a, z in segsplit.scan_top_level(src):
        if kind != "func":
            continue
        # the home is the FIRST segN:0xXXXX in the unit (its banner/header)
        m = re.search(r"seg(\d+):0x([0-9a-fA-F]+)", src[a:z])
        if m:
            out.add((int(m.group(1)), int(m.group(2), 16)))
    return out


def parse_stubs(stub_src, app_src, default_seg=1, entry_map=None, graftmap=None):
    """-> {stub_name: (seg, off)} for EVERY stub form:
       Call_segN_XXXX  -> (N, XXXX)                     (segment explicit)
       Call_0xNNNN     -> (default_seg, NNNN)           (offset only)
       Sub_0xNNNN      -> (default_seg, NNNN)
       Named (SetCurCard) -> the app's forward-decl `NAME(...); /* segN:0xXXXX */`
    Named stubs are the majority in the big multi-segment apps; without them the
    grafter only ever saw the offset-named single-segment apps.

    seg 255 is the reconstruction's PLACEHOLDER for "this is an NE entry-table
    ORDINAL, real segment unknown" (WRITE's Entry_00NN, annotated seg255:0x00NN).
    With entry_map={ordinal: (seg, off)} from the NE, resolve the ordinal to its
    real home so the lift body (write_sSEG_OFF) can be found."""
    out = {}
    # forms encoding the address in the NAME
    for m in re.finditer(r"\b(Call_seg(\d+)_([0-9a-f]+))\b", stub_src):
        out[m.group(1)] = (int(m.group(2)), int(m.group(3), 16))
    # offset-only: segment unknown here (None); plan() resolves it against the
    # lift — a multi-segment app (calendar) has Call_0xNNNN targets in seg3/6,
    # not seg1, so a fixed default would silently miss them.
    for m in re.finditer(r"\b((?:Call|Sub)_0x([0-9a-f]+))\b", stub_src):
        out.setdefault(m.group(1), (None, int(m.group(2), 16)))
    # NAMED stubs: every `<type> FAR <NAME>()` / `<type> FAR * <NAME>()` in the
    # stub module that is not a Call_/Sub_ form. Resolve its address from the
    # app's own forward declaration `... NAME ( ... ) ; /* segN:0xXXXX */`.
    for m in re.finditer(r"(?m)^[A-Za-z_][\w \t\*]*?\bFAR\s*\*?\s*([A-Za-z_]\w*)"
                         r"\s*\(\s*\)\s*(?:\{|;)", stub_src):
        nm = m.group(1)
        if nm.startswith(("Call_", "Sub_")) or nm in out:
            continue
        d = re.search(r"\b" + re.escape(nm) + r"\s*\([^;{]*\)\s*;?[^\n]*?"
                      r"seg(\d+):0x([0-9a-f]+)", app_src)
        if d:
            out[nm] = (int(d.group(1)), int(d.group(2), 16))
    # graftmap: backup-derived name->(seg,off). The comment-strip removed the
    # `segN:0xXXXX` annotations the named-stub loop above reads from app_src, so
    # resolve any stub whose address survives in the sidecar map. Catches every
    # named stub form (incl. WinMain, which the FAR-only loop misses).
    if graftmap:
        # Match the function HEADER only (`TYPE NAME(params)`), not requiring `{`
        # right after `)`: ansi2knr turns stubs K&R, so WinMain has param decls
        # (`HANDLE hInstance;`) between `)` and `{`.
        for m in re.finditer(r"(?m)^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)"
                             r"\s*\([^;{)]*\)", stub_src):
            nm = m.group(1)
            if nm not in out and nm in graftmap:
                out[nm] = graftmap[nm]
    # Resolve the seg-255 ordinal placeholder to the real (seg, off) via the NE
    # entry table. Applies to every stub form uniformly (the offset field is the
    # entry ordinal, not a code offset).
    if entry_map:
        for nm, (s, o) in list(out.items()):
            if s == 255 and o in entry_map:
                out[nm] = entry_map[o]
    return out


def map_globals(body):
    """Rewrite lift globals g_XXXX -> the app's ds_0xXXXX convention, so a
    grafted function shares the DS cell with the hand-code that declares it."""
    return re.sub(r"\bg_([0-9a-f]{2,4})\b",
                  lambda m: "ds_0x%x" % int(m.group(1), 16), body)


def split_multidecl(body):
    """Split a multi-declarator local (`WORD _p0, _p1;`) into single decls.
    hoistdecl only moves single-declarator decls to the block top; a
    multi-declarator one left mid-block is rejected by C 3.0 ("type WORD
    unexpected"). The lift emits `WORD _p0, _p1;  /* push snapshots */` after
    statements, so split it so hoistdecl can hoist each part."""
    def repl(m):
        typ, decls = m.group(1), m.group(2)
        parts = [d.strip() for d in decls.split(",")]
        if len(parts) < 2 or any("=" in p or "(" in p for p in parts):
            return m.group(0)
        return " ".join("%s %s;" % (typ, p) for p in parts)
    return re.sub(r"\b(WORD|int|BYTE|long|char|DWORD)\s+"
                  r"([A-Za-z_]\w*(?:\s*,\s*[A-Za-z_]\w*)+)\s*;", repl, body)


def entry_map_for(app):
    """{ordinal: (seg, off)} from the app binary's NE entry table, for resolving
    the seg-255 ordinal placeholder. Returns {} if the binary can't be opened."""
    import cfg
    for name in (app, app.upper(), app.lower()):
        try:
            ne = cfg.open_any(name)
        except Exception:
            continue
        return {e["ordinal"]: (e["seg"], e["offset"])
                for e in ne.entries()
                if e.get("kind") in ("MOVEABLE", "FIXED")}
    return {}


def plan(app, stub_src, lift_text, src, entry_map=None):
    lift = parse_lift(lift_text)                       # {(seg,off): (...)}
    defined = app_defined(src)                         # {(seg,off)}
    if entry_map is None:
        entry_map = entry_map_for(app)
    stubs = parse_stubs(stub_src, src, entry_map=entry_map,
                        graftmap=load_graftmap(app))          # {name: (seg,off)}
    # resolve segment-unknown (None) stubs against the lift, by their offset
    by_off = {}
    for (s, o) in lift:
        by_off.setdefault(o, []).append(s)
    for nm, (s, o) in list(stubs.items()):
        if s is None:
            cands = by_off.get(o, [])
            stubs[nm] = (cands[0], o) if len(cands) == 1 else \
                        ((cands[0], o) if cands else (None, o))

    # BREADTH-FIRST closure from the stub targets, with a CAP. The 1985 LINK4
    # cannot build an unbounded number of graft modules into one _TEXT segment
    # (grafts go there for the __ulmul-near reason): terminal (108) and calendar
    # (121) linked clean, but WRITE's full self-import closure (234) produced no
    # EXE. So expand breadth-first — keeping the logic CLOSEST to the entry
    # points — and stop at MAX_CLOSURE; the un-grafted tail falls through to Y0
    # stubs. An honest bound: "as much recovered logic as the period linker fits
    # in one code segment," flagged, not silently truncated.
    import collections as _c
    cap = PER_APP_CAP.get(app.lower(), MAX_CLOSURE)
    need, seen, missing = set(), set(), {}
    work = _c.deque()
    for nm, key in stubs.items():
        if key in lift:
            work.append(key)
        else:
            missing[nm] = key                          # no lifted body
    capped = 0
    while work:
        key = work.popleft()                           # BFS: nearest first
        if key in seen:
            continue
        seen.add(key)
        if len(need) >= cap:
            capped += 1
            continue                                   # tail -> Y0 stub
        need.add(key)
        for c in lift[key][2]:
            if c in lift and c not in seen and c not in defined:
                work.append(c)
    if capped:
        print("  closure capped at %d (of %d reachable); %d deep-tail functions "
              "stay stubbed — exceeds the 1985 linker's one-segment capacity"
              % (len(need), len(need) + capped, capped))
    return lift, stubs, need, missing


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 1
    app = sys.argv[1].lower()
    lf = lift_path(app)
    if not lf:
        print("no lift file for %s" % app); return 1
    # PAINT and WRITE are reconstructed across two files; concatenate them so a
    # named stub's forward declaration and every app-defined home is visible.
    MULTI = {"paint": ["paint_core.c", "paint_tools.c"],
             "write": ["write_dlg.c", "write_wnd.c"]}
    srcs = MULTI.get(app, [app + ".c"])
    src = ""
    for s in srcs:
        p = os.path.join(layout.recon_dir(ROOT), s)
        if os.path.exists(p):
            src += open(p, encoding="utf-8", errors="replace").read() + "\n"
    # locate the stub module — explicit app -> build dir (the stem is not always
    # a prefix of the app name: control -> CTLLK, paint -> PNTLK).
    import glob
    STEM = {"calc": "CALC", "calendar": "CAL", "cardfile": "CARD",
            "clipbrd": "CLIP", "clock": "CLK", "control": "CTL",
            "notepad": "NOTE", "paint": "PNT", "reversi": "REV",
            "spooler": "SPL", "terminal": "TERM", "write": "WRT"}
    stem = STEM.get(app, app[:4].upper())
    xf = os.path.join(ROOT, "build16", stem + "LK", stem + "X.C")
    if not os.path.exists(xf):
        g = glob.glob(os.path.join(ROOT, "build16", stem + "LK", "*X.C"))
        xf = g[0] if g else None
    stub_src = open(xf, encoding="utf-8", errors="replace").read() if xf else ""
    lift_text = open(lf, encoding="utf-8", errors="replace").read()

    lift, stubs, need, missing = plan(app, stub_src, lift_text, src)
    print("%s: %d stubs, %d have lifted bodies, closure = %d functions to graft"
          % (app, len(stubs), len(stubs) - len(missing), len(need)))
    if missing:
        print("  NO lifted body (stay stubbed): "
              + ", ".join(sorted(missing)))
    if "--emit" in sys.argv:
        outdir = sys.argv[sys.argv.index("--emit") + 1]
        xbase = os.path.basename(glob.glob(os.path.join(outdir, "*X.C"))[0])
        stem = xbase[:-2].rstrip("X") or xbase[:-2]      # REVX.C -> REV
        out = [
            "/* %s_graft.c — MECHANICALLY GRAFTED lifted bodies (graft.py).",
            " * These resolve the app's Call_0xNNNN stubs with lift.py's",
            " * per-instruction transliteration. NOT human-interpreted: the code",
            " * is correct-by-construction but unreadable. Globals g_XXXX are",
            " * mapped to the ds_0xNNNN convention. See graft.py header. */",
            "#include <windows.h>",
            "#include <winexp.h>",     # grafts call lstrlen/_lopen etc. (far
                                       # PASCAL) — without it they compile the
                                       # C-convention `_lstrlen` and go unresolved
            '#include "%s.h"' % stem,
            # lift labels the compiler's stack-check/frame-probe call `LOCALSIZE`
            # (`ax = LOCALSIZE(framesize, ...)`). The recompiled C manages its own
            # stack, so make it a harmless no-op instead of an unresolved external.
            "#define LOCALSIZE(a, b) 0L", ""]
        # Rename each grafted function so a stub TARGET carries the exact name
        # the app calls (Call_0xNNNN) — the app's own call arguments then flow
        # straight into the lifted body (no wrapper, no arg-count mismatch). A
        # non-target closure member keeps its lift name. Internal callee refs are
        # rewritten to match, so the whole grafted set links consistently.
        key2name = {}                                # (seg,off) -> stub name
        for nm, key in stubs.items():
            key2name[key] = nm
        emit_name = {}
        for key in need:
            emit_name[key] = key2name.get(key, lift[key][0])
        # FAR-derivation for the closure (segsplit did this for the hand-code,
        # but grafts are added after the split). A closure member CALLED from a
        # different segment must be FAR — a near call cannot cross a segment
        # ("Intersegment self-relative fixup"). Stub targets are always FAR
        # (called from app modules in various segments).
        far = set(k for k in need if k in key2name)
        for (cseg, coff) in need:
            for callee in lift[(cseg, coff)][2]:
                if callee in need and callee[0] != cseg:
                    far.add(callee)
        # Emit ONE FUNCTION PER MODULE. A single lifted function's register
        # block + dozens of loc_N locals and labels already loads the C 3.0
        # front-end table heavily; several in one file overflow it ("Stack
        # overflow"). Per-module also mirrors how the reconstruction is split.
        # Also emit a shared header so cross-graft calls have prototypes.
        base_preamble = "\n".join(out)     # includes (+ maybe an inlined _mkpt)
        # _mkpt (a POINT constructor some lifted bodies use) becomes a SINGLE FAR
        # definition in the ZLM module below -- NOT a per-module `static` copy. The
        # copies made LINK4 warn "_mkpt defined more than once": MS C 3.0 emits the
        # static under a content-hashed public name, so byte-identical bodies in
        # different object files collide. Strip any inlined copy and declare it FAR
        # (a POINT-returning far call; DS/near-data return convention is unchanged).
        base_preamble = re.sub(r"(?m)^\s*static POINT _mkpt\b[^\n]*\n", "",
                               base_preamble)
        # FAR prototypes for the ZLM helpers: the _mkpt POINT-constructor and the
        # long-math wrappers _ulmul_f/_uldiv_f/_ulrem_f, all defined FAR in _TEXT
        # (the <stem>ZLM module below). lift.py emits calls to these instead of a
        # per-module static _mkpt / raw DWORD * / % -- the compiler's own __ulmul/
        # __uldiv/__ulrem are NEAR in _TEXT and unreachable by a near call from a
        # graft in another segment. Injected PER-MODULE, only where the body calls
        # them, so the shared preamble stays minimal: extra prototypes load C 3.0's
        # P1 symbol table and can tip a boundary module into "Stack overflow".
        ZLM_PROTOS = {"_mkpt": "POINT FAR _mkpt();",
                      "_ulmul_f": "DWORD FAR _ulmul_f();",
                      "_uldiv_f": "DWORD FAR _uldiv_f();",
                      "_ulrem_f": "DWORD FAR _ulrem_f();"}
        # a prototype for every INTERNAL closure member (a transitive callee kept
        # under its lift name), so a graft->graft call links with the right
        # return type. The STUB TARGETS are skipped — the app's shared header
        # (included above) already declares those, and re-declaring them clashes
        # ("PutUpDB : redefinition"). Keyed by emit-name; each MODULE below pulls
        # in ONLY the ones its own body calls — a preamble that declared the whole
        # closure overflowed C 3.0's P1 symbol table once the closure grew large
        # (WRITE follows self-import edges to 234 members -> "Stack overflow" on
        # EVERY file). Declaring just the called ones keeps each unit small.
        proto_of = {}
        for key in need:
            if key in key2name:
                continue                    # a stub target: header has it
            _, body0, _ = lift[key]
            hd = split_multidecl(map_globals(body0)).split("{", 1)[0]
            hd = re.sub(r"\b" + re.escape(lift[key][0]) + r"\b",
                        emit_name[key], hd).strip()
            # Distributed grafts sit in DIFFERENT segments, so every graft->graft
            # call may cross a segment boundary — declare each internal member FAR
            # unconditionally (a FAR call to a same-segment target is still valid).
            if " FAR " not in hd:
                hd = re.sub(r"\b" + re.escape(emit_name[key]),
                            "FAR " + emit_name[key], hd, count=1)
            proto_of[emit_name[key]] = re.sub(r"\(.*", "();", hd, flags=re.S)

        made = []
        manifest = []            # (module, segment, resolved-stub-name-or-"")
        for i, key in enumerate(sorted(need)):
            seg, off = key
            _, body, _ = lift[key]
            body = split_multidecl(map_globals(body))
            # explicit `app_sN_XXXX(` -> that closure member's emit name
            for k2 in need:
                body = re.sub(r"\b" + re.escape(lift[k2][0]) + r"\b",
                              emit_name[k2], body)
            # short `sub_XXXX(` -> its closure member, same segment if present
            # else any segment (matching parse_lift's cross-segment resolution).
            def _sub(m):
                o = int(m.group(1), 16)
                for k3 in [(seg, o)] + [k for k in need if k[1] == o]:
                    if k3 in emit_name:
                        return emit_name[k3] + "("
                return m.group(0)
            body = re.sub(r"\bsub_([0-9a-f]+)\s*\(", _sub, body)
            # Every internal member is FAR in its DEFINITION too (matching its FAR
            # proto above), so a cross-segment call site and the body agree.
            if " FAR " not in body.split("{", 1)[0]:
                body = re.sub(r"^(\s*[A-Za-z_][\w \t\*]*?)\b"
                              + re.escape(emit_name[key]),
                              r"\1FAR " + emit_name[key], body, count=1)
            # ONLY the prototypes THIS body calls (minus its own name), so the
            # per-module symbol table stays small no matter how big the closure.
            called = set(re.findall(r"(?<![.\w])([A-Za-z_]\w*)\s*\(", body))
            called.discard(emit_name[key])
            protos = [proto_of[n] for n in sorted(called) if n in proto_of]
            # ZLM helper prototypes, ONLY the ones this body actually calls.
            protos += [ZLM_PROTOS[n] for n in ZLM_PROTOS if n in called]
            # Any OTHER called name that isn't a windows/winexp API, a macro/keyword,
            # or already a closure member is an unresolved cross-segment target that
            # lift left un-declared (the UPPERCASE app-name artifact, sub_XXXX, ...).
            # Declare it FAR so the DISTRIBUTED call is far (it links to a stub). A
            # FAR call to a same-segment target is still valid.
            far_ext = ["extern long FAR %s();" % n for n in sorted(called)
                       if n not in proto_of and n not in WIN_NAMES
                       and n not in _NOFAR and not n.startswith("_")]
            preamble = base_preamble + "\n".join(protos + far_ext) + "\n\n"
            base = "%sZ%d" % (stem, i)
            open(os.path.join(outdir, base + ".C"), "w", encoding="utf-8",
                 newline="").write(preamble + "\n" + body + "\n")
            made.append(base)
            manifest.append("%s %d %s"
                            % (base, seg, key2name.get(key, "")))
        # Long-math FAR-wrapper module, forced into _TEXT (seg 1) where the near
        # __ulmul/__uldiv/__ulrem helpers live. The wrappers' OWN calls to those
        # helpers stay in-segment (near, valid); grafts in other segments reach the
        # wrappers via a FAR call. This is the surgical fix for the compiler-emitted
        # long-math fixups that farfix cannot touch (no source token to FAR-ify).
        lm = "%sZLM" % stem
        open(os.path.join(outdir, lm + ".C"), "w", encoding="utf-8",
             newline="").write(
            "#include <windows.h>\n"
            "POINT FAR _mkpt(WORD _x, WORD _y) { POINT _p; _p.x = _x; _p.y = _y; return _p; }\n"
            "DWORD FAR _ulmul_f(DWORD _a, DWORD _b) { return _a * _b; }\n"
            "DWORD FAR _uldiv_f(DWORD _a, DWORD _b) { return _a / _b; }\n"
            "DWORD FAR _ulrem_f(DWORD _a, DWORD _b) { return _a % _b; }\n")
        made.append(lm)
        manifest.append("%s 1 " % lm)     # seg 1 = _TEXT
        # manifest: link16 --graft reads it to compile each module into the
        # SEGMENT its function was recovered from, and to know which stub names
        # the graft resolves (so mkstubs only stubs the remainder).
        open(os.path.join(outdir, stem + "GRAFT.LST"), "w",
             newline="").write("\n".join(manifest) + "\n")
        print("  -> %d graft modules (resolve %d stubs, +%d transitive callees)"
              % (len(made), sum(1 for k in need if k in key2name),
                 len(need) - sum(1 for k in need if k in key2name)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
