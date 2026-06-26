#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
converter_lint.py - flag KNOWN console-crash patterns in converted Xbox 360 C++.

Every rule here maps to a real bug we hit on the RGH console and hand-fixed. Run it on the converter
OUTPUT (or the live project) to catch those crash classes BEFORE building/deploying -- turning the
old "convert -> deploy -> crash -> debug" loop into pre-build warnings.

Severities:
  CRASH - this pattern caused a hard crash / hang on real hardware.
  LEAK  - leaks memory (Java GC'd it; C++ doesn't) -> eventual OOM.
  WARN  - suspicious; verify by hand.

CLI:  python converter_lint.py <dir-or-file> [--severity CRASH|LEAK|WARN]
Importable:  lint_file(path) -> [Finding]; lint_tree(dir) -> [Finding]
"""

import os
import re
import sys
import argparse
import collections

Finding = collections.namedtuple('Finding', 'file line severity rule message')

SEV_ORDER = {'CRASH': 0, 'LEAK': 1, 'WARN': 2}

# --- type-agnostic per-line patterns -----------------------------------------
_NONASCII = re.compile(r"'[^\x00-\x7f]'")
_GOTO = re.compile(r'\bgoto\s+(label\w+)\b')
_DISCARDED_NEW = re.compile(r'^\s*new\s+\w[\w:]*\s*\(.*\)\s*;\s*$')          # `new X(...);` (result thrown away)
_VALUE_TEMP_NEW = re.compile(r'\bnew\s+(Vec3D|AxisAlignedBB|MovingObjectPosition)\s*\(')
_CLEAR_BOTH = re.compile(r'D3DCLEAR_TARGET\s*\|\s*D3DCLEAR_ZBUFFER')
_LIST_DEREF = re.compile(r'\b(?:Block::)?blocksList\s*\[\s*\w+\s*\]\s*->|\b(?:Item::)?itemsList\s*\[\s*\w+\s*\]\s*->')
_NEW_ARRAY_FIELD = re.compile(r'\b(m_\w+)\s*=\s*new\s+\w[\w:]*\s*\[')


def _read(p):
    with open(p, 'r', encoding='utf-8', errors='replace') as f:
        return f.read()


def _string_fields(cpp_path):
    """std::string VALUE field names declared in the sibling .h (for the (NULL) rule)."""
    h = os.path.splitext(cpp_path)[0] + '.h'
    names = set()
    if os.path.isfile(h):
        for m in re.finditer(r'\bstd::string\s+(\w+)\s*;', _read(h)):
            names.add(m.group(1))
    return names


def lint_file(path):
    findings = []
    if not (path.endswith('.cpp') or path.endswith('.h')):
        return findings
    base = path   # Finding.file holds the FULL path (GUI opens it); CLI displays the basename.
    text = _read(path)
    lines = text.split('\n')

    # class-aware: std::string field set from NULL (THE console boot crash class)
    str_pats = []
    for nm in (_string_fields(path) if path.endswith('.cpp') else set()):
        str_pats.append((re.compile(r'\b%s\s*\(\s*NULL\s*\)' % re.escape(nm)), nm))
        str_pats.append((re.compile(r'\b%s\s*=\s*NULL\b' % re.escape(nm)), nm))

    # array-length companion: a `m_X = new T[..]` whose m_X_count is never assigned anywhere in the file
    new_array_fields = set(m.group(1) for m in _NEW_ARRAY_FIELD.finditer(text))
    count_unset = set(f for f in new_array_fields if not re.search(r'\b%s_count\s*=' % re.escape(f), text))

    for i, ln in enumerate(lines, 1):
        s = ln.strip()
        if s.startswith('//') or s.startswith('*'):
            continue
        for pat, nm in str_pats:
            if pat.search(ln):
                findings.append(Finding(base, i, 'CRASH', 'string-null',
                    "std::string '%s' set from NULL -> std::string(const char*=NULL) -> strlen(NULL) AV on console. Use \"\"." % nm))
        if _NONASCII.search(ln):
            findings.append(Finding(base, i, 'CRASH', 'char-mask',
                "non-ASCII char literal -> MSVC mangles it to '?' (63). Use 0xNNNN (the MathHelper sine-table mask bug)."))
        m = _GOTO.search(ln)
        if m:
            findings.append(Finding(base, i, 'CRASH', 'label-goto',
                "goto %s (Java labeled break/continue): verify it EXITs the loop / jumps to the do-while CONDITION -- else infinite loop (shore-water freeze)." % m.group(1)))
        if _DISCARDED_NEW.match(ln) or _VALUE_TEMP_NEW.search(ln):
            findings.append(Finding(base, i, 'LEAK', 'new-leak',
                "`new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it."))
        if _CLEAR_BOTH.search(ln):
            findings.append(Finding(base, i, 'WARN', 'clear-target',
                "Clear(TARGET|ZBUFFER): if this came from glClear(DEPTH) it should be ZBUFFER-only -- TARGET wipes the rendered world (the 'eraser' bug)."))
        if _LIST_DEREF.search(ln) and 'NULL' not in ln:
            findings.append(Finding(base, i, 'WARN', 'list-deref',
                "blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered."))

    for f in sorted(count_unset):
        # report at the first new[] line for that field
        for i, ln in enumerate(lines, 1):
            if re.search(r'\b%s\s*=\s*new\s+\w[\w:]*\s*\[' % re.escape(f), ln):
                findings.append(Finding(base, i, 'WARN', 'count-unset',
                    "array field '%s' is allocated but '%s_count' is never set in this file -> uninitialized length companion (garbage loop bound / bad_alloc)." % (f, f)))
                break

    return findings


def lint_tree(root):
    out = []
    for dp, _d, fns in os.walk(root):
        for fn in fns:
            if fn.endswith('.cpp') or fn.endswith('.h'):
                out.extend(lint_file(os.path.join(dp, fn)))
    out.sort(key=lambda f: (SEV_ORDER.get(f.severity, 9), f.file, f.line))
    return out


def main():
    ap = argparse.ArgumentParser(description='Lint converted Xbox 360 C++ for known crash patterns.')
    ap.add_argument('target', help='File or directory to lint')
    ap.add_argument('--severity', choices=['CRASH', 'LEAK', 'WARN'], default='WARN',
                    help='Minimum severity to report (default: WARN = all)')
    args = ap.parse_args()
    minsev = SEV_ORDER[args.severity]
    fs = lint_file(args.target) if os.path.isfile(args.target) else lint_tree(args.target)
    fs = [f for f in fs if SEV_ORDER.get(f.severity, 9) <= minsev]
    counts = collections.Counter(f.severity for f in fs)
    for f in fs:
        print('[%-5s] %s:%d  %s  -- %s' % (f.severity, os.path.basename(f.file), f.line, f.rule, f.message))
    print('\n%d findings  (CRASH=%d  LEAK=%d  WARN=%d)'
          % (len(fs), counts.get('CRASH', 0), counts.get('LEAK', 0), counts.get('WARN', 0)))


if __name__ == '__main__':
    main()
