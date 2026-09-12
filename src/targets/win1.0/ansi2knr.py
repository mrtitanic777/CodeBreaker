#!/usr/bin/env python3
"""
ansi2knr.py — rewrite ANSI-C function syntax as K&R, for Microsoft C 3.0.

MS C 3.0 (1985) predates ANSI C. It rejects a prototype that names its
parameters — `static void InitClock(HWND hWnd);` is a syntax error to it — and
it rejects an ANSI function definition. Our reconstruction is written ANSI
style; the period source is K&R. This converts one to the other.

    DEFINITION
      long FAR PASCAL WndProc(HWND hWnd, unsigned msg, WORD wParam, LONG lParam)
      {
    becomes
      long FAR PASCAL WndProc(hWnd, msg, wParam, lParam)
      HWND hWnd;
      unsigned msg;
      WORD wParam;
      LONG lParam;
      {

    DECLARATION (forward prototype)
      static void InitClock(HWND hWnd);
    becomes
      static void InitClock();

    (void)  ->  ()   (C 3.0 reads f(void) as a "type list", not a formal list)

COMMENT-AWARE. Our reconstruction annotates headers heavily:
      static long DoMenuCommand(HWND hWnd, WORD id)   /* seg3:0x298c */
      {
and even puts seg-ref comments *inside* a multi-line parameter list. Parsing is
done on a comment-blanked copy of the source (comments replaced by spaces, so
every byte offset still lines up); the rewrite is spliced back into the original
text, and any comment trailing the `)` is carried onto the new header line.

WHAT IT DOES NOT TOUCH
    Calls (`f(a, b)` — fine in K&R), and any header whose parameter list it
    cannot cleanly parse (a function-pointer parameter, an unusual declarator)
    is LEFT ALONE and reported, so nothing is silently corrupted.

Usage:
    python ansi2knr.py FILE.c            # rewrite in place
    python ansi2knr.py FILE.c --dry      # show what would change
"""
import re, sys

# A type-ish token in the return run — used to tell a real declaration from a
# bare `foo(a, b);` call statement.
TYPE_RE = re.compile(r"\b(static|extern|void|int|long|char|short|unsigned|"
                     r"WORD|BOOL|HWND|HDC|HANDLE|LONG|DWORD|BYTE|HMENU|HPEN|"
                     r"HBRUSH|LPSTR|LPVOID|FARPROC|HICON|HCURSOR|HBITMAP|HRGN|"
                     r"HFONT|ATOM|LPMSG|LPRECT|struct|union)\b")

_COMMENT = re.compile(r"/\*.*?\*/", re.S)


def blank_comments(s):
    """Replace every /*...*/ with same-length spaces, keeping newlines, so
    offsets are preserved 1:1 between the returned text and the original."""
    def repl(m):
        return "".join("\n" if c == "\n" else " " for c in m.group(0))
    return _COMMENT.sub(repl, s)


# A function header on the comment-blanked text: indent, return-type run, name,
# a `(...)` list, optional space/one-newline, then `{` (definition) or `;`.
HDR = re.compile(
    r"(?P<indent>^[ \t]*)"
    r"(?P<ret>(?:static|extern)?[ \t]*[A-Za-z_][\w \t\*]*?)"
    r"\b(?P<name>[A-Za-z_]\w*)[ \t]*"
    r"\((?P<params>[^;{]*?)(?P<rp>\))"
    r"(?P<between>[ \t]*(?:\r?\n[ \t]*)?)"
    r"(?P<tail>[{;])",
    re.M)


def split_params(s):
    """Top-level comma split of a parameter list."""
    out, depth, cur = [], 0, ""
    for c in s:
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur); cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur)
    return [p.strip() for p in out if p.strip()]


def param_name(p):
    """(name, type-text, array-suffix) for one ANSI parameter, or None for
    `void`/empty, or 'UNPARSEABLE' for anything not a plain `type name`."""
    p = " ".join(p.split())          # collapse internal whitespace/newlines
    if p == "void" or p == "":
        return None
    if "(" in p or ("[" in p and "]" not in p):
        return "UNPARSEABLE"
    m = re.match(r"^(.*?\b)(\**)\s*([A-Za-z_]\w*)\s*(\[\s*\d*\s*\])?$", p)
    if not m:
        return "UNPARSEABLE"
    base, stars, name, arr = m.groups()
    typ = base.strip() + (" " + stars if stars else "")
    return (name, typ.strip(), arr or "")


