#!/usr/bin/env python3
"""gate.py -- the ported NE reader against the Python one: the same files through both, every fact compared.

    python native/ported/ne/gate.py [--exe PATH] [--limit N] [--corpus DIR]

Each file of the corpus that starts with "MZ" goes through codebreaker.formats.ne (Python) and
ne_facts (the C++ the Python front end wrote from it, built from native/ported/ne). The facts
are the reader's own answers -- the header, the name tables, module references, entries,
segments, relocations, resources, imported names, embedded modules -- or, where the reader
raises, the exception's name. Facts the port does not have (refused functions) are left out of
the comparison and counted. Prints the gate table and the speed of both.
"""
import json
import os
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
sys.path.insert(0, ROOT)

from codebreaker.formats import ne  # noqa: E402

FACTS = ['ne_off', 'h', 'resident_names', 'nonresident_names', 'module_name', 'module_refs', 'entries',
         'export_ordinals', 'resources', 'segments', 'relocations', 'imported_names', 'embedded']


def ename(e):
    n = type(e).__name__
    return 'StructError' if n == 'error' else n


def call(fn):
    try:
        return fn()
    except Exception as e:
        return {'error': ename(e)}


def facts(path):
    out = {'path': path}
    try:
        f = ne.NEFile(path)
    except Exception as e:
        out['error'] = ename(e)
        return out
    out['ne_off'] = f.ne_off
    out['h'] = f.h
    for m in ('resident_names', 'nonresident_names', 'module_name', 'module_refs', 'entries', 'export_ordinals',
              'resources', 'segments'):
        out[m] = call(getattr(f, m))
    segs = out['segments']
    if isinstance(segs, list):
        out['relocations'] = [call(lambda s=s: f.relocations(s)) for s in segs]
    else:
        out['relocations'] = segs
    out['imported_names'] = call(lambda: [f.imported_name(f.u16(f.ne_off + f.h['mod_tab'] + 2 * i))
                                          for i in range(f.h['n_modrefs'])])
    out['embedded'] = call(lambda: ne.embedded(f.data))
    return out


def norm(x):
    """JSON's view (tuples are lists)"""
    return json.loads(json.dumps(x))


def corpus_files(root, limit=None):
    out = []
    for dp, dirs, files in os.walk(root):
        dirs.sort()
        for fn in sorted(files):
            p = os.path.join(dp, fn)
            try:
                with open(p, 'rb') as fh:
                    if fh.read(2) != b'MZ':
                        continue
            except OSError:
                continue
            out.append(p)
    return out[:limit] if limit else out


def main(argv):
    exe = argv[argv.index('--exe') + 1] if '--exe' in argv else os.path.join(HERE, 'build', 'ne_facts.exe')
    limit = int(argv[argv.index('--limit') + 1]) if '--limit' in argv else None
    if '--corpus' in argv:
        root = argv[argv.index('--corpus') + 1]
    else:
        from codebreaker.base import settings
        root = settings.get('corpus')
    files = corpus_files(root, limit)
    work = os.path.join(ROOT, 'state', 'work', 'py2cpp')
    os.makedirs(work, exist_ok=True)
    listing = os.path.join(work, 'ne_gate_files.txt')
    with open(listing, 'w', encoding='utf-8') as f:
        f.write('\n'.join(files) + '\n')
    t = time.perf_counter()
    py = [norm(facts(p)) for p in files]
    py_secs = time.perf_counter() - t
    t = time.perf_counter()
    r = subprocess.run([exe, listing], capture_output=True, text=True, encoding='utf-8')
    cpp_wall = time.perf_counter() - t
    if r.returncode:
        print(r.stderr)
        return 1
    cpp_secs = float(r.stderr.split()[-1]) if r.stderr.startswith('seconds') else cpp_wall
    cpp = [json.loads(line) for line in r.stdout.splitlines()]
    assert len(cpp) == len(py), (len(cpp), len(py))
    identical = 0
    per = dict((k, [0, 0, 0]) for k in FACTS)          # fact -> [compared, identical, not in the port]
    diffs = []
    ne_files = errors = 0
    for a, b in zip(py, cpp):
        assert a['path'] == b['path']
        if 'error' in a or 'error' in b:
            errors += 1
            same = a.get('error') == b.get('error')
            identical += same
            if not same:
                diffs.append((a['path'], 'error', a.get('error'), b.get('error')))
            continue
        ne_files += 1
        have = set(b['_have'])
        same = True
        for k in FACTS:
            if k not in have:
                per[k][2] += 1
                continue
            per[k][0] += 1
            if a.get(k) == b.get(k):
                per[k][1] += 1
            else:
                same = False
                diffs.append((a['path'], k, a.get(k), b.get(k)))
        identical += same
    print('files %d (NE modules %d, raised on open %d): identical %d' % (len(files), ne_files, errors, identical))
    print('| fact | compared | identical | not in the port |')
    print('|---|---|---|---|')
    for k in FACTS:
        c, i, n = per[k]
        print('| %s | %d | %d | %d |' % (k, c, i, n))
    print('Python %.2f s, C++ %.2f s (process wall %.2f s): x%.1f' % (py_secs, cpp_secs, cpp_wall, py_secs / max(cpp_secs, 1e-9)))
    for d in diffs[:12]:
        print('DIFF', d[0], d[1], str(d[2])[:300], '|', str(d[3])[:300])
    return 0 if not diffs else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
