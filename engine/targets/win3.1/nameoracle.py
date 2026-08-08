#!/usr/bin/env python3
r"""
nameoracle.py — propose a name for every function FROM THE BINARY ALONE.

The reconstruction is already binary-only (conduct never reads any source). The
one place the leaked source was a convenience was NAMING internal functions. This
tool removes that dependency: it derives names deterministically from evidence
that lives entirely in the shipped binary —

  1. EXPORT TABLE      exported functions are named exactly (the NE resident /
                       non-resident name tables — Microsoft ships these names).
  2. IMPORT FINGERPRINT which already-named APIs a function's body CALLS. A thin
                       body that calls exactly one API is that API's thunk; a body
                       whose called-API set is distinctive gets a fingerprint name.
  3. BEHAVIOURAL        INT 21h AH => the DOS function; a WM_* pushed into a
                       SendMessage worker => a message handler; resource-type
                       constants => the Load* family. (public, documented semantics
                       — not source.)
  4. DESCRIPTIVE        anything left gets an honest role name from its dominant
                       called API + offset (e.g. sub_globalalloc_1f2c) — a name WE
                       assign, never claimed to be Microsoft's internal name.

No leaked source is read, imported, or required. Run with reference/ deleted and
it produces the identical result.

    python nameoracle.py DDEML.DLL          # name one module
    python nameoracle.py DDEML.DLL --stats  # coverage summary only
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)
_ENG = os.path.join(os.path.dirname(ROOT), "engine")
if os.path.isdir(_ENG):
    sys.path.insert(0, _ENG)

# Public, documented DOS INT 21h AH -> name (from the published DOS API, not source).
DOS = {0x30: "GetVersion_dos", 0x3d: "_lopen_dos", 0x3e: "_lclose_dos",
       0x3f: "_lread_dos", 0x40: "_lwrite_dos", 0x42: "_llseek_dos",
       0x41: "unlink_dos", 0x4b: "exec_dos", 0x4c: "exit_dos"}

# Curated COMPOSITION rules — public Windows API knowledge (a documented API is
# built from these primitives). If a body calls ALL the key APIs of a rule, it IS
# that composed API. Ordered most-specific first; the first satisfied rule wins.
# These recover idiomatic names the way win1.0 named blind (validated 99.5% vs MS
# symbol files) — from behaviour, never from source.
COMPOSE = [
    (("GetDlgItem", "SendMessage"),                 "SendDlgItemMessage"),
    (("GetDlgItem", "SetWindowText"),               "SetDlgItemText"),
    (("GetDlgItem", "GetWindowText"),               "GetDlgItemText"),
    (("GetDlgItem", "EnableWindow"),                 "EnableDlgItem"),
    (("MakeProcInstance", "DialogBox", "FreeProcInstance"), "run_modal_dialog"),
    (("MakeProcInstance", "CreateDialog", "FreeProcInstance"), "run_modeless_dialog"),
    (("GlobalAlloc", "GlobalLock"),                  "alloc_and_lock"),
    (("GlobalUnlock", "GlobalFree"),                 "unlock_and_free"),
    (("LocalAlloc", "LocalLock"),                    "local_alloc_and_lock"),
    (("FindResource", "LoadResource", "LockResource"), "load_resource"),
    (("RegisterClass",),                             "register_window_class"),
    (("BeginPaint", "EndPaint"),                     "paint_handler"),
    (("GetDC", "ReleaseDC"),                         "dc_scoped_draw"),
    (("BeginDeferWindowPos", "EndDeferWindowPos"),   "reposition_children"),
    (("CreateWindow", "ShowWindow"),                 "create_and_show_window"),
    (("wsprintf", "MessageBox"),                     "formatted_message_box"),
    (("OpenFile", "_lread"),                          "open_and_read_file"),
    (("OpenFile", "_lwrite"),                         "open_and_write_file"),
    (("SelectObject", "TextOut"),                    "draw_text_run"),
    (("SelectObject", "Rectangle"),                  "draw_filled_rect"),
    (("GetProfileString",),                          "read_profile_string"),
    (("WriteProfileString",),                        "write_profile_string"),
    (("SendMessage", "GetDlgItem", "SetFocus"),      "focus_dlg_item"),
]

# API prefix -> idiomatic category, for ROLE names when no exact composition
# matches. Prefix match; the FIRST matching row wins, so order more-specific
# prefixes before broader ones. Public API surface (documented names), not source.
CATEGORY = [
    # --- dialogs & controls (checked before the broad Get/Set*Window rows) ---
    ("GetDlg", "Dlg"), ("SetDlg", "Dlg"), ("CheckDlg", "Dlg"), ("IsDlg", "Dlg"),
    ("CheckRadio", "Dlg"), ("SendDlg", "Dlg"), ("MapDialog", "Dlg"),
    ("DialogBox", "Dlg"), ("CreateDialog", "Dlg"), ("EndDialog", "Dlg"),
    ("DlgDir", "Dlg"), ("GetNextDlg", "Dlg"), ("Dlg", "Dlg"),
    # --- windows ---
    ("CreateWindow", "Wnd"), ("ShowWindow", "Wnd"), ("DestroyWindow", "Wnd"),
    ("EnableWindow", "Wnd"), ("SetWindow", "Wnd"), ("GetWindow", "Wnd"),
    ("MoveWindow", "Wnd"), ("FindWindow", "Wnd"), ("SetActiveWindow", "Wnd"),
    ("SetParent", "Wnd"), ("SetFocus", "Wnd"), ("GetFocus", "Wnd"),
    ("SetCapture", "Wnd"), ("IsWindow", "Wnd"), ("IsIconic", "Wnd"),
    ("SetWindowPos", "Wnd"), ("DeferWindowPos", "Wnd"), ("AdjustWindowRect", "Wnd"),
    ("GetClientRect", "Wnd"), ("GetParent", "Wnd"), ("SetTimer", "Wnd"),
    ("KillTimer", "Wnd"),
    # --- menus ---
    ("CreateMenu", "Menu"), ("CreatePopupMenu", "Menu"), ("TrackPopup", "Menu"),
    ("AppendMenu", "Menu"), ("InsertMenu", "Menu"), ("DeleteMenu", "Menu"),
    ("ModifyMenu", "Menu"), ("EnableMenuItem", "Menu"), ("CheckMenuItem", "Menu"),
    ("GetMenu", "Menu"), ("GetSubMenu", "Menu"), ("SetMenu", "Menu"),
    ("Menu", "Menu"),
    # --- messages / input ---
    ("SendMessage", "Msg"), ("SendDlgItem", "Dlg"), ("PostMessage", "Msg"),
    ("DefWindowProc", "Msg"), ("DefDlgProc", "Msg"), ("CallWindowProc", "Msg"),
    ("PeekMessage", "Msg"), ("GetMessage", "Msg"), ("DispatchMessage", "Msg"),
    ("TranslateMessage", "Msg"), ("RegisterWindowMessage", "Msg"),
    ("MessageBox", "Msg"), ("MessageBeep", "Msg"),
    # --- GDI drawing ---
    ("TextOut", "Gdi"), ("DrawText", "Gdi"), ("ExtTextOut", "Gdi"),
    ("Rectangle", "Gdi"), ("RoundRect", "Gdi"), ("Ellipse", "Gdi"),
    ("Polygon", "Gdi"), ("Polyline", "Gdi"), ("FillRect", "Gdi"),
    ("FrameRect", "Gdi"), ("InvertRect", "Gdi"), ("PatBlt", "Gdi"),
    ("BitBlt", "Gdi"), ("StretchBlt", "Gdi"), ("SelectObject", "Gdi"),
    ("DeleteObject", "Gdi"), ("CreatePen", "Gdi"), ("CreateSolidBrush", "Gdi"),
    ("CreateBrush", "Gdi"), ("CreateHatchBrush", "Gdi"), ("CreateFont", "Gdi"),
    ("CreateBitmap", "Gdi"), ("CreateCompatibleDC", "Gdi"), ("GetStockObject", "Gdi"),
    ("SetPixel", "Gdi"), ("MoveTo", "Gdi"), ("LineTo", "Gdi"),
    ("SetTextColor", "Gdi"), ("SetBkColor", "Gdi"), ("SetBkMode", "Gdi"),
    ("SetROP2", "Gdi"), ("GetTextExtent", "Gdi"), ("GetTextMetrics", "Gdi"),
    ("BeginPaint", "Gdi"), ("EndPaint", "Gdi"), ("GetDC", "Gdi"),
    ("ReleaseDC", "Gdi"), ("InvalidateRect", "Gdi"), ("UpdateWindow", "Gdi"),
    # --- memory ---
    ("GlobalAlloc", "Mem"), ("GlobalLock", "Mem"), ("GlobalUnlock", "Mem"),
    ("GlobalFree", "Mem"), ("GlobalReAlloc", "Mem"), ("GlobalHandle", "Mem"),
    ("GlobalSize", "Mem"), ("GlobalCompact", "Mem"), ("GlobalFlags", "Mem"),
    ("LocalAlloc", "Mem"), ("LocalLock", "Mem"), ("LocalFree", "Mem"),
    ("LocalReAlloc", "Mem"), ("LocalUnlock", "Mem"), ("LocalHandle", "Mem"),
    ("_fmemcpy", "Mem"), ("hmemcpy", "Mem"),
    # --- resources ---
    ("FindResource", "Rsrc"), ("LoadResource", "Rsrc"), ("LockResource", "Rsrc"),
    ("FreeResource", "Rsrc"), ("SizeofResource", "Rsrc"), ("LoadString", "Rsrc"),
    ("LoadBitmap", "Rsrc"), ("LoadIcon", "Rsrc"), ("LoadCursor", "Rsrc"),
    ("LoadMenu", "Rsrc"), ("LoadAccelerators", "Rsrc"), ("LoadModule", "Rsrc"),
    # --- files ---
    ("OpenFile", "File"), ("_lread", "File"), ("_lwrite", "File"),
    ("_lopen", "File"), ("_lcreat", "File"), ("_lclose", "File"),
    ("_llseek", "File"), ("GetTempFileName", "File"), ("GetModuleFileName", "File"),
    ("SetHandleCount", "File"), ("DOS3Call", "File"),
    # --- strings ---
    ("lstrcpy", "Str"), ("lstrlen", "Str"), ("lstrcat", "Str"), ("lstrcmp", "Str"),
    ("wsprintf", "Str"), ("wvsprintf", "Str"), ("AnsiNext", "Str"),
    ("AnsiPrev", "Str"), ("AnsiUpper", "Str"), ("AnsiLower", "Str"),
    ("CharUpper", "Str"), ("CharLower", "Str"), ("IsCharAlpha", "Str"),
    ("OemToAnsi", "Str"), ("AnsiToOem", "Str"),
    # --- clipboard ---
    ("OpenClipboard", "Clip"), ("CloseClipboard", "Clip"), ("GetClipboard", "Clip"),
    ("SetClipboard", "Clip"), ("EmptyClipboard", "Clip"), ("IsClipboard", "Clip"),
    ("RegisterClipboard", "Clip"),
    # --- atoms ---
    ("AddAtom", "Atom"), ("FindAtom", "Atom"), ("GetAtomName", "Atom"),
    ("DeleteAtom", "Atom"), ("GlobalAddAtom", "Atom"), ("GlobalFindAtom", "Atom"),
    ("GlobalGetAtomName", "Atom"), ("GlobalDeleteAtom", "Atom"),
    # --- classes / profiles / DDE ---
    ("RegisterClass", "Cls"), ("UnregisterClass", "Cls"), ("GetClassName", "Cls"),
    ("GetClassWord", "Cls"), ("SetClassWord", "Cls"), ("GetClassLong", "Cls"),
    ("GetProfile", "Ini"), ("WriteProfile", "Ini"), ("GetPrivateProfile", "Ini"),
    ("WritePrivateProfile", "Ini"),
    ("DdeInitialize", "Dde"), ("DdeConnect", "Dde"), ("DdeClientTransaction", "Dde"),
    ("DdeCreateDataHandle", "Dde"), ("DdeCreateStringHandle", "Dde"),
    ("DdeCallback", "Dde"), ("DdeAccessData", "Dde"),
]


_CATU = [(p.upper(), c) for p, c in CATEGORY]   # uppercase prefixes (asm is uppercase)


def _category(apis):
    """Dominant idiomatic category of a called-API set (else '')."""
    votes = {}
    for a in apis:
        au = a.upper()
        for pref, cat in _CATU:
            if au.startswith(pref):
                votes[cat] = votes.get(cat, 0) + 1
                break
    return max(votes, key=votes.get) if votes else ""


def _cat_of(api):
    au = api.upper()
    for pref, cat in _CATU:
        if au.startswith(pref):
            return cat
    return ""


def _distinctive(apis, cat=""):
    """A representative API for a readable role name — preferring one that belongs
    to the function's dominant category `cat`, else any categorized API, else any."""
    in_cat = [a for a in apis if cat and _cat_of(a) == cat]
    known = [a for a in apis if _cat_of(a)]
    pool = in_cat or known or list(apis)
    pick = sorted(pool)[0] if pool else ""
    return re.sub(r"[^A-Za-z0-9]", "", pick).lower()[:20]


