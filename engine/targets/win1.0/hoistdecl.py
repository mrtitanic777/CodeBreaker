#!/usr/bin/env python3
"""
hoistdecl.py — move block-local declarations to the top of their block, for
Microsoft C 3.0.

MS C 3.0 (pre-ANSI) requires every declaration in a block to appear BEFORE the
first statement of that block. Our reconstruction is written in modern C, which
freely mixes them:

      if (wParam == 1) {
          GetDlgItemText(hDlg, 7, &buf, 0xb);      <- a statement
          int serial = (int)ParseDate(&buf);       <- a declaration, too late
          if (serial == 0) ...

C 3.0 silently drops the late declaration, so `serial` then reads as undefined.
This tool rewrites the block to:

      if (wParam == 1) {
          int serial;                              <- hoisted to the top
          GetDlgItemText(hDlg, 7, &buf, 0xb);
          serial = (int)ParseDate(&buf);           <- initializer left in place
          if (serial == 0) ...

so evaluation order is unchanged. A declaration with no initializer just moves
up. Only clear `TYPE [*] name [= init | dims];` single-declarator declarations
are touched; anything it is unsure about is left alone and counted.

Usage:  python hoistdecl.py FILE.c [--dry]
"""
import re, sys

# storage classes and the type tokens the reconstruction actually uses. A
# statement whose first real token is one of these (after optional storage
# class) is treated as a declaration.
STORAGE = r"(?:static|register|auto|extern|const|volatile)"
BASE = (r"unsigned|signed|int|char|short|long|void|float|double|struct|union|"
        r"enum|WORD|DWORD|BYTE|BOOL|LONG|ATOM|HANDLE|HWND|HDC|HMENU|HPEN|"
        r"HBRUSH|HBITMAP|HRGN|HFONT|HICON|HCURSOR|HWND|LPSTR|PSTR|NPSTR|LPINT|"
        r"FARPROC|RECT|POINT|PAINTSTRUCT|MSG|WNDCLASS|LPMSG|LPRECT|LPPOINT|"
        r"TEXTMETRIC|DLGPROC|LPBITMAP|BITMAP|OFSTRUCT|LPVOID|HFILE|LPWORD|"
        r"CARDHEADER|LPCARDHEADER|PCARDHEADER|PCARD|HBITMAP|LPCATCHBUF|CATCHBUF")
DECL_RE = re.compile(
    r"^(?:%s\s+)*(?:unsigned\s+|signed\s+)?(?:%s)\b[\s\*A-Za-z0-9_,\[\]]*"
    % (STORAGE, BASE))
# a simple single-name declarator with optional initializer / array bounds
SIMPLE = re.compile(
    r"^(?P<pre>(?:%s\s+)*(?:unsigned\s+|signed\s+)?(?:%s)\b[\s\*]*"
    r"(?:FAR\s+|NEAR\s+)?\**\s*)(?P<name>[A-Za-z_]\w*)"
    r"(?P<arr>(?:\s*\[[^\]]*\])*)\s*(?P<init>=\s*.*)?;\s*$"
    % (STORAGE, BASE), re.S)

WS = " \t\r\n"


def _skip_ws_cmt(s, i):
    n = len(s)
    while i < n:
        if s[i] in WS:
            i += 1
        elif s[i] == '/' and i + 1 < n and s[i+1] == '*':
            j = s.find('*/', i + 2); i = (j + 2) if j >= 0 else n
        elif s[i] == '/' and i + 1 < n and s[i+1] == '/':
            j = s.find('\n', i); i = j if j >= 0 else n
        else:
            break
    return i


