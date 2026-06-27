# -*- coding: utf-8 -*-
"""One-time-ish cleanup of MSVC warnings in the handwritten overrides/ (.cpp) files. Overrides are
copied VERBATIM by the converter, so the emission passes in j2x.emit.statements never touch them --
this re-applies the SAFE ones directly, plus a couple of override-specific fixes:

  C4554  shift/bitwise precedence  -> statements._clarify_precedence (per line; bails on anything hairy)
  C4566  unicode char/string lits  -> statements._fix_unicode_literals
  C4101  unused catch variable     -> `catch (Type* e){}` with e unused-in-its-block -> `catch (Type*)`
  C4102  unreferenced goto label    -> drop `Lxxx_brk:;` / `Lxxx_cont:;` with no matching `goto`
  C4800  map.count() returned bool  -> `return x.count(k);` -> `return x.count(k) != 0;`

Idempotent. Run from the ConversionTool dir: `python tools/clean_override_warnings.py`.
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
from j2x.emit import statements as st   # noqa: E402

OVERRIDES = os.path.join(ROOT, 'overrides')


def _drop_unused_catch(text):
    """`catch (Type* name) { ...block... }` where name is unused IN ITS OWN BLOCK -> drop the name."""
    out = []
    last = 0
    # REQUIRE a real separator ('*' or whitespace) between type and var name, else the regex would
    # split a bare `catch (NoSuchFieldError)` into type='NoSuchFieldErr' + name='or' and corrupt it.
    for m in re.finditer(r'catch\s*\(\s*([\w:]+)(\s*\*\s*|\s+)([A-Za-z_]\w*)\s*\)\s*\{', text):
        typ, sep, name = m.group(1), m.group(2), m.group(3)
        star = '*' if '*' in sep else ''
        brace = m.end() - 1                       # index of the '{'
        depth, j = 0, brace
        while j < len(text):
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
                if depth == 0:
                    break
            j += 1
        block = text[brace + 1:j]
        if not re.search(r'\b' + re.escape(name) + r'\b', block):
            out.append(text[last:m.start()])
            out.append('catch (%s%s) {' % (typ, star))
            last = m.end()
    out.append(text[last:])
    return ''.join(out)


def _drop_unused_labels(text):
    gotos = set(re.findall(r'\bgoto\s+(\w+)\s*;', text))
    keep = []
    for ln in text.split('\n'):
        m = re.match(r'^\s*(\w+_(?:brk|cont)):\s*;?\s*$', ln)
        if m and m.group(1) not in gotos:
            continue                              # unreferenced label target -> drop
        keep.append(ln)
    return '\n'.join(keep)


def clean(text):
    text = _drop_unused_catch(text)
    text = _drop_unused_labels(text)
    lines = []
    for ln in text.split('\n'):
        ln = st._fix_unicode_literals(ln)
        ln = st._clarify_precedence(ln)
        # C4800: `return <recv>.count(<args>);` returned as bool
        ln = re.sub(r'(\breturn\s+.*\.count\([^;]*\))\s*;', r'\1 != 0;', ln)
        lines.append(ln)
    return '\n'.join(lines)


def main():
    changed = []
    for fn in sorted(os.listdir(OVERRIDES)):
        if not fn.endswith('.cpp'):
            continue
        p = os.path.join(OVERRIDES, fn)
        with open(p, encoding='utf-8', newline='') as f:
            src = f.read()
        nl = '\r\n' if '\r\n' in src else '\n'
        body = src.replace('\r\n', '\n')
        out = clean(body)
        if out != body:
            with open(p, 'w', encoding='utf-8', newline='') as f:
                f.write(out.replace('\n', nl))
            changed.append(fn)
    print('cleaned %d override files:' % len(changed))
    for c in changed:
        print('  ', c)


if __name__ == '__main__':
    main()
