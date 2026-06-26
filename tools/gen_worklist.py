# -*- coding: utf-8 -*-
"""
gen_worklist.py -- compile the converted output and write a per-file failure worklist.

    python tools/gen_worklist.py <out_dir> [--cc g++] [--std c++03] [--out overrides/WORKLIST.md]

For each <out_dir>/*.cpp it runs `<cc> -std=<std> -fsyntax-only -I<out_dir> <file>` and records
the files that fail to compile, sorted by error count (fewest first = closest to clean), with the
dominant error category. This drives the handwritten-override push: low-count files are usually a
small converter nudge or tiny override; high-count files want a full fresh override.

The output dir must already contain a stub `stdafx.h` (std includes + using min/max + NULL).
"""

import argparse
import collections
import glob
import os
import re
import subprocess
import sys

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


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument('out_dir')
    ap.add_argument('--cc', default=os.environ.get('J2X_CC', 'g++'))
    ap.add_argument('--std', default='c++03')
    ap.add_argument('--out', default=os.path.join('overrides', 'WORKLIST.md'))
    a = ap.parse_args(argv)

    cpps = sorted(glob.glob(os.path.join(a.out_dir, '*.cpp')))
    rows, clean = [], 0
    for cpp in cpps:
        r = subprocess.run([a.cc, '-std=' + a.std, '-fsyntax-only', '-I' + a.out_dir, cpp],
                           stderr=subprocess.PIPE, stdout=subprocess.DEVNULL)
        err = r.stderr.decode('utf-8', 'replace')
        n = sum(1 for l in err.splitlines() if _ERR.search(l))
        if n == 0:
            clean += 1
            continue
        rows.append((n, os.path.basename(cpp), _category(err)))
    rows.sort()

    total = len(cpps)
    L = ['# Override worklist (%s)' % a.out_dir, '',
         '%d / %d clean; %d failing. Sorted by error count (fewest first).' % (clean, total, len(rows)),
         'Regenerate: `python tools/gen_worklist.py %s`' % a.out_dir, '',
         '| errs | file | dominant error |', '|---:|---|---|']
    L += ['| %d | %s | %s |' % r for r in rows]
    with open(a.out, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L) + '\n')
    print('clean=%d/%d  failing=%d  -> %s' % (clean, total, len(rows), a.out))
    return 0


if __name__ == '__main__':
    sys.exit(main())