def _read_statement(s, i):
    """From i (start of a statement at block level), return (end, kind) where
    kind in {'decl','stmt','block','label','control'}. end is just past the
    statement. Comment/paren/brace/string aware."""
    n = len(s)
    start = i
    # a nested bare block
    if s[i] == '{':
        return _match(s, i, '{', '}'), 'block'
    # label:  IDENT:  or case ...:  or default:
    mlab = re.match(r"(case\b[^:;{}]*|default\s*|[A-Za-z_]\w*)\s*:(?!:)", s[i:])
    if mlab and s[i:i+mlab.end()].count('{') == 0:
        # ensure it's a label, not the '?:' or a bitfield — good enough here
        return i + mlab.end(), 'label'
    # control flow
    mctl = re.match(r"(if|for|while|switch|do|else)\b", s[i:])
    if mctl:
        return _read_control(s, i), 'control'
    # otherwise read to the terminating ';' at paren/brace depth 0
    j, kind_text = _read_to_semi(s, i)
    text = s[start:j]
    body = _strip(text).strip()
    if DECL_RE.match(body):
        return j, 'decl'
    return j, 'stmt'


def _read_to_semi(s, i):
    n = len(s); depth = 0
    while i < n:
        c = s[i]
        if c == '/' and i+1 < n and s[i+1] == '*':
            k = s.find('*/', i+2); i = (k+2) if k >= 0 else n; continue
        if c == '/' and i+1 < n and s[i+1] == '/':
            k = s.find('\n', i); i = k if k >= 0 else n; continue
        if c in '"\'':
            i = _skip_str(s, i); continue
        if c in '([{':
            depth += 1
        elif c in ')]}':
            if depth == 0:
                return i, 'stmt'      # a stray close — stop
            depth -= 1
        elif c == ';' and depth == 0:
            return i + 1, 'stmt'
        i += 1
    return n, 'stmt'


def _read_control(s, i):
    """Read a control-flow statement fully (header + its sub-statement, which
    may itself be a block or another control statement)."""
    n = len(s)
    m = re.match(r"(if|for|while|switch|do|else)\b", s[i:])
    kw = m.group(1)
    i += m.end()
    i = _skip_ws_cmt(s, i)
    # optional parenthesized header (not for else/do)
    if i < n and s[i] == '(':
        i = _match(s, i, '(', ')')
    i = _skip_ws_cmt(s, i)
    if i >= n:
        return n
    # the sub-statement
    if s[i] == '{':
        i = _match(s, i, '{', '}')
    else:
        e, k = _read_statement(s, i)
        i = e
    # do-while tail / else
    i2 = _skip_ws_cmt(s, i)
    if kw == 'do' and re.match(r"while\b", s[i2:]):
        j = s.find(';', i2)
        i = (j + 1) if j >= 0 else i
    return i


def _match(s, i, op, cl):
    depth = 0; n = len(s)
    while i < n:
        c = s[i]
        if c == '/' and i+1 < n and s[i+1] == '*':
            k = s.find('*/', i+2); i = (k+2) if k >= 0 else n; continue
        if c == '/' and i+1 < n and s[i+1] == '/':
            k = s.find('\n', i); i = k if k >= 0 else n; continue
        if c in '"\'':
            i = _skip_str(s, i); continue
        if c == op:
            depth += 1
        elif c == cl:
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return n


def _skip_str(s, i):
    q = s[i]; i += 1; n = len(s)
    while i < n:
        if s[i] == '\\':
            i += 2; continue
        if s[i] == q:
            return i + 1
        i += 1
    return n


