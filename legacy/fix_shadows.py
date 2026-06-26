#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Propagate the WorldChunkManagerHell fix and detect any OTHER inherited-field
shadows introduced by the rename (a child field renamed to match an ancestor
field name -- compiles but silently shadows). javac already catches the
type-mismatch ones; this catches the same-type ones it can't."""

import os
import re
import shutil

WS = r"C:/Users/mrtit/Desktop/MinecraftAlpha"
TINY = os.path.join(WS, "conf", "mappings.tiny")
BAK = TINY + ".bak-0"
SRC = os.path.join(WS, "minecraft", "src", "net", "minecraft", "src")
SRC2 = os.path.join(WS, "minecraft", "source", "net", "minecraft", "src")
PLACEHOLDER = re.compile(r'^(func|field)_\d+_[A-Za-z]\w*$')

# --- Step A: propagate the hand-fix to source/, then fix the mapping entries ---
shutil.copyfile(os.path.join(SRC, "WorldChunkManagerHell.java"),
                os.path.join(SRC2, "WorldChunkManagerHell.java"))

lines = open(TINY, encoding='utf-8').read().split('\n')
WCMH = {'temperature': 'fixedTemperature', 'humidity': 'fixedHumidity'}
cur, out = None, []
for ln in lines:
    p = ln.split('\t')
    if p and p[0] == 'c':
        cur = p[1].split('/')[-1] if len(p) > 1 else None
    if cur == 'WorldChunkManagerHell' and len(p) >= 4 and p[0] == '' and p[1] == 'f' and p[3] in WCMH:
        p[3] = WCMH[p[3]]
        out.append('\t'.join(p)); continue
    out.append(ln)
open(TINY, 'w', encoding='utf-8').write('\n'.join(out))
lines = out
print("Step A: WorldChunkManagerHell fix propagated to source/ + mappings.tiny")

# --- Step B: detect same-type inherited-field shadows from my renames ---
# class -> current field names (from mapping)
cls_fields, cur = {}, None
for ln in lines:
    p = ln.split('\t')
    if p and p[0] == 'c':
        cur = p[1].split('/')[-1] if len(p) > 1 else None
        cls_fields.setdefault(cur, set())
    elif len(p) >= 4 and p[0] == '' and p[1] == 'f':
        cls_fields.setdefault(cur, set()).add(p[3])

# class -> parent (from source 'extends')
parent = {}
EXT = re.compile(r'\bclass\s+(\w+)\s+extends\s+(\w+)')
for fn in os.listdir(SRC):
    if fn.endswith('.java'):
        m = EXT.search(open(os.path.join(SRC, fn), encoding='utf-8', errors='replace').read())
        if m:
            parent[m.group(1)] = m.group(2)

# field renames I made (old placeholder -> new), with class context
renamed, cur = [], None
old_lines = open(BAK, encoding='utf-8').read().split('\n')
for o, n in zip(old_lines, lines):
    po, pn = o.split('\t'), n.split('\t')
    if po and po[0] == 'c':
        cur = po[1].split('/')[-1] if len(po) > 1 else None
    if (len(po) >= 4 and len(pn) >= 4 and po[0] == '' and po[1] == 'f'
            and po[3] != pn[3] and PLACEHOLDER.match(po[3])):
        renamed.append((cur, po[3], pn[3]))

def ancestors(c):
    seen = []
    p = parent.get(c)
    guard = {c}
    while p and p not in guard:
        seen.append(p); guard.add(p)
        p = parent.get(p)
    return seen

shadows = []
for cls, old, new in renamed:
    for anc in ancestors(cls):
        if new in cls_fields.get(anc, set()):
            shadows.append((cls, old, new, anc))
            break

print("Step B: scanned %d field renames" % len(renamed))
if not shadows:
    print("  no inherited-field shadows found -- clean.")
else:
    print("  %d shadow(s) found (child field renamed to an ancestor's field name):" % len(shadows))
    for cls, old, new, anc in shadows:
        print("   %s.%s -> '%s'  collides with %s.%s" % (cls, old, new, anc, new))
