#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Partition the unmapped-symbol worklist into balanced per-class batches for
parallel naming. Each batch file lists classes and their unmapped symbols so a
worker can read each class's source and propose names."""

import csv
import os
import sys
from collections import defaultdict

CSV_PATH = "alpha_unmapped_worklist.csv"
NBATCHES = int(sys.argv[1]) if len(sys.argv) > 1 else 12
OUTDIR = "_batches"

rows = list(csv.DictReader(open(CSV_PATH, newline='', encoding='utf-8')))
by_class = defaultdict(list)
for r in rows:
    by_class[r['defining_class']].append(r)

# sort classes by symbol count desc, distribute round-robin to balance load
classes = sorted(by_class, key=lambda c: -len(by_class[c]))
bins = [[] for _ in range(NBATCHES)]
load = [0] * NBATCHES
for c in classes:
    i = load.index(min(load))
    bins[i].append(c)
    load[i] += len(by_class[c])

os.makedirs(OUTDIR, exist_ok=True)
for i, cls_list in enumerate(bins):
    with open(os.path.join(OUTDIR, 'batch_%02d.txt' % i), 'w', encoding='utf-8') as f:
        for c in sorted(cls_list):
            f.write('CLASS %s\n' % c)
            for r in sorted(by_class[c], key=lambda r: (r['kind'], r['symbol'])):
                f.write('  %s\t%s\t%s\trefs=%s\n' % (r['kind'], r['symbol'], r['signature'], r['refs']))
    print('batch_%02d.txt  classes=%d symbols=%d' % (i, len(cls_list), load[i]))
print('\nTotal: %d classes, %d symbols, %d batches' % (len(classes), len(rows), NBATCHES))
