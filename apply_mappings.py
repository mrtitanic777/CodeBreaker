#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
apply_mappings.py -- write filled-in names from the worklist back into mappings.tiny.

Reads the worklist CSV (the `new_name` column you filled), and for every row with
a name, rewrites that symbol's `named` column in conf/mappings.tiny. After this,
re-run RetroMCP's decompile and the new names propagate to every reference in the
source (and downstream into the C++ port).

Safety:
  * always writes a timestamped backup (mappings.tiny.bak-N) first
  * validates each new_name is a legal Java identifier
  * refuses a name that would collide with an existing name in the same class
  * --dry-run prints the before/after lines and writes nothing

Usage:
  python apply_mappings.py [worklist.csv] [workspace_dir] [--dry-run]
"""

import csv
import os
import re
import sys

CSV_PATH = "alpha_unmapped_worklist.csv"
WORKSPACE = r"C:/Users/mrtit/Desktop/MinecraftAlpha"
DRY = '--dry-run' in sys.argv
_args = [a for a in sys.argv[1:] if not a.startswith('--')]
if len(_args) > 0:
    CSV_PATH = _args[0]
if len(_args) > 1:
    WORKSPACE = _args[1]
TINY = os.path.join(WORKSPACE, "conf", "mappings.tiny")

_IDENT = re.compile(r'^[A-Za-z_]\w*$')
_JAVA_KEYWORDS = {
    'abstract', 'assert', 'boolean', 'break', 'byte', 'case', 'catch', 'char', 'class',
    'const', 'continue', 'default', 'do', 'double', 'else', 'enum', 'extends', 'final',
    'finally', 'float', 'for', 'goto', 'if', 'implements', 'import', 'instanceof', 'int',
    'interface', 'long', 'native', 'new', 'package', 'private', 'protected', 'public',
    'return', 'short', 'static', 'strictfp', 'super', 'switch', 'synchronized', 'this',
    'throw', 'throws', 'transient', 'try', 'void', 'volatile', 'while', 'true', 'false', 'null',
}


def load_renames(path):
    """symbol -> new_name, for rows with a non-empty, valid new_name."""
    out, bad = {}, []
    with open(path, newline='', encoding='utf-8') as f:
        for row in csv.DictReader(f):
            new = (row.get('new_name') or '').strip()
            sym = (row.get('symbol') or '').strip()
            if not new or not sym:
                continue
            if not _IDENT.match(new) or new in _JAVA_KEYWORDS:
                bad.append((sym, new)); continue
            out[sym] = new
    return out, bad


def main():
    if not os.path.isfile(CSV_PATH):
        print('worklist not found:', CSV_PATH); return 2
    if not os.path.isfile(TINY):
        print('mappings.tiny not found:', TINY); return 2

    renames, bad = load_renames(CSV_PATH)
    if bad:
        print('Skipped %d invalid names (not legal Java identifiers):' % len(bad))
        for s, n in bad[:10]:
            print('   %s -> %r' % (s, n))
    if not renames:
        print('No valid new_name entries to apply. Fill the new_name column first.'); return 0
    print('Applying %d rename(s).' % len(renames))

    lines = open(TINY, encoding='utf-8').read().split('\n')
    # collision guard: existing named symbols per class
    cur, names_by_class, line_class = None, {}, []
    for ln in lines:
        p = ln.split('\t')
        if p and p[0] == 'c':
            cur = p[1] if len(p) > 1 else None
            names_by_class.setdefault(cur, set())
        elif len(p) >= 4 and p[0] == '' and p[1] in ('m', 'f'):
            names_by_class.setdefault(cur, set()).add(p[3])
        line_class.append(cur)

    applied, collisions, out = 0, [], []
    for idx, ln in enumerate(lines):
        p = ln.split('\t')
        if len(p) >= 4 and p[0] == '' and p[1] in ('m', 'f') and p[3] in renames:
            new = renames[p[3]]
            cls = line_class[idx]
            if new in names_by_class.get(cls, set()):
                collisions.append((cls, p[3], new)); out.append(ln); continue
            if DRY and applied < 30:
                print('  [%s] %s\n     -> %s' % (cls, ln.strip(), new))
            p[3] = new
            out.append('\t'.join(p))
            applied += 1
        else:
            out.append(ln)

    if collisions:
        print('\n%d collision(s) skipped (name already used in that class):' % len(collisions))
        for c, s, n in collisions[:10]:
            print('   %s: %s -> %s (exists)' % (c, s, n))

    print('\n%s %d symbol(s).' % ('Would rename' if DRY else 'Renamed', applied))
    if DRY:
        print('(dry run -- nothing written)'); return 0

    n = 0
    while os.path.exists('%s.bak-%d' % (TINY, n)):
        n += 1
    bak = '%s.bak-%d' % (TINY, n)
    os.rename(TINY, bak)
    with open(TINY, 'w', encoding='utf-8') as f:
        f.write('\n'.join(out))
    print('Backup: %s' % bak)
    print('Wrote:  %s' % TINY)
    print('\nNext: re-run RetroMCP decompile so the new names propagate to the source.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