def looks_like_control(ret, name):
    """`if (...) {`, `while (...)`, etc. are not function headers."""
    return name in ("if", "for", "while", "switch", "return", "sizeof",
                    "do", "else") or ret.strip().endswith("return")


def convert(src):
    blanked = blank_comments(src)
    out, last, changed, skipped = [], 0, 0, []
    for m in HDR.finditer(blanked):
        ret, name = m.group("ret"), m.group("name")
        params, tail = m.group("params"), m.group("tail")
        if looks_like_control(ret, name):
            continue
        # a bare `foo(a, b);` call statement — no type in the return run.
        if tail == ";" and not TYPE_RE.search(ret + " "):
            continue
        parts = split_params(params)
        if parts == ["void"]:
            parts = []
        elif parts and all(re.fullmatch(r"[A-Za-z_]\w*", p) for p in parts):
            continue                              # already K&R
        # A DECLARATION needs no parameter names at all — its K&R form is an
        # empty list — so a parameter this converter cannot take apart (an
        # unnamed `PQ*`, a function-pointer type) is no obstacle here. Only a
        # DEFINITION, which has to re-declare each parameter below the header,
        # is left alone in that case.
        # Guard: every part must READ as a type, or this is not a prototype at
        # all but a call statement whose arguments happen not to be bare names
        # (`x = (WORD)f(a+1, b);`). Emptying those would silently delete
        # arguments — far worse than leaving a header unconverted.
        def _is_type_ish(p):
            p = p.strip()
            return bool(TYPE_RE.search(p) or
                        re.fullmatch(r"[A-Za-z_]\w*\s*\**\s*(?:\[\s*\d*\s*\])?"
                                     r"(?:\s+\w+\s*(?:\[\s*\d*\s*\])?)?", p)
                        and re.match(r"^[A-Z]", p))
        if tail == ";" and all(_is_type_ish(p) for p in parts):
            trail = src[m.end("rp"):m.start("tail")]
            tcoms = re.findall(r"/\*.*?\*/", trail, re.S)
            out.append(src[last:m.start()])
            out.append("%s%s%s();%s" % (m.group("indent"), ret, name,
                                        ("  " + " ".join(tcoms)) if tcoms else ""))
            last = m.end()
            changed += 1
            continue
        names, decls, ok = [], [], True
        for p in parts:
            info = param_name(p)
            if info is None:
                continue
            if info == "UNPARSEABLE":
                ok = False
                break
            nm, typ, arr = info
            names.append(nm)
            decls.append("    %s %s%s;" % (typ, nm, arr))
        if not ok:
            skipped.append("%s (line %d)" % (name, blanked[:m.start()].count("\n") + 1))
            continue
        # carry any comment that trailed the ")" onto the new header line.
        trail = src[m.end("rp"):m.start("tail")]
        tcoms = re.findall(r"/\*.*?\*/", trail, re.S)
        tcom = ("  " + " ".join(tcoms)) if tcoms else ""
        out.append(src[last:m.start()])
        indent = m.group("indent")
        if tail == ";":
            out.append("%s%s%s();%s" % (indent, ret, name, tcom))
        else:
            hdr = "%s%s%s(%s)%s" % (indent, ret, name, ", ".join(names), tcom)
            body = ("\n" + "\n".join(decls)) if decls else ""
            out.append(hdr + body + "\n" + indent + "{")
        last = m.end()
        changed += 1
    out.append(src[last:])
    return "".join(out), changed, skipped


def main():
    path = sys.argv[1]
    src = open(path, encoding="utf-8", errors="replace").read()
    new, n, skipped = convert(src)
    print("%s: %d function headers converted to K&R" % (path, n))
    if skipped:
        print("  LEFT ALONE (unparseable param list — check by hand):")
        for s in skipped:
            print("    " + s)
    if "--dry" not in sys.argv and new != src:
        open(path, "w", encoding="utf-8").write(new)
        print("  written")


if __name__ == "__main__":
    main()
