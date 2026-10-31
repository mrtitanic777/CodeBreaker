"""judge_c_java.py -- the C and C++ corpora judged as Java (docs/TRANSLATOR_DESIGN.md, phase 5).

    python tools/judge_c_java.py [c|cpp|all|roundtrip] [--only NAME ...] [--keep] [--workers N] [--md FILE]

`roundtrip` judges Java -> C++ -> Java (judge_native_jvm.roundtrip) on the Java lane's fixture classes.

Each program of codebreaker/translate/frontends/clang/corpus/manifest.json is staged (a K&R test gets
its stdio include, a DOS file is cut at its Ctrl-Z), built and run natively with clang, translated
(clang front end -> L2 -> Java back end), compiled with javac and run on the JVM, and judged
(codebreaker/translate/judge_native_jvm.py): equivalent, divergent, refused, no-compile or
no-original, with the reason. The table goes to stdout (and --md FILE); the verdicts to
state/translate/judge-c-java/<lang>.json. Each program's working copy is deleted after its verdict
unless --keep.
"""
import collections
import glob
import json
import os
import shutil
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.base import settings                                   # noqa: E402
from codebreaker.translate import judge_native_jvm as judge             # noqa: E402

CORPUS = os.path.join(ROOT, 'codebreaker', 'translate', 'frontends', 'clang', 'corpus')


def programs(lang):
    """[(name, group, [files], options)] of the manifest's `lang` section ('c' or 'cpp')."""
    with open(os.path.join(CORPUS, 'manifest.json'), encoding='utf-8') as f:
        man = json.load(f)
    out, seen = [], set()
    for entry in man[lang]:
        opts = dict((k, entry[k]) for k in ('std', 'prepend', 'cut_eof', 'defines', 'headers') if k in entry)
        found = []
        if 'glob' in entry:
            pat = entry['glob'] if os.path.isabs(entry['glob']) else os.path.join(CORPUS, entry['glob'])
            for p in sorted(glob.glob(pat)):
                found.append([p])
        if 'files' in entry:
            found.append([p if os.path.isabs(p) else os.path.join(CORPUS, p) for p in entry['files']])
        if not found:
            out.append((entry.get('name') or entry.get('glob') or entry['files'][0], entry['group'], [], dict(opts, missing=True)))
        for files in found:
            base = os.path.basename(files[0])
            o = dict(opts)
            if base in entry.get('skip', {}):
                o['skip'] = entry['skip'][base]
            extra = entry.get('extra', {}).get(base, [])
            files = files + [os.path.join(os.path.dirname(files[0]), x) for x in extra]
            o['args'] = entry.get('args', {}).get(base, [])
            name = entry.get('name') or os.path.splitext(base)[0]
            stem = name
            k = 2
            while name.lower() in seen:
                name = '%s_%d' % (stem, k)
                k += 1
            seen.add(name.lower())
            if not all(os.path.isfile(p) for p in files):
                o['missing'] = True
            out.append((name, entry['group'], files, o))
    return out


def stage(files, opts, dest):
    """Copies of the program's files (and the headers beside them) with the manifest's changes."""
    if os.path.isdir(dest):
        shutil.rmtree(dest)
    os.makedirs(dest)
    here = os.path.dirname(files[0])
    headers = [os.path.join(here, f) for f in os.listdir(here) if f.lower().endswith(('.h', '.hpp'))] \
        if opts.get('headers', True) else []
    out = []
    for p in files + headers:
        with open(p, 'rb') as f:
            data = f.read()
        if opts.get('cut_eof') and b'\x1a' in data:
            data = data[:data.index(b'\x1a')]
        target = os.path.join(dest, os.path.basename(p))
        if p in files:
            if opts.get('prepend'):
                data = opts['prepend'].encode() + data
            if p.lower().endswith('.c') and os.path.basename(p) != os.path.basename(p).lower():
                target = os.path.join(dest, os.path.basename(p).lower())
            out.append(target)
        with open(target, 'wb') as f:
            f.write(data)
    return out


