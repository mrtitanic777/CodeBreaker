#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Propagate our mappings.tiny renames into conf/exceptions.exc.

exceptions.exc keys each entry by the NAMED method (e.g.
`net/minecraft/src/TexturePackCustom/func_6485_a (..)V java/io/IOException`).
When we renamed func_/field_ placeholders in mappings.tiny, those entries went
stale, so RDI's fixExceptions() no longer re-adds the `throws` clause on
re-decompile -> unreachable catch -> compile error (TexturePackList). This
rewrites the method name in each affected line to the new name, keeping the
exceptions data consistent with the mapping. Backs up exceptions.exc first."""

import os
import re

WS = r"C:/Users/mrtit/Desktop/MinecraftAlpha"
TINY = os.path.join(WS, "conf", "mappings.tiny")
BAK = TINY + ".bak-0"
EXC = os.path.join(WS, "conf", "exceptions.exc")
PLACEHOLDER = re.compile(r'^(func|field)_\d+_[A-Za-z]\w*$')


def rename_map():
    """old placeholder name -> new name, from the bak->current mappings.tiny diff."""
    old = open(BAK, encoding='utf-8').read().split('\n')
    new = open(TINY, encoding='utf-8').read().split('\n')
    m = {}
    for o, n in zip(old, new):
        po, pn = o.split('\t'), n.split('\t')
        if (len(po) >= 4 and len(pn) >= 4 and po[0] == '' and po[1] in ('m', 'f')
                and po[3] != pn[3] and PLACEHOLDER.match(po[3])):
            m[po[3]] = pn[3]
    return m


def main():
    rmap = rename_map()
    print("rename map: %d symbols" % len(rmap))
    lines = open(EXC, encoding='utf-8').read().split('\n')
    out, changed = [], 0
    for ln in lines:
        if not ln.strip():
            out.append(ln); continue
        head = ln.split(' ', 1)            # token0 = class/.../method
        path = head[0].split('/')
        method = path[-1]
        if method in rmap:
            path[-1] = rmap[method]
            head[0] = '/'.join(path)
            out.append(' '.join(head))
            changed += 1
        else:
            out.append(ln)
    if not os.path.exists(EXC + '.bak'):
        os.rename(EXC, EXC + '.bak')
    else:
        # keep first backup; overwrite working copy
        pass
    with open(EXC, 'w', encoding='utf-8') as f:
        f.write('\n'.join(out))
    print("exceptions.exc entries updated: %d" % changed)


if __name__ == '__main__':
    main()
