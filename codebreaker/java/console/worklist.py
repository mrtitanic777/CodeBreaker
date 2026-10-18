# -*- coding: utf-8 -*-
"""
worklist.py -- compile the converted output and write a per-file failure worklist.

    python -m codebreaker java worklist <out_dir> [--cc g++] [--std c++03] [--out WORKLIST.md]

For each <out_dir>/*.cpp it runs `<cc> -std=<std> -fsyntax-only -I<out_dir> <file>` and records
the files that fail to compile, sorted by error count (fewest first = closest to clean), with the
dominant error category.
"""

import collections
import glob
import os
import re
import subprocess

_ERR = re.compile(r': error:')
_CAT = re.compile(r'error: (.*)')


def _category(text):
    cats = collections.Counter()
    for line in text.splitlines():
        m = _CAT.search(line)
        if m:
            c = re.sub(r"'[^']*'", 'X', m.group(1))
            cats[re.sub(r'\d+', 'N', c)[:60]] += 1
    return cats.most_common(1)[0][0] if cats else '(unknown)'


def run(out_dir, cc=None, std='c++03', worklist_path=None):
    if not cc:
        from codebreaker.base import settings
        cc = settings.get('host_cxx') or 'g++'
    if worklist_path is None:
        worklist_path = 'WORKLIST.md'

    cpps = sorted(glob.glob(os.path.join(out_dir, '*.cpp')))
    rows, clean = [], 0
    for cpp in cpps:
        r = subprocess.run([cc, '-std=' + std, '-fsyntax-only', '-I' + out_dir, cpp],
                           stderr=subprocess.PIPE, stdout=subprocess.DEVNULL)
        err = r.stderr.decode('utf-8', 'replace')
        n = sum(1 for l in err.splitlines() if _ERR.search(l))
        if n == 0:
            clean += 1
            continue
        rows.append((n, os.path.basename(cpp), _category(err)))
    rows.sort()

    total = len(cpps)
    L = ['# Override worklist (%s)' % out_dir, '',
         '%d / %d clean; %d failing. Sorted by error count (fewest first).' % (clean, total, len(rows)),
         'Regenerate: `python -m j2ctool worklist %s`' % out_dir, '',
         '| errs | file | dominant error |', '|---:|---|---|']
    L += ['| %d | %s | %s |' % r for r in rows]
    with open(worklist_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L) + '\n')
    print('clean=%d/%d  failing=%d  -> %s' % (clean, total, len(rows), worklist_path))
    return 0
