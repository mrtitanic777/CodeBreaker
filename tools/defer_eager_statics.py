#!/usr/bin/env python3
"""One-off: convert an override's EAGER static field definitions into deferred initStatics() form,
so the ordered j2x_initAll() driver controls their init order (the static-init-order crash class).

  Block* Block::s_stone = (new BlockStone(1,1))->...;   // eager, undefined cross-TU order
becomes
  Block* Block::s_stone;                                // zero-init (order-safe)
  // ... and, appended in source order, inside void Block::initStatics():
  Block::s_stone = (new BlockStone(1,1))->...;

Only top-level lines `Type Class::s_name = init;` (column 0, qualified static member) are touched --
NOT indented in-method assignments, NOT aggregate data arrays / JArray(data,N) ctor defs (no '=').
Source order is preserved so dependencies (e.g. Block sound statics before the blocks that use them)
stay correct. Run: python tools/defer_eager_statics.py overrides/Block.cpp overrides/MobSpawnerBase.cpp
You must add `static void initStatics();` to each class's header by hand.
"""
import os
import re
import sys

# Top-level static field DEFINITION with an initializer: `<type> <Class>::s_<name> = <init>;`
DEF = re.compile(r'^([A-Za-z_][\w:<>\*&,\s]*?\s[\*&]?)((\w+)::s_\w+)\s*=\s*(.+);\s*$')


def process(path):
    with open(path, 'r', encoding='utf-8', newline='') as f:
        text = f.read()
    nl = '\r\n' if '\r\n' in text else '\n'
    lines = text.split(nl)
    out = []
    inits = {}                       # class name -> [assignment lines] in source order
    order = []                       # class names in first-seen order
    for ln in lines:
        m = DEF.match(ln)
        if not m:
            out.append(ln)
            continue
        typ, qual, cls, init = m.group(1), m.group(2), m.group(3), m.group(4)
        out.append('%s%s;' % (typ, qual))            # zero-initialized definition
        if cls not in inits:
            inits[cls] = []
            order.append(cls)
        inits[cls].append('\t%s = %s;' % (qual, init))   # deferred assignment (source order)
    # append one initStatics() per class found
    body = []
    for cls in order:
        body.append('')
        body.append('// Deferred static initialization (ordered by j2x_initAll()): was eager static')
        body.append('// definitions whose cross-translation-unit init order is undefined.')
        body.append('void %s::initStatics()' % cls)
        body.append('{')
        body.extend(inits[cls])
        body.append('}')
    if not order:
        print('  (no eager static defs found in %s)' % os.path.basename(path))
        return
    out += body
    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(nl.join(out))
    print('  %s: deferred %d static def(s) across %s' %
          (os.path.basename(path), sum(len(v) for v in inits.values()), ', '.join(order)))


def main():
    for p in sys.argv[1:]:
        process(os.path.abspath(p))


if __name__ == '__main__':
    main()
