# -*- coding: utf-8 -*-
"""Makes the Java back end's idiomatic-tier test data (native/cb/translate/backends/java_objects_test.cpp) from the programs in
objects/: for each X.c / X.cpp the clang front end's L2 document (X.l2.json) and what codebreaker/translate/backends/java.py (with
java_objects.py and java_libc.py) wrote for it as the idiomatic tier (X.idiomatic.java, X.notes.txt), reviewed by hand before it is
checked in. Run from the checkout root:

    python native/cb/translate/testdata/java/make_java_objects_golden.py [name ...]
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.abspath(os.path.join(HERE, '..', '..', '..', '..', '..')))

from codebreaker.translate import l2
from codebreaker.translate.backends import java as jb
from codebreaker.translate.frontends import clang

SRC = os.path.join(HERE, 'objects')


def _drop_columns(node):
    if isinstance(node, dict):
        node.pop('col', None)
        for v in node.values():
            _drop_columns(v)
    elif isinstance(node, list):
        for v in node:
            _drop_columns(v)


def main(names):
    for f in sorted(os.listdir(SRC)):
        if not f.endswith('.c') and not f.endswith('.cpp'):
            continue
        stem = os.path.splitext(f)[0]
        if names and stem not in names:
            continue
        doc = clang.l2_file(os.path.join(SRC, f))
        doc['path'] = f      # (the checkout's own path stays out of the checked-in document)
        _drop_columns(doc)
        with open(os.path.join(SRC, stem + '.l2.json'), 'w', encoding='utf-8', newline='\n') as out:
            json.dump(doc, out, separators=(',', ':'))
        units = [l2.from_json(doc)]
        try:
            prog = jb.program(units, stem, tier='idiomatic')
            text, notes, tier = prog.source, prog.notes, prog.tier
        except jb.Refused as e:
            text, notes, tier = '// REFUSED: %s\n' % e, [], 'refused'
        with open(os.path.join(SRC, stem + '.idiomatic.java'), 'w', encoding='utf-8', newline='\n') as out:
            out.write(text)
        with open(os.path.join(SRC, stem + '.notes.txt'), 'w', encoding='utf-8', newline='\n') as out:
            out.write('\n'.join(notes) + ('\n' if notes else ''))
        print(stem, tier)


if __name__ == '__main__':
    main(sys.argv[1:])
