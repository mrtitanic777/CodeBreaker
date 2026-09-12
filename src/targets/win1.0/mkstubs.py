#!/usr/bin/env python3
"""
mkstubs.py — turn a linker's unresolved-external list into a module of
LOUDLY MARKED stubs, so a partially reconstructed app can be linked and loaded
while the missing work stays visible and counted.

The list is not guessed at: it is what LINK4 said it could not resolve. Every
name in it is a function the reconstruction CALLS and has not TRANSCRIBED — the
honest inventory of what is left to do for that app.

A stub does nothing and returns 0. That is a real behavioural hole, not a
detail, so it is recorded three ways: a banner in the generated module, one
comment per stub naming the seg:offset where the real function lives when the
name encodes it, and a count printed by the caller.

The declarations are emitted FAR and appended to the shared header, because the
stub module is a separate OBJ in a separate segment: a near call cannot reach
it. That is also why every module must be RECOMPILED after stubs are generated
— the call sites themselves change shape.

Not a standalone step in normal use; link16.py drives it.
"""
import os
import re

# `_Name` is the C convention (leading underscore); `NAME` with no underscore
# is a PASCAL-convention symbol, which for our purposes means a Windows entry
# point the runtime expects us to supply — WinMain.
CALLSEG = re.compile(r"^Call_seg(\d+)_([0-9a-fA-F]+)$")
SUBOFF = re.compile(r"^Sub_0x([0-9a-fA-F]+)$")


def parse_unresolved(link_txt):
    """-> [symbol, ...] exactly as the linker spelled them.

    LINK4 prints the unresolved list on STDERR, not stdout, so both streams are
    read: link.txt for the run that leaves stderr on the console, link.err for
    the one that captures it (which it must — with stderr unredirected LINK4's
    prompts block the DOS session and nothing gets built at all)."""
    txt = ""
    for p in (link_txt, os.path.splitext(link_txt)[0] + ".err",
              os.path.splitext(link_txt)[0] + ".ERR"):
        if os.path.exists(p):
            txt += open(p, encoding="cp437", errors="replace").read()
    if not txt:
        return []
    tail = txt.split("Unresolved externals:", 1)
    if len(tail) < 2:
        return []
    out = []
    for m in re.finditer(r"^\s*([A-Za-z_@][\w@]*)\s+in file\(s\)\s*:",
                         tail[1], re.M):
        out.append(m.group(1))
    return out


def describe(name):
    """What the reconstruction knows about the missing function, from its name."""
    m = CALLSEG.match(name)
    if m:
        return "the function at seg%s:0x%s — known by address only" % (
            m.group(1), m.group(2).lower())
    m = SUBOFF.match(name)
    if m:
        return "the function at offset 0x%s — known by address only" % m.group(1).lower()
    return "declared and called by the reconstruction, never transcribed"


