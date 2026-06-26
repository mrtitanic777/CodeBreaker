#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Resolve the 3 same-type inherited-field shadows precisely:
  EntityPickupFX.yOffset (private, local)  -> targetYOffset  (Entity.yOffset is canonical)
  EntityLiving.cameraYaw / prevCameraYaw   -> reverted to placeholders (mis-named parent;
                                              cameraYaw is really EntityPlayer's, kept there)
Edits source (src/ + source/), mappings.tiny (class-scoped), and the worklist."""

import csv
import os
import re

WS = r"C:/Users/mrtit/Desktop/MinecraftAlpha"
TINY = os.path.join(WS, "conf", "mappings.tiny")
WORKLIST = "alpha_unmapped_worklist.csv"

# per-class source token edits (word-boundary, applied only in that class's file)
SRC_EDITS = {
    'EntityPickupFX': [('yOffset', 'targetYOffset')],
    'EntityLiving':   [('prevCameraYaw', 'field_9329_Q'), ('cameraYaw', 'field_9328_R')],
}
# class-scoped mapping renames (current named -> corrected named)
MAP_EDITS = {
    'EntityPickupFX': {'yOffset': 'targetYOffset'},
    'EntityLiving':   {'cameraYaw': 'field_9328_R', 'prevCameraYaw': 'field_9329_Q'},
}
# worklist symbol -> corrected new_name ('' = back to placeholder)
WL_EDITS = {
    'field_676_r': 'targetYOffset',
    'field_9328_R': '',
    'field_9329_Q': '',
}


def src_paths(cls):
    return [os.path.join(WS, 'minecraft', sub, 'net', 'minecraft', 'src', cls + '.java')
            for sub in ('src', 'source')]


for cls, subs in SRC_EDITS.items():
    for p in src_paths(cls):
        if not os.path.isfile(p):
            continue
        t = open(p, encoding='utf-8', newline='').read()
        for old, new in subs:
            t = re.sub(r'\b' + re.escape(old) + r'\b', new, t)
        open(p, 'w', encoding='utf-8', newline='').write(t)
print("source files patched")

lines = open(TINY, encoding='utf-8').read().split('\n')
cur, out = None, []
for ln in lines:
    p = ln.split('\t')
    if p and p[0] == 'c':
        cur = p[1].split('/')[-1] if len(p) > 1 else None
    if cur in MAP_EDITS and len(p) >= 4 and p[0] == '' and p[1] == 'f' and p[3] in MAP_EDITS[cur]:
        p[3] = MAP_EDITS[cur][p[3]]
        out.append('\t'.join(p)); continue
    out.append(ln)
open(TINY, 'w', encoding='utf-8').write('\n'.join(out))
print("mappings.tiny patched")

rows = list(csv.DictReader(open(WORKLIST, newline='', encoding='utf-8')))
for r in rows:
    if r['symbol'] in WL_EDITS:
        r['new_name'] = WL_EDITS[r['symbol']]
        if not r['new_name']:
            r['confidence'] = 'low'; r['note'] = 'reverted (parent-field shadow)'
cols = ['refs', 'kind', 'symbol', 'defining_class', 'signature', 'new_name', 'confidence', 'note']
with open(WORKLIST, 'w', newline='', encoding='utf-8') as f:
    w = csv.DictWriter(f, fieldnames=cols); w.writeheader()
    for r in rows:
        w.writerow({k: r.get(k, '') for k in cols})
print("worklist updated")