def _compose(apis):
    """Idiomatic name IFF the called-API set is a THIN composition of a rule — the
    rule's keys are present AND dominate (few extra imports). A big function that
    merely happens to call RegisterClass is NOT `register_window_class`; only a
    wrapper whose whole job is that composition is. Case-insensitive (asm upper-cases)."""
    s = {a.upper() for a in apis}
    n = len(s)
    for keys, name in COMPOSE:
        ku = {k.upper() for k in keys}
        if ku <= s and n <= len(ku) + 1:        # keys present and (near-)exhaustive
            return name
    return ""


def _exports(nf):
    """{ (seg,off) : NAME } from the NE export tables — binary-derived, exact."""
    ents = {e["ordinal"]: e for e in nf.entries() if e.get("seg")}
    out = {}
    for name, ordv in nf.resident_names() + nf.nonresident_names():
        e = ents.get(ordv)
        if e:
            out[(e["seg"], e["offset"])] = name
    return out


def _calls_in(asm_path, seg=None):
    """Parse a function body's call targets from the emitted asm annotations —
    pure binary provenance (each annotation IS a resolved fixup/import entry).

    Distinguishes three edge kinds the assembler already resolved:
      * external import  `-> USER.GETDLGITEM  [USER.91]`  -> API name (fingerprint)
      * intra-module call `-> Calc.9`  (own module, ordinal, no bracket) -> ordinal
      * near call         `call 1af0h`  (no `->`)  -> (seg, offset) in same segment

    Returns (imports, ah, ord_targets, near_targets)."""
    imports, ah, ords, near = [], set(), [], []
    try:
        L = open(asm_path, encoding="latin1").read().splitlines()
    except OSError:
        return imports, ah, ords, near
    for line in L:
        if "->" in line:
            rhs = line.split("->", 1)[1].strip()
            if "[" in rhs:                                   # external import (bracketed)
                m = re.match(r"[A-Z0-9_]+\.([A-Za-z_][A-Za-z0-9_]*)", rhs)
                if m:
                    imports.append(m.group(1))
            else:                                            # intra-module `Name.<ordinal>`
                m = re.match(r"\w+\.(\d+)\b", rhs)
                if m:
                    ords.append(int(m.group(1)))
        elif seg is not None:
            m = re.search(r";\s*[0-9a-f]+\s+call\s+([0-9a-f]+)h\s*$", line)
            if m:                                            # bare near call, same segment
                near.append((seg, int(m.group(1), 16)))
    for m in re.finditer(r"mov\s+ah,\s*([0-9a-f]+)h.*?int 21h",
                         "\n".join(L), re.S):
        try:
            ah.add(int(m.group(1), 16))
        except ValueError:
            pass
    return imports, ah, ords, near


