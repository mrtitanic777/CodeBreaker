# -*- coding: utf-8 -*-
"""
statements.py -- Module 4c: method-body / statement translation (the deep core).

For Module 4b this is a SEAM, not the real translator: it returns the original
Java body as commented-out lines so `emit/source.py` can lay out a correct,
safe .cpp structure (static defs, ctor init lists, destructors) without yet
attempting statement-level Java->C++03 translation.

4c will replace `translate_body` with the real thing: value/pointer access
(`.` vs `->` from the resolver), Java-API -> STL, null-deref guards, labeled
break -> goto, etc. Everything else in emit/ already consumes this contract, so
4c is a drop-in upgrade.
"""


def inner_lines(body):
    """The statement lines between the outermost braces of a parsed method body.

    `body` is the parser's list of (line_no, raw_text) spanning the method,
    which includes the signature's `{` line and the closing `}`.
    """
    text = '\n'.join(t for _ln, t in body)
    i = text.find('{')
    j = text.rfind('}')
    if i != -1 and j != -1 and j > i:
        text = text[i + 1:j]
    return [ln.rstrip() for ln in text.split('\n')]


def translate_body(method, cls, resolver, ledger=None):
    """4b stub: emit the Java body as comments, marked for 4c translation.
    Returns a list of C++ source lines (no indentation; the caller indents)."""
    stmts = [ln.strip() for ln in inner_lines(method.body) if ln.strip()]
    if not stmts:
        return []
    out = ['// PORT: body pending statement translation (Module 4c)']
    out.extend('// ' + s for s in stmts)
    return out
