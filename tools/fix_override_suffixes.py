#!/usr/bin/env python3
"""One-off: normalize Java numeric-literal suffixes (D/d -> double, L/l -> LL) in the handwritten
overrides so they compile under MSVC/XDK (g++ tolerated `0.0D`; the Xbox compiler rejects it).

Only touches REAL CODE -- skips // and /* */ comments (so the `/* ORIGINAL JAVA */` blocks keep
their authentic Java suffixes) and string/char literals. Run: python tools/fix_override_suffixes.py
"""
import os
import re
import sys

SUFFIX = re.compile(
    r'(?<![\w.])(\d+\.?\d*(?:[eE][+-]?\d+)?|\.\d+(?:[eE][+-]?\d+)?)([DdLl])(?![\w.])')


def _fix_code(code):
    def rep(m):
        num, suf = m.group(1), m.group(2)
        if suf in 'Ll':
            return num + 'LL'
        if '.' not in num and 'e' not in num and 'E' not in num:
            return num + '.0'
        return num
    return SUFFIX.sub(rep, code)


def fix_text(s):
    """Apply _fix_code to every CODE span; pass comments and string/char literals through verbatim."""
    out = []
    i, n = 0, len(s)
    code_start = 0

    def flush_code(end):
        out.append(_fix_code(s[code_start:end]))

    while i < n:
        c = s[i]
        two = s[i:i + 2]
        if two == '//':                              # line comment to EOL
            flush_code(i)
            j = s.find('\n', i)
            j = n if j < 0 else j
            out.append(s[i:j])
            i = code_start = j
        elif two == '/*':                            # block comment
            flush_code(i)
            j = s.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(s[i:j])
            i = code_start = j
        elif c == '"' or c == "'":                   # string / char literal
            flush_code(i)
            j = i + 1
            while j < n:
                if s[j] == '\\':
                    j += 2
                    continue
                if s[j] == c:
                    j += 1
                    break
                j += 1
            out.append(s[i:j])
            i = code_start = j
        else:
            i += 1
    flush_code(n)
    return ''.join(out)


def main():
    root = os.path.join(os.path.dirname(__file__), '..', 'overrides')
    root = os.path.abspath(root)
    changed = 0
    for name in sorted(os.listdir(root)):
        if not (name.endswith('.cpp') or name.endswith('.h')):
            continue
        path = os.path.join(root, name)
        with open(path, 'r', encoding='utf-8', newline='') as f:   # newline='' preserves CRLF/LF
            orig = f.read()
        fixed = fix_text(orig)
        if fixed != orig:
            with open(path, 'w', encoding='utf-8', newline='') as f:
                f.write(fixed)
            changed += 1
            print('  fixed', name)
    print('changed %d files' % changed)


if __name__ == '__main__':
    main()