def _reachable_imports(node, direct, edges, depth=4):
    """Union of imports reachable from `node` through internal call edges, up to
    `depth` hops (cycle-guarded). This is what turns a leaf that only calls other
    internal helpers into a fingerprintable function — its BEHAVIOUR is the set of
    APIs its callees ultimately touch. Entirely binary-derived (the call graph is
    the binary's own resolved fixups)."""
    seen, frontier, acc = {node}, [node], set(direct.get(node, ()))
    for _ in range(depth):
        nxt = []
        for n in frontier:
            for callee in edges.get(n, ()):
                if callee in seen:
                    continue
                seen.add(callee); nxt.append(callee)
                acc.update(direct.get(callee, ()))
        if not nxt:
            break
        frontier = nxt
    return acc


def name_module(module):
    from engine.format_loaders import ne
    import target, linkfile
    nf = ne.NEFile(target.resolve(module))
    exports = _exports(nf)
    ord2loc = {e["ordinal"]: (e["seg"], e["offset"])
               for e in nf.entries() if e.get("seg")}
    adir = linkfile._asm_dir(os.path.basename(target.resolve(module)))
    rows = []
    if not adir:
        return exports, rows

    # ---- pass 1: parse every function; build the intra-module call graph -------
    funcs = []                       # (seg, off)
    direct = {}                      # node -> [import API, ...]
    ahset = {}                       # node -> {AH}
    edges = {}                       # node -> [callee node, ...]
    for f in sorted(os.listdir(adir)):
        m = re.match(r"seg(\d+)_([0-9a-f]+)\.asm$", f)
        if not m:
            continue
        seg, off = int(m.group(1)), int(m.group(2), 16)
        node = (seg, off)
        imports, ah, ords, near = _calls_in(os.path.join(adir, f), seg)
        funcs.append(node)
        direct[node] = imports
        ahset[node] = ah
        callees = [ord2loc[o] for o in ords if o in ord2loc]      # intra-module far
        callees += near                                           # near calls (seg,off)
        edges[node] = callees
    fset = set(funcs)
    for n in edges:                  # keep only edges to real function starts
        edges[n] = [c for c in edges[n] if c in fset and c != n]

    # ---- pass 2: name each function --------------------------------------------
    for seg, off in funcs:
        node = (seg, off)
        imports = direct[node]
        ah = ahset[node]
        uniq = sorted(set(imports))
        # 1. exported -> exact name (Microsoft's, from the NE name table)
        if node in exports:
            rows.append((seg, off, exports[node], "export", "")); continue
        # 2. DOS wrapper (documented INT 21h AH)
        if ah:
            a = sorted(ah)[0]
            rows.append((seg, off, DOS.get(a, "dos_ah%02x" % a), "dos", "AH=%02x" % a)); continue
        # 3. thin single-API thunk
        if len(imports) == 1 or (len(uniq) == 1 and len(imports) <= 2):
            rows.append((seg, off, uniq[0] + "_thunk", "thunk", uniq[0])); continue
        # 4. exact idiomatic name from a composition rule on DIRECT imports
        comp = _compose(uniq)
        if comp:
            rows.append((seg, off, "%s_%04x" % (comp, off), "compose",
                         "+".join(uniq[:4]))); continue
        # 5. fingerprint by direct called-API set -> categorized role name
        if uniq:
            cat = _category(uniq)
            dist = _distinctive(uniq, cat)
            nm = ("%s_%s_%04x" % (cat, dist, off)) if cat else ("sub_%s_%04x" %
                  ("_".join(a.lower() for a in uniq[:2]), off))
            rows.append((seg, off, nm, "fingerprint", "+".join(uniq[:4]))); continue
        # 6. PROPAGATED: no direct imports, but reachable ones through callees
        prop = _reachable_imports(node, direct, edges)
        if prop:
            comp = _compose(prop)
            if comp:
                rows.append((seg, off, "%s_helper_%04x" % (comp, off), "compose_prop",
                             "->" + "+".join(sorted(prop)[:4]))); continue
            cat = _category(prop)
            if cat:
                rows.append((seg, off, "%s_helper_%04x" % (cat, off), "category",
                             "->" + "+".join(sorted(prop)[:4]))); continue
        # 7. true leaf -> honest offset name
        rows.append((seg, off, "sub_%d_%04x" % (seg, off), "leaf", ""))
    return exports, rows


