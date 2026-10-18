# -*- coding: utf-8 -*-
"""
statements/scan.py -- the lowest-level text primitive shared across the statements package.

Leaf module (only `re` + the shared literal pattern); nothing here knows Java/C++ semantics, so both
the package __init__ and the sibling transform modules (expr.py, ...) can import it with no cycle.
"""

from ...textutil import STR_LITERAL as _STR_LITERAL


def sub_outside_literals(rx, repl, s):
    """re.sub(rx, repl, s) applied ONLY to the parts of s outside string/char literals (renaming a
    word inside "stone" would corrupt sound/texture resource names, etc.)."""
    out, last = [], 0
    for m in _STR_LITERAL.finditer(s):
        out.append(rx.sub(repl, s[last:m.start()]))
        out.append(m.group(0))
        last = m.end()
    out.append(rx.sub(repl, s[last:]))
    return ''.join(out)


def _split_top_commas(expr):
    """Split on top-level commas (respecting (), [], {}, and string/char literals). '<>'
    is NOT tracked -- it would mis-balance on '->' and comparisons; call-arg generics with
    commas don't occur in this emitted code."""
    parts, depth, buf, i, n = [], 0, [], 0, len(expr)
    while i < n:
        c = expr[i]
        if c == '"' or c == "'":
            q = c; buf.append(c); i += 1
            while i < n:
                buf.append(expr[i])
                if expr[i] == '\\' and i + 1 < n:
                    buf.append(expr[i + 1]); i += 2; continue
                if expr[i] == q:
                    i += 1; break
                i += 1
            continue
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif c == ',' and depth == 0:
            parts.append(''.join(buf)); buf = []; i += 1; continue
        buf.append(c); i += 1
    if buf:
        parts.append(''.join(buf))
    return [p.strip() for p in parts]


def _balanced_paren(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '(':
            depth += 1
        elif s[i] == ')':
            depth -= 1
            if depth == 0:
                return i
    return -1


def _balanced_brace(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '{':
            depth += 1
        elif s[i] == '}':
            depth -= 1
            if depth == 0:
                return i
    return -1


def _balanced_from(s, i):
    """s[i] must be '('. Return (inner, index_after_close) for the balanced paren group."""
    assert s[i] == '('
    depth, j = 0, i
    while j < len(s):
        if s[j] == '(':
            depth += 1
        elif s[j] == ')':
            depth -= 1
            if depth == 0:
                return s[i + 1:j], j + 1
        j += 1
    return None, len(s)


def _balanced_bracket(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '[':
            depth += 1
        elif s[i] == ']':
            depth -= 1
            if depth == 0:
                return i
    return -1