def _strip(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    s = re.sub(r"//[^\n]*", " ", s)
    return s


class Stats:
    def __init__(self):
        self.hoisted = 0
        self.skipped = 0


def process_block(s, open_i, close_i, stats):
    """Rewrite one block's body (s[open_i+1:close_i-1] is the interior). Returns
    the rewritten FULL block text s[open_i:close_i]. Recurses into nested
    blocks. Hoists declarations that appear after the first statement."""
    interior_start = open_i + 1
    interior_end = close_i - 1
    i = interior_start
    seen_stmt = False
    hoist = []          # (decl_text_to_insert, replacement_for_original, a, b)
    pieces = []         # rewritten sub-parts, we rebuild via recursion
    # We recurse into nested blocks by rewriting them first (right to left to
    # keep offsets stable would be complex; instead build a new string).
    out = []
    while i < interior_end:
        j = _skip_ws_cmt(s, i)
        out.append(s[i:j])
        i = j
        if i >= interior_end:
            break
        end, kind = _read_statement(s, i)
        seg = s[i:end]
        if kind == 'block':
            out.append(process_block(s, i, end, stats))
        elif kind == 'control':
            out.append(_rewrite_control(s, i, end, stats))
            seen_stmt = True
        elif kind == 'decl':
            if seen_stmt:
                rep = _split_decl(seg)
                if rep is not None:
                    decl_only, assign = rep
                    hoist.append(decl_only)
                    out.append(assign)
                    stats.hoisted += 1
                else:
                    stats.skipped += 1
                    out.append(seg)
            else:
                out.append(seg)
        elif kind == 'label':
            out.append(seg)
        else:  # stmt
            out.append(seg)
            seen_stmt = True
        i = end
    body = "".join(out)
    if hoist:
        indent = _guess_indent(s, open_i)
        head = "\n" + "\n".join(indent + h for h in hoist)
        body = head + body
    return "{" + body + "}"


def _rewrite_control(s, a, b, stats):
    """Recurse into a control statement's block body if it has one."""
    # find the sub-statement block(s) inside and rewrite them
    # simplest: re-scan the control text for a top-level '{...}' body and recurse
    m = re.match(r"\s*(if|for|while|switch|do|else)\b", s[a:b])
    kw = m.group(1)
    # locate the first '{' that starts the body (after the header parens)
    i = a + m.end()
    n = b
    # skip header parens
    i = _skip_ws_cmt(s, i)
    if i < n and s[i] == '(':
        i = _match(s, i, '(', ')')
    i = _skip_ws_cmt(s, i)
    if i < n and s[i] == '{':
        blk_end = _match(s, i, '{', '}')
        rewritten = process_block(s, i, blk_end, stats)
        return s[a:i] + rewritten + s[blk_end:b]
    return s[a:b]


def _split_decl(text):
    """`TYPE name = init;` -> ('TYPE name;', 'name = init;'); `TYPE name;` ->
    ('TYPE name;', ''). Returns None if not a clean single declarator."""
    body = _strip(text).strip()
    m = SIMPLE.match(body)
    if not m:
        return None
    pre = m.group('pre').strip()
    name = m.group('name')
    arr = m.group('arr') or ""
    init = m.group('init')
    # drop a leading storage class from the hoisted decl? keep it (static local
    # stays static). Keep as-is.
    decl_only = "%s %s%s;" % (pre, name, arr)
    decl_only = re.sub(r"\s+", " ", decl_only).strip()
    if init:
        rhs = init[1:].strip().rstrip(";").strip()
        assign = "%s = %s;" % (name, rhs)
        return (decl_only, assign)
    return (decl_only, "")


def _guess_indent(s, open_i):
    # indent = whitespace at the start of the line the '{' is on, + 4 spaces
    ls = s.rfind('\n', 0, open_i) + 1
    lead = re.match(r"[ \t]*", s[ls:open_i]).group(0)
    return lead + "    "


def transform(src):
    stats = Stats()
    # process each top-level function block. Find top-level '{...}' blocks.
    out = []
    i, n = 0, len(src)
    while i < n:
        c = src[i]
        if c == '/' and i+1 < n and src[i+1] == '*':
            k = src.find('*/', i+2); e = (k+2) if k >= 0 else n
            out.append(src[i:e]); i = e; continue
        if c == '/' and i+1 < n and src[i+1] == '/':
            k = src.find('\n', i); e = k if k >= 0 else n
            out.append(src[i:e]); i = e; continue
        if c in '"\'':
            e = _skip_str(src, i); out.append(src[i:e]); i = e; continue
        if c == '{':
            e = _match(src, i, '{', '}')
            out.append(process_block(src, i, e, stats))
            i = e
            continue
        out.append(c)
        i += 1
    return "".join(out), stats


def main():
    path = sys.argv[1]
    src = open(path, encoding="utf-8", errors="replace").read()
    new, st = transform(src)
    print("%s: hoisted %d declaration(s), skipped %d unclear"
          % (path, st.hoisted, st.skipped))
    if "--dry" not in sys.argv and new != src:
        open(path, "w", encoding="utf-8").write(new)
        print("  written")


if __name__ == "__main__":
    main()