def generate(outdir, stem, symbols):
    """Write <STEM>X.C and append FAR declarations to <STEM>.H.
    Returns (n_stubs, needs_winmain)."""
    U = stem.upper()
    # The app's own prototypes, from the shared header. A PASCAL-named unresolved
    # symbol that the APP declares (ScrollToPage, FormatDate — the cross-segment
    # helpers in segments we haven't transcribed) is a missing FUNCTION, and must
    # be stubbed like any other. A PASCAL name the app does NOT declare is a real
    # API binding (a wrong ordinal) and is left alone — stubbing it would hide
    # the bug. The header is what tells the two apart.
    hpath0 = os.path.join(outdir, U + ".H")
    htext0 = open(hpath0, encoding="utf-8", errors="replace").read() \
        if os.path.exists(hpath0) else ""
    app_decl = set(re.findall(
        r"(?m)^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\(", htext0))

    # data globals declared but not allocated: `extern TYPE NAME[];` with empty
    # brackets is a tentative array that emits NOTHING, so the symbol comes up
    # unresolved. It is DATA, not a function — stubbing it as `long FAR NAME()`
    # both mis-types it and clashes with the extern. Collect these names so they
    # get a data definition instead (and a flag: the extent is unknown).
    data_globals = set(re.findall(
        r"(?m)^\s*extern\s+[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\[\s*\]\s*;",
        htext0))

    cnames, pnames, dnames, winmain = [], [], [], False
    for s in symbols:
        base = s[1:] if s.startswith("_") else s
        if s.upper() == "WINMAIN":
            winmain = True
        elif base in data_globals:
            dnames.append(base)                      # unallocated data global
        elif s.startswith("_"):
            cnames.append(base)
        else:
            # PASCAL-convention (uppercase, no underscore). objdump/LINK4 upcase
            # PASCAL names, so match the app's declaration case-insensitively.
            hit = next((d for d in app_decl if d.upper() == s.upper()), None)
            if hit:
                pnames.append(hit)
            # else: a genuine unresolved API import — left alone on purpose.
    cnames = sorted(set(cnames))
    pnames = sorted(set(pnames))
    dnames = sorted(set(dnames))

    path = os.path.join(outdir, U + "X.C")
    with open(path, "w", encoding="utf-8", newline="") as f:
        # The stubs are trivial (return 0) and reference no windows.h symbols,
        # so trim the big unused sections to keep this single unit under MS C
        # 3.0 P1's symbol table (segsplit trims the app units the same way).
        f.write('#define NODRAWTEXT\n#define NOMETAFILE\n#define NOWH\n'
                '#define NONCMESSAGES\n#define NOSYSMETRICS\n#define NOGDICAPMASKS\n'
                '#define NOKANJI\n#define NOSOUND\n#define NOATOM\n')
        f.write('#include <windows.h>\n#include "%s.h"\n\n' % U)
        f.write("/* " + "=" * 70 + "\n")
        f.write(" * %sX.C — NOT RECONSTRUCTED. %d function(s).\n" %
                (U, len(cnames) + (1 if winmain else 0)))
        f.write(" *\n"
                " * Every function below EXISTS IN THE BINARY and has not been\n"
                " * transcribed. Each one here is an empty body returning 0, present\n"
                " * only so the module set links and the app can be loaded and looked\n"
                " * at. Anything that depends on them is ABSENT from the built EXE —\n"
                " * this file is the measure of how partial that EXE is.\n"
                " *\n"
                " * Generated by mkstubs.py from the linker's own unresolved-external\n"
                " * list. Delete a stub as soon as its function is transcribed.\n")
        f.write(" * " + "=" * 70 + " */\n\n")
        if winmain:
            f.write("/* WinMain: the C runtime's startup (SLIBW winstart) calls it, so a\n"
                    " * Windows program cannot link without one. The real one is in the\n"
                    " * binary and is NOT transcribed; this returns immediately, which\n"
                    " * means the built EXE registers no class and shows no window. */\n"
                    "int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)\n"
                    "    HANDLE hInstance;\n"
                    "    HANDLE hPrevInstance;\n"
                    "    LPSTR  lpCmdLine;\n"
                    "    int    nCmdShow;\n"
                    "{\n"
                    "    return 0;                    /* NOT RECONSTRUCTED */\n"
                    "}\n\n")
        pass          # the stub bodies are written below, once their declared
                      # return types are known from the shared header

    # The reconstruction usually ALREADY declares these (its own forward
    # declarations, which segsplit moves into the shared header). Re-declaring
    # them is a redefinition error, and inventing a return type would clash
    # with the existing one — so adopt what is declared and only make it FAR.
    hpath = os.path.join(outdir, U + ".H")
    h = open(hpath, encoding="utf-8", errors="replace").read() \
        if os.path.exists(hpath) else ""
    quals = {}
    for n in cnames:
        m = re.search(r"(?m)^([A-Za-z_][\w \t\*]*?)\b" + re.escape(n) +
                      r"\s*\([^;{]*\)\s*;", h)
        if m:
            q = " ".join(m.group(1).split())
            # FAR only counts as making the FUNCTION far if it comes AFTER the
            # last pointer star. `char FAR *StrBrk` is a NEAR function returning
            # a far pointer — the stub lands in _TEXT and a call to it from
            # another segment then emitted a near "Intersegment self-relative
            # fixup". Insert FAR right before the name (after the last *) so it
            # is the function that is far.
            tail = q.rsplit("*", 1)[-1]
            if re.search(r"\bnear\b", q, re.I):
                pass                              # honor a genuine near helper
            elif not re.search(r"\b(FAR|far|PASCAL|pascal)\b", tail):
                q = q + " FAR"
                newline = re.sub(r"(\*?\s*)" + re.escape(n) + r"\b",
                                 r"\1FAR " + n, m.group(0), count=1)
                h = h[:m.start()] + newline + h[m.end():]
            quals[n] = q
        else:
            quals[n] = "long FAR"
    missing = [n for n in cnames if not re.search(
        r"(?m)^[A-Za-z_][\w \t\*]*?\b" + re.escape(n) + r"\s*\([^;{]*\)\s*;", h)]
    if missing:
        decls = ["", "/* ---- not-yet-transcribed functions, defined as stubs in "
                 "%sX.C ----" % U,
                 " * FAR because the stub module is its own segment: a near call",
                 " * cannot reach it. */"]
        decls += ["%s %s();" % (quals[n], n) for n in missing]
        marker = "\n#endif\n"
        h = (h.replace(marker, "\n".join(decls) + marker, 1)
             if marker in h else h + "\n".join(decls) + "\n")
    if hpath:
        open(hpath, "w", encoding="utf-8", newline="").write(h)

    with open(path, "a", encoding="utf-8", newline="") as f:
        for n in cnames:
            q = quals[n]
            body = "{ }" if re.match(r"^(void|VOID)\b", q) else "{ return 0; }"
            f.write("/* %s */\n%s %s() %s   /* NOT RECONSTRUCTED */\n\n"
                    % (describe(n), q, n, body))
        # PASCAL-convention stubs (the app's own cross-segment helpers in
        # segments not yet transcribed). These keep their DECLARED parameter
        # list, in K&R form: a PASCAL callee pops its arguments (`retf N`), so a
        # stub with the wrong arg count would unbalance the caller's stack. The
        # signature comes from the app's own prototype in the header.
        for n in pnames:
            ret, params = _pascal_sig(htext0, n)
            names = ["a%d" % (i + 1) for i in range(len(params))]
            hdr = "%s %s(%s)" % (ret, n, ", ".join(names))
            decl = "".join("    %s %s;\n" % (t, nm)
                           for t, nm in zip(params, names))
            body = "{ }" if re.match(r"^(void|VOID)\b", ret) else "{ return 0; }"
            f.write("/* %s (FAR PASCAL, %d arg%s) */\n%s\n%s%s   /* NOT RECONSTRUCTED */\n\n"
                    % (describe(n), len(params), "" if len(params) == 1 else "s",
                       hdr, decl, body))
        # Unallocated data globals: give each a minimal 1-element definition so
        # the link resolves, and flag it — the real EXTENT is not recovered, so
        # anything that indexes past [0] reads/writes out of bounds. This is the
        # honest placeholder, the data analogue of a function stub.
        for n in dnames:
            typ = _data_type(htext0, n)
            f.write("/* %s — DATA global, EXTENT NOT RECOVERED (placeholder [1]) */\n"
                    "%s %s[1];   /* NOT RECONSTRUCTED — size unknown */\n\n"
                    % (n, typ, n))
    return (len(cnames) + len(pnames) + len(dnames) + (1 if winmain else 0),
            winmain)


def _data_type(header, name):
    """Base type of an `extern TYPE NAME[];` in the header (default char)."""
    m = re.search(r"(?m)^\s*extern\s+([A-Za-z_][\w \t\*]*?)\b"
                  + re.escape(name) + r"\s*\[\s*\]\s*;", header)
    return " ".join(m.group(1).split()) if m else "char"


def _pascal_sig(header, name):
    """(return-type-with-FAR-PASCAL, [param types]) from the app's prototype."""
    m = re.search(r"(?m)^([A-Za-z_][\w \t\*]*?\bFAR\s+PASCAL)\s+"
                  + re.escape(name) + r"\s*\(([^;)]*)\)\s*;", header, re.I)
    if not m:
        return ("WORD FAR PASCAL", [])
    ret = " ".join(m.group(1).split())
    inside = m.group(2).strip()
    if not inside or inside.lower() == "void":
        return (ret, [])
    params = [" ".join(p.split()) for p in inside.split(",") if p.strip()]
    return (ret, params)