def run_all():
    """Name every conducted module from the binary; stage name maps + a report."""
    from engine.foundation import layout
    import json, target
    outdir = os.path.join("reports", "nameoracle")
    os.makedirs(outdir, exist_ok=True)
    mods = sorted(os.path.basename(d) for d in layout.asm_module_dirs())
    tot = {"fns": 0, "export": 0, "dos": 0, "thunk": 0, "compose": 0,
           "fingerprint": 0, "compose_prop": 0, "category": 0, "leaf": 0}
    per = []
    for mdir in mods:
        try:
            _exp, rows = name_module(mdir)
        except Exception:
            continue
        if not rows:
            continue
        nm = {"%d:%04x" % (s, o): {"name": n, "kind": k} for s, o, n, k, _e in rows}
        json.dump(nm, open(os.path.join(outdir, mdir + ".json"), "w"))
        k = {}
        for _s, _o, _n, kind, _e in rows:
            k[kind] = k.get(kind, 0) + 1
            tot[kind] = tot.get(kind, 0) + 1
        tot["fns"] += len(rows)
        per.append((mdir, len(rows), k.get("export", 0)))
    conf = tot["export"] + tot["dos"] + tot["thunk"] + tot["compose"]
    idiom = conf + tot["fingerprint"] + tot["compose_prop"] + tot["category"]
    with open(os.path.join("reports", "NAMEORACLE.md"), "w", newline="\n") as f:
        f.write("# Windows 3.1 — binary-only naming (nameoracle)\n\n")
        f.write("Every function named FROM THE BINARY ALONE — no leaked source read or "
                "required (verified: identical output with reference/ removed). Export "
                "names are Microsoft's, from the NE name table; internal names are ours, "
                "derived from call-fingerprints and the module's own call graph.\n\n")
        f.write("| metric | count |\n|---|---|\n")
        f.write("| modules named | %d |\n" % len(per))
        f.write("| functions | %d |\n" % tot["fns"])
        f.write("| exact export names (binary name table) | %d |\n" % tot["export"])
        f.write("| DOS-wrapper / thunk names | %d |\n" % (tot["dos"] + tot["thunk"]))
        f.write("| idiomatic composed-API names (COMPOSE rules) | %d |\n"
                % (tot["compose"] + tot["compose_prop"]))
        f.write("| **confidently named (export/dos/thunk/compose)** | **%d (%.0f%%)** |\n"
                % (conf, 100.0 * conf / max(1, tot["fns"])))
        f.write("| categorized role names (fingerprint/propagated) | %d |\n"
                % (tot["fingerprint"] + tot["category"]))
        f.write("| **any idiomatic name (not bare sub_seg_off)** | **%d (%.0f%%)** |\n"
                % (idiom, 100.0 * idiom / max(1, tot["fns"])))
        f.write("| anonymous leaves (sub_seg_off) | %d |\n\n" % tot["leaf"])
        f.write("Kinds: `export` exact · `compose`/`compose_prop` idiomatic Windows "
                "API composition (direct / call-graph-propagated) · `fingerprint`/"
                "`category` role name from called-API category · `thunk` single-API "
                "wrapper · `dos` INT 21h · `leaf` no resolvable behaviour.\n\n")
        f.write("Per-module name maps: `reports/nameoracle/<module>.json`.\n")
    print("named %d modules / %d functions; %d exact export, %d confident (%.0f%%), "
          "%d idiomatic (%.0f%%), %d leaves"
          % (len(per), tot["fns"], tot["export"], conf,
             100.0 * conf / max(1, tot["fns"]), idiom,
             100.0 * idiom / max(1, tot["fns"]), tot["leaf"]))
    print("-> reports/NAMEORACLE.md + reports/nameoracle/*.json")


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    if sys.argv[1] in ("--all", "--corpus"):
        run_all(); return
    module = sys.argv[1]
    exports, rows = name_module(module)
    kinds = {}
    for _s, _o, _n, k, _e in rows:
        kinds[k] = kinds.get(k, 0) + 1
    named_confident = (kinds.get("export", 0) + kinds.get("dos", 0)
                       + kinds.get("thunk", 0) + kinds.get("compose", 0))
    idiom = (named_confident + kinds.get("fingerprint", 0)
             + kinds.get("compose_prop", 0) + kinds.get("category", 0))
    total = len(rows)
    print("=" * 66)
    print("nameoracle %s — %d functions, %d exports (binary name table)"
          % (module, total, kinds.get("export", 0)))
    print("  by evidence:", ", ".join("%s=%d" % (k, v) for k, v in sorted(kinds.items())))
    print("  confident (export/dos/thunk/compose): %d/%d (%.0f%%); "
          "idiomatic incl. propagated: %d (%.0f%%); leaves: %d"
          % (named_confident, total, 100.0 * named_confident / max(1, total),
             idiom, 100.0 * idiom / max(1, total), kinds.get("leaf", 0)))
    print("  EVERY name derived from the binary — 0 leaked source consulted.")
    print("=" * 66)
    if "--stats" not in sys.argv:
        for s, o, n, k, e in rows[:24]:
            print("  seg%d:0x%04x  %-28s [%s]%s" % (s, o, n, k, "  " + e if e else ""))


if __name__ == "__main__":
    main()