def run_one(item, keep):
    name, group, files, opts = item
    if opts.get('missing'):
        return dict(program=name, group=group, verdict='not-here', reason='not on this machine', tier=None)
    if opts.get('skip'):
        return dict(program=name, group=group, verdict='skipped', reason=opts['skip'], tier=None)
    work = settings.state('translate', 'judge-c-java', 'work', name)
    staged = stage(files, opts, os.path.join(work, 'src'))
    try:
        v = judge.judge(staged, os.path.join(work, 'run'), name=judge._class_name(name), std=opts.get('std'),
                        defines=opts.get('defines', ()), args=opts.get('args', ()))
    except Exception as e:                                   # a crash of the translator is a verdict too
        v = judge.Verdict(name, 'refused', 'translator error: %s: %s' % (type(e).__name__, str(e)[:300]))
    row = v.row()
    row.update(program=name, group=group, files=files)
    if v.java and keep:
        row['java'] = os.path.join(work, 'run', 'java')
    if not keep:
        shutil.rmtree(work, ignore_errors=True)
    return row


def roundtrip_files():
    """The round trip's Java: the Java lane's fixtures, the C++03 back end's test classes, the driver."""
    import re
    fixtures = os.path.join(ROOT, 'tests', 'java', 'fixtures', 'java')
    files = [os.path.join(fixtures, f + '.java') for f in ('Vec3D', 'MathHelper', 'AxisAlignedBB')]
    files.append(os.path.join(CORPUS, 'roundtrip', 'RoundTrip.java'))
    work = settings.state('translate', 'judge-c-java', 'roundtrip-in')
    os.makedirs(work, exist_ok=True)
    with open(os.path.join(ROOT, 'tests', 'translate', 'test_cpp03_bodies.py'), encoding='utf-8') as f:
        tests = f.read()
    for name in ('BASE', 'SUB'):
        body = re.search(r"%s = '''(.*?)'''" % name, tests, re.S).group(1)
        p = os.path.join(work, name.capitalize() + '.java')
        with open(p, 'w', encoding='utf-8', newline='\n') as f:
            f.write('package sample;\n\n' + body)
        files.append(p)
    return files


def run_roundtrip(keep):
    work = settings.state('translate', 'judge-c-java', 'roundtrip')
    v = judge.roundtrip(roundtrip_files(), 'sample.RoundTrip', work)
    row = dict(v.row(), program='Java -> C++ -> Java: Vec3D, MathHelper, AxisAlignedBB, Base, Sub, RoundTrip')
    if not keep:
        shutil.rmtree(work, ignore_errors=True)
    return row


def table(rows):
    lines = ['| program | verdict | tier | reason |', '|---|---|---|---|']
    for r in rows:
        lines.append('| %s | %s | %s | %s |' % (r['program'], r['verdict'], r.get('tier') or '',
                                              (r.get('reason') or '').replace('|', '\\|').replace('\n', ' ')[:220]))
    return '\n'.join(lines)


def main(argv):
    langs = [a for a in argv if a in ('c', 'cpp')] or (['c', 'cpp'] if 'all' in argv or not argv else ['c'])
    keep = '--keep' in argv
    only = []
    if '--only' in argv:
        i = argv.index('--only') + 1
        while i < len(argv) and not argv[i].startswith('--'):
            only.append(argv[i].lower())
            i += 1
    workers = int(argv[argv.index('--workers') + 1]) if '--workers' in argv else 2
    md = []
    if 'roundtrip' in argv:
        row = run_roundtrip(keep)
        head = 'Round trip: ' + row['verdict']
        print(head)
        print(table([row]))
        md += ['### ' + head, '', table([row]), '']
        langs = [a for a in argv if a in ('c', 'cpp')]
    for lang in langs:
        items = [p for p in programs(lang) if not only or p[0].lower() in only]
        with ThreadPoolExecutor(max_workers=max(1, min(workers, 2))) as pool:
            rows = list(pool.map(lambda it: run_one(it, keep), items))
        counts = collections.Counter(r['verdict'] for r in rows)
        head = '%s corpus: %d programs -- %s' % (lang.upper(), len(rows), ', '.join('%d %s' % (n, k) for k, n in counts.most_common()))
        print(head)
        print(table(rows))
        md += ['### ' + head, '', table(rows), '']
        if not only:
            out = settings.state('translate', 'judge-c-java', '%s.json' % lang)
            with open(out, 'w', encoding='utf-8') as f:
                json.dump(rows, f, indent=1)
    if '--md' in argv:
        with open(argv[argv.index('--md') + 1], 'w', encoding='utf-8') as f:
            f.write('\n'.join(md))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
