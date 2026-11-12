# -*- coding: utf-8 -*-
"""Makes the Java back end's test data (native/cb/translate/backends/java_test.cpp) from the C programs beside it:
for each X.c the clang front end's L2 document (X.l2.json) and what codebreaker/translate/backends/java.py wrote for it
(X.faithful.java, X.notes.txt), reviewed by hand before it is checked in. Run from the checkout root:

    python native/cb/translate/testdata/java/make_java_golden.py [name ...]
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.abspath(os.path.join(HERE, '..', '..', '..', '..', '..')))

from codebreaker.translate import l2
from codebreaker.translate.backends import java as jb
from codebreaker.translate.frontends import clang


# programs the tests use only as input (the library mapping and the idiomatic tier are other modules')
NO_GOLDEN = ('CallsLib', 'ClassPoint')


def _drop_columns(node):
    if isinstance(node, dict):
        node.pop('col', None)
        for v in node.values():
            _drop_columns(v)
    elif isinstance(node, list):
        for v in node:
            _drop_columns(v)


def main(names):
    for f in sorted(os.listdir(HERE)):
        if not f.endswith('.c') and not f.endswith('.cpp'):
            continue
        stem = os.path.splitext(f)[0]
        if names and stem not in names:
            continue
        doc = clang.l2_file(os.path.join(HERE, f))
        doc['path'] = f      # (the checkout's own path stays out of the checked-in document)
        _drop_columns(doc)   # (smaller: nothing here reads a column)
        with open(os.path.join(HERE, stem + '.l2.json'), 'w', encoding='utf-8', newline='\n') as out:
            json.dump(doc, out, separators=(',', ':'))
        if stem in NO_GOLDEN:
            continue
        units = [l2.from_json(doc)]
        try:
            prog = jb.program(units, stem, tier='faithful')
            text = prog.source
            notes = prog.notes
        except jb.Refused as e:
            text = '// REFUSED: %s\n' % e
            notes = []
        with open(os.path.join(HERE, stem + '.faithful.java'), 'w', encoding='utf-8', newline='\n') as out:
            out.write(text)
        with open(os.path.join(HERE, stem + '.notes.txt'), 'w', encoding='utf-8', newline='\n') as out:
            out.write('\n'.join(notes) + ('\n' if notes else ''))
        print(stem, 'ok')


if __name__ == '__main__':
    main(sys.argv[1:])
