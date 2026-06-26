#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Merge the per-batch naming results (_batches/out_*.csv) back into the worklist,
add confidence/note columns, and report coverage + within-class collisions."""

import csv
import glob
import os
import re
from collections import defaultdict

WORKLIST = "alpha_unmapped_worklist.csv"
PLACEHOLDER = re.compile(r'^(func|field)_\d+_[A-Za-z]\w*$')

# 1. gather agent proposals
proposals = {}
for path in sorted(glob.glob(os.path.join('_batches', 'out_*.csv'))):
    for r in csv.DictReader(open(path, newline='', encoding='utf-8')):
        sym = (r.get('symbol') or '').strip()
        name = (r.get('new_name') or '').strip()
        conf = (r.get('confidence') or '').strip().lower()
        note = (r.get('note') or '').strip()
        if not sym:
            continue
        # treat "named back to placeholder" or unchanged as blank
        if name == sym or PLACEHOLDER.match(name):
            name = ''
        proposals[sym] = (name, conf, note)

# 2. merge into worklist
rows = list(csv.DictReader(open(WORKLIST, newline='', encoding='utf-8')))
named = blank = 0
conf_count = defaultdict(int)
by_class_name = defaultdict(lambda: defaultdict(list))   # class -> new_name -> [symbols]
uncovered = []
for r in rows:
    sym = r['symbol']
    name, conf, note = proposals.get(sym, ('', '', ''))
    if sym not in proposals:
        uncovered.append(sym)
    r['new_name'] = name
    r['confidence'] = conf
    r['note'] = note
    if name:
        named += 1
        conf_count[conf or '?'] += 1
        by_class_name[r['defining_class']][name].append(sym)
    else:
        blank += 1

# 3. within-class collisions (same new_name on 2+ symbols in one class -> apply will skip)
collisions = []
for cls, names in by_class_name.items():
    for name, syms in names.items():
        if len(syms) > 1:
            collisions.append((cls, name, syms))

# 4. write merged worklist
cols = ['refs', 'kind', 'symbol', 'defining_class', 'signature', 'new_name', 'confidence', 'note']
with open(WORKLIST, 'w', newline='', encoding='utf-8') as f:
    w = csv.DictWriter(f, fieldnames=cols)
    w.writeheader()
    for r in rows:
        w.writerow({k: r.get(k, '') for k in cols})

print("worklist rows: %d" % len(rows))
print("named:   %d   (high=%d med=%d low=%d other=%d)" %
      (named, conf_count.get('high', 0), conf_count.get('med', 0),
       conf_count.get('low', 0), sum(v for k, v in conf_count.items() if k not in ('high', 'med', 'low'))))
print("blank:   %d" % blank)
print("uncovered by any batch: %d %s" % (len(uncovered), uncovered[:8]))
print("within-class name collisions: %d" % len(collisions))
for cls, name, syms in collisions[:12]:
    print("   %s: '%s' <- %s" % (cls, name, ', '.join(syms)))
