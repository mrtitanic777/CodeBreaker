#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Propagate the names just applied to mappings.tiny into the decompiled source.

The rename map is derived from the DIFF between mappings.tiny.bak-0 (before) and
mappings.tiny (after), so it contains EXACTLY the symbols that were renamed in the
mapping -- collisions and low-confidence entries stay placeholders in both places,
avoiding any duplicate-name breakage. Then each placeholder token is replaced with
its new name across the source (word-boundary, single pass per file).

Renames BOTH src/ and source/ so RetroMCP's patch system sees no spurious diff.

Usage: python rename_source.py [workspace_dir]
"""

import os
import re
import sys

WS = sys.argv[1] if len(sys.argv) > 1 else r"C:/Users/mrtit/Desktop/MinecraftAlpha"
TINY = os.path.join(WS, "conf", "mappings.tiny")
BAK = TINY + ".bak-0"
SRC_DIRS = [os.path.join(WS, "minecraft", "src"), os.path.join(WS, "minecraft", "source")]
PLACEHOLDER = re.compile(r'^(func|field)_\d+_[A-Za-z]\w*$')
TOKEN = re.compile(r'\b(?:func|field)_\d+_[A-Za-z]\w*\b')


def build_map():
    old = open(BAK, encoding='utf-8').read().split('\n')
    new = open(TINY, encoding='utf-8').read().split('\n')
    if len(old) != len(new):
        print('WARNING: line counts differ (%d vs %d) -- apply may have reordered.' % (len(old), len(new)))
    m = {}
    for o, n in zip(old, new):
        po, pn = o.split('\t'), n.split('\t')
        if len(po) >= 4 and len(pn) >= 4 and po[0] == '' and po[1] in ('m', 'f'):
            if po[3] != pn[3] and PLACEHOLDER.match(po[3]):
                m[po[3]] = pn[3]
    return m


def main():
    if not os.path.isfile(BAK):
        print('backup not found (run apply_mappings.py first):', BAK); return 2
    rename = build_map()
    print('rename map: %d symbols' % len(rename))
    if not rename:
        return 0

    def repl(mo):
        return rename.get(mo.group(0), mo.group(0))

    files_changed = total = 0
    for root in SRC_DIRS:
        if not os.path.isdir(root):
            continue
        for dp, _, fs in os.walk(root):
            for fn in fs:
                if not fn.endswith('.java'):
                    continue
                p = os.path.join(dp, fn)
                with open(p, encoding='utf-8', newline='') as f:
                    text = f.read()
                new_text, n = TOKEN.subn(repl, text)
                # subn counts ALL token matches incl. unmapped; recount only real changes
                if new_text != text:
                    changed = sum(1 for _ in TOKEN.finditer(text)) - \
                              sum(1 for _ in TOKEN.finditer(new_text))
                    with open(p, 'w', encoding='utf-8', newline='') as f:
                        f.write(new_text)
                    files_changed += 1
                    total += changed
    print('files changed: %d' % files_changed)
    print('placeholder references replaced: %d' % total)
    return 0


if __name__ == '__main__':
    sys.exit(main())
