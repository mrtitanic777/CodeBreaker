#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mapping_worklist.py -- rank RetroMCP's UNMAPPED symbols by how much naming them
would clean up the source.

For each `func_N_x` / `field_N_x` still wearing a placeholder name, it joins:
  * reference count across the decompiled source (impact), and
  * the symbol's signature + defining class from conf/mappings.tiny (context),
then writes a fill-in worksheet (CSV) sorted by impact. You fill the `new_name`
column; a follow-up step rewrites those names back into mappings.tiny and you
re-run RetroMCP -- the names then propagate to every reference (and downstream
into the C++ port).

Usage: python mapping_worklist.py [workspace_dir] [out.csv]
"""

import csv
import os
import re
import sys
from collections import Counter

WORKSPACE = sys.argv[1] if len(sys.argv) > 1 else r"C:/Users/mrtit/Desktop/MinecraftAlpha"
OUT_CSV = sys.argv[2] if len(sys.argv) > 2 else "alpha_unmapped_worklist.csv"
SRC = os.path.join(WORKSPACE, "minecraft", "source")
TINY = os.path.join(WORKSPACE, "conf", "mappings.tiny")

_PLACEHOLDER = re.compile(r'^(func|field)_\d+_[A-Za-z]\w*$')
_TOKEN = re.compile(r'\b(?:func|field)_\d+_[A-Za-z]\w*\b')
_PRIM = {'B': 'byte', 'C': 'char', 'D': 'double', 'F': 'float', 'I': 'int',
         'J': 'long', 'S': 'short', 'Z': 'boolean', 'V': 'void'}


def _read_type(desc, i):
    arr = 0
    while desc[i] == '[':
        arr += 1; i += 1
    c = desc[i]
    if c in _PRIM:
        t = _PRIM[c]; i += 1
    elif c == 'L':
        j = desc.index(';', i); t = desc[i + 1:j].split('/')[-1]; i = j + 1
    else:
        t = c; i += 1
    return t + '[]' * arr, i


def pretty(desc):
    """JVM descriptor -> readable. Method '(I)Lx;' -> '(int) -> x'; field 'Lx;' -> 'x'."""
    if not desc:
        return '?'
    if desc[0] == '(':
        i, params = 1, []
        while desc[i] != ')':
            t, i = _read_type(desc, i); params.append(t)
        ret, _ = _read_type(desc, i + 1)
        return '(%s) -> %s' % (', '.join(params), ret)
    t, _ = _read_type(desc, 0)
    return t


def parse_tiny(path):
    """named placeholder symbol -> (kind, descriptor, defining_class)."""
    info = {}
    cur = None
    with open(path, encoding='utf-8', errors='replace') as f:
        for line in f:
            p = line.rstrip('\n').split('\t')
            if p and p[0] == 'c':
                cur = p[1].split('/')[-1] if len(p) > 1 else None
            elif len(p) >= 4 and p[0] == '' and p[1] in ('m', 'f'):
                kind = 'method' if p[1] == 'm' else 'field'
                desc, named = p[2], p[3]
                if _PLACEHOLDER.match(named):
                    info[named] = (kind, desc, cur)
    return info


def count_refs(src_root):
    counts = Counter()
    for dp, _, fs in os.walk(src_root):
        for fn in fs:
            if fn.endswith('.java'):
                txt = open(os.path.join(dp, fn), encoding='utf-8', errors='replace').read()
                for tok in _TOKEN.findall(txt):
                    counts[tok] += 1
    return counts


def main():
    if not os.path.isfile(TINY):
        print('mappings.tiny not found:', TINY); return 2
    if not os.path.isdir(SRC):
        print('source dir not found:', SRC); return 2

    info = parse_tiny(TINY)
    counts = count_refs(SRC)

    rows = []
    for sym, n in counts.items():
        kind, desc, cls = info.get(sym, (sym.split('_')[0].replace('func', 'method'), '', '?'))
        rows.append((n, kind, sym, cls or '?', pretty(desc)))
    rows.sort(key=lambda r: (-r[0], r[1], r[2]))

    with open(OUT_CSV, 'w', newline='', encoding='utf-8') as f:
        w = csv.writer(f)
        w.writerow(['refs', 'kind', 'symbol', 'defining_class', 'signature', 'new_name'])
        for n, kind, sym, cls, sig in rows:
            w.writerow([n, kind, sym, cls, sig, ''])

    methods = [r for r in rows if r[1] == 'method']
    fields = [r for r in rows if r[1] == 'field']
    print('Unmapped symbols referenced in source: %d  (methods=%d, fields=%d)'
          % (len(rows), len(methods), len(fields)))
    print('Total placeholder references: %d' % sum(counts.values()))
    print('Worklist written: %s\n' % os.path.abspath(OUT_CSV))

    def show(title, items, k=20):
        print('=== top %d %s (by references) ===' % (k, title))
        print('%5s  %-16s  %-22s  %s' % ('refs', 'symbol', 'class', 'signature'))
        for n, kind, sym, cls, sig in items[:k]:
            print('%5d  %-16s  %-22s  %s' % (n, sym, cls[:22], sig[:46]))
        print()

    show('METHODS', methods, 25)
    show('FIELDS', fields, 25)
    return 0


if __name__ == '__main__':
    sys.exit(main())
