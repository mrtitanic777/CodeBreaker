# -*- coding: utf-8 -*-
"""Makes the test data of the C# back end's Java library (native/cb/translate/backends/csharp_java_test.cpp): for each Java
program, the javac front end's units as L2 (NAME.l2.json) and what codebreaker/translate/backends/csharp.py (with
csharp_java.py) wrote for them (NAME.cs.txt: each file under a `// ==== file ====` line, the runtime's JavaRt.cs left out;
NAME.ledger.txt: what it declined, `where<TAB>kind<TAB>why`), reviewed by hand before it is checked in. Needs a JDK. Run from
the checkout root:

    python native/cb/translate/testdata/csharp_java/make_csharp_java_golden.py [name ...]
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..', '..', '..', '..'))
sys.path.insert(0, ROOT)

from codebreaker.translate import l2
from codebreaker.translate.backends import csharp as bcs
from codebreaker.translate.dotnet import pairs

CORPUS = os.path.join(ROOT, 'tests', 'translate', 'corpus', 'java')
PROGRAMS = {
    'calc': os.path.join(CORPUS, 'calc'),
    'fixtures': os.path.join(CORPUS, 'fixtures'),
    'modern': os.path.join(CORPUS, 'modern'),
}
for _n in ('calls', 'features', 'inner_outer', 'enum_object', 'hash_order', 'boxed_null', 'exception_cause'):
    PROGRAMS[_n] = os.path.join(HERE, 'src', _n)
WORK = os.path.join(ROOT, 'state', 'work', 'csharp_java_golden')


def _drop_columns(node):
    if isinstance(node, dict):
        node.pop('col', None)
        for v in node.values():
            _drop_columns(v)
    elif isinstance(node, list):
        for v in node:
            _drop_columns(v)


def main(names):
    for name, src in sorted(PROGRAMS.items()):
        if names and name not in names:
            continue
        units = pairs.java_units(src, os.path.join(WORK, name))
        docs = [l2.to_json(u) for u in units]
        _drop_columns(docs)
        with open(os.path.join(HERE, name + '.l2.json'), 'w', encoding='utf-8', newline='\n') as out:
            json.dump(docs, out, separators=(',', ':'))
        prog = bcs.Program([l2.from_json(d) for d in docs])
        text = ''
        for fname, body in prog.files().items():
            if fname == 'JavaRt.cs':
                continue
            text += '// ==== %s ====\n%s' % (fname, body if body.endswith('\n') else body + '\n')
        with open(os.path.join(HERE, name + '.cs.txt'), 'w', encoding='utf-8', newline='\n') as out:
            out.write(text)
        with open(os.path.join(HERE, name + '.ledger.txt'), 'w', encoding='utf-8', newline='\n') as out:
            for where, kind, why in prog.ledger:
                out.write('%s\t%s\t%s\n' % (where, kind, why.replace('\n', ' ')))
        print(name, 'ok', len(prog.ledger), 'ledger entries', 'JavaRt used' if 'JavaRt.cs' in prog.files() else '')


if __name__ == '__main__':
    main(sys.argv[1:])
