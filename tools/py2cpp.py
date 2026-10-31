#!/usr/bin/env python3
"""py2cpp.py -- CodeBreaker's Python->C++ porting engine, run on CodeBreaker itself.

    python tools/py2cpp.py port codebreaker.formats.ne native/ported/ne/src [--checked] [--fast]
        translate one module (types inferred over the whole package, or with --fast over the modules
        that name it) and write its .hpp/.cpp under the folder, by package path
    python tools/py2cpp.py coverage [--checked] [--compile] [--out FILE.json] [--md FILE.md]
        translate every function of codebreaker/ and say, per function, translated / compiles /
        refused (with the reason); prints the table by package and the top refusal reasons

The front end is codebreaker/translate/frontends/python (docs/PYTHON_FRONTEND.md), the back end
codebreaker/translate/backends/cpp17.py, the support header native/ported/include/py.hpp.
Working files go under state/work/py2cpp/.
"""
import collections
import json
import os
import re
import shutil
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.translate.frontends import python as pyfe          # noqa: E402
from codebreaker.translate.backends import cpp17                     # noqa: E402

WORK = os.path.join(ROOT, 'state', 'work', 'py2cpp')
INCLUDE = os.path.join(ROOT, 'native', 'ported', 'include')


def log(msg):
    print(msg, flush=True)


def load(modules=None, policy='strict', quiet=False):
    """the Program: every module of codebreaker/ (or `modules`: {name: path}), solved"""
    t = time.time()
    prog = pyfe.Program()
    if modules is None:
        prog.add_tree(os.path.join(ROOT, 'codebreaker'), 'codebreaker')
    else:
        for name, path in modules.items():
            prog.add_module(name, path)
    prog.index()
    prog.int_policy = policy
    rounds = prog.solve(log=None if quiet else (lambda m: log('  ' + m)))
    log('%d modules, %d functions: types in %d rounds, %.0f s' % (len(prog.modules), len(prog.fns), rounds,
                                                                   time.time() - t))
    return prog


def users_of(module):
    """the module and every module of codebreaker/ whose text names it (a faster, smaller program)"""
    leaf = module.rsplit('.', 1)[-1]
    out = {}
    for dp, dirs, files in os.walk(os.path.join(ROOT, 'codebreaker')):
        dirs[:] = [d for d in dirs if d != '__pycache__']
        for f in files:
            if not f.endswith('.py'):
                continue
            p = os.path.join(dp, f)
            rel = os.path.relpath(p, ROOT)[:-3].replace(os.sep, '.')
            if rel.endswith('.__init__'):
                rel = rel[:-9]
            with open(p, encoding='utf-8') as fh:
                src = fh.read()
            if rel == module or re.search(r'\b%s\b' % re.escape(module), src) or \
                    re.search(r'from\s+%s\s+import\s+[^\n]*\b%s\b' % (re.escape(module.rsplit('.', 1)[0]), leaf), src):
                out[rel] = p
    return out


def write(results, outdir):
    """each module's header and source under outdir/<package path>; -> {module: (hpp, cpp, spans)}"""
    out = {}
    for mod, r in results.items():
        if r.unit is None:
            continue
        try:
            h, s, spans = cpp17.write_unit(r.unit)
        except cpp17.Decline as e:
            out[mod] = (None, None, {'__decline__': str(e)})
            continue
        hp = os.path.join(outdir, cpp17.header_path(mod))
        cp = os.path.join(outdir, cpp17.source_path(mod))
        os.makedirs(os.path.dirname(hp), exist_ok=True)
        with open(hp, 'w', encoding='utf-8', newline='\n') as f:
            f.write(h)
        with open(cp, 'w', encoding='utf-8', newline='\n') as f:
            f.write(s)
        out[mod] = (hp, cp, spans)
    return out


def cmd_port(args):
    module, outdir = args[0], args[1]
    policy = 'checked' if '--checked' in args else 'strict'
    prog = load(users_of(module) if '--fast' in args else None, policy)
    results = pyfe.translate(prog, [module], int_policy=policy)
    r = results[module]
    for q, i in sorted(r.refused.items()):
        log('  refused %-50s %s' % (q.split('.', 2)[-1], i.reason))
    log('%s: %d translated, %d refused' % (module, len(r.translated), len(r.refused)))
    files = write(results, outdir)
    for mod, (hp, cp, spans) in files.items():
        log('  wrote %s, %s' % (os.path.relpath(hp, ROOT), os.path.relpath(cp, ROOT)))


# ---------------------------------------------------------------------------------------------- compile
_ENV = None


def msvc():
    global _ENV
    if _ENV is None:
        from codebreaker.recompile.xenon import msvc_env
        env = dict(msvc_env())
        tmp = os.path.join(WORK, 'tmp')
        os.makedirs(tmp, exist_ok=True)
        env['TEMP'] = env['TMP'] = tmp
        _ENV = (env, shutil.which('cl', path=env.get('Path') or env.get('PATH')))
    return _ENV


def compile_one(src, incdir, objdir):
    """cl /c one source -> (ok, [(file, line, message)])"""
    env, cl = msvc()
    obj = os.path.join(objdir, os.path.basename(src) + '.%d.obj' % (abs(hash(src)) % 100000))
    r = subprocess.run([cl, '/nologo', '/c', '/std:c++17', '/EHsc', '/utf-8', '/W3', '/permissive-', '/bigobj',
                        '/I' + INCLUDE, '/I' + incdir, '/Fo' + obj, src], env=env, capture_output=True, text=True,
                       errors='replace', cwd=objdir)
    errs = []
    for line in (r.stdout + r.stderr).splitlines():
        m = re.match(r'(.+?)\((\d+)\)\s*:\s*(?:fatal )?error (C\d+): (.*)', line.strip())
        if m:
            errs.append((os.path.normpath(m.group(1)), int(m.group(2)), m.group(3) + ' ' + m.group(4)))
    if os.path.isfile(obj):
        os.remove(obj)
    return r.returncode == 0, errs


def cmd_coverage(args):
    policy = 'checked' if '--checked' in args else 'strict'
    out_json = args[args.index('--out') + 1] if '--out' in args else os.path.join(WORK, 'coverage-%s.json' % policy)
    out_md = args[args.index('--md') + 1] if '--md' in args else None
    prog = load(None, policy)
    t = time.time()
    results = pyfe.translate(prog, None, int_policy=policy)
    log('lowered in %.0f s' % (time.time() - t))
    src_dir = os.path.join(WORK, 'cov-' + policy, 'src')
    if os.path.isdir(src_dir):
        shutil.rmtree(src_dir)
    files = write(results, src_dir)
    status = {}                                     # fn -> (status, reason)
    for mod, r in results.items():
        for q in r.translated:
            status[q] = ('translated', '')
        for q, i in r.refused.items():
            status[q] = ('refused', i.reason)
        hp, cp, spans = files.get(mod, (None, None, {}))
        if '__decline__' in spans:
            for q in r.translated:
                status[q] = ('no-c++', 'back end: ' + spans['__decline__'])
    if '--compile' in args:
        from concurrent.futures import ThreadPoolExecutor
        objdir = os.path.join(WORK, 'cov-' + policy, 'obj')
        os.makedirs(objdir, exist_ok=True)
        todo = [(mod, cp) for mod, (hp, cp, spans) in files.items() if cp and results[mod].translated]
        log('compiling %d modules (2 workers) ...' % len(todo))
        t = time.time()
        with ThreadPoolExecutor(2) as pool:
            done = dict(zip([m for m, _ in todo], pool.map(lambda mc: compile_one(mc[1], src_dir, objdir), todo)))
        log('compiled in %.0f s' % (time.time() - t))
        for mod, (ok, errs) in done.items():
            r = results[mod]
            hp, cp, spans = files[mod]
            if ok:
                for q in r.translated:
                    status[q] = ('compiles', '')
                continue
            hit = set()
            for f, line, msg in errs:
                pinned = False
                if os.path.normcase(f) == os.path.normcase(os.path.normpath(cp)):
                    for q, (a, b) in spans.items():
                        if a <= line <= b and q in status:
                            hit.add(q)
                            status[q] = ('no-compile', msg[:120])
                            pinned = True
                if not pinned:                           # in a header (its own or another's): the module's
                    for q in r.translated:
                        if q not in hit:
                            status[q] = ('no-compile', 'header: ' + msg[:110])
                            hit.add(q)
            for q in r.translated:
                if q not in hit:
                    status[q] = ('compiles?', 'module failed elsewhere')
    report(prog, status, out_json, out_md, policy)


def package_of(q):
    parts = q.split('.')
    return parts[1] if len(parts) > 2 else '(top)'


def report(prog, status, out_json, out_md, policy):
    rows = collections.OrderedDict()
    reasons = collections.Counter()
    classes = collections.Counter()
    class_lines = collections.Counter()
    for q, f in sorted(prog.fns.items()):
        st, why = status.get(q, ('refused', 'frontend-error:not lowered'))
        pkg = package_of(q)
        row = rows.setdefault(pkg, collections.Counter())
        row['functions'] += 1
        row['lines'] += f.lines
        if st in ('translated', 'compiles', 'compiles?'):
            row['translated'] += 1
            row['translated_lines'] += f.lines
        if st == 'compiles':
            row['compiles'] += 1
            row['compiles_lines'] += f.lines
        if st == 'refused':
            c = why.split(':', 1)[0]
            classes[c] += 1
            class_lines[c] += f.lines
            reasons[_reason_key(why)] += 1
    tot = collections.Counter()
    for r in rows.values():
        tot.update(r)
    doc = {'policy': policy, 'packages': rows, 'total': tot, 'classes': classes, 'class_lines': class_lines,
           'reasons': reasons.most_common(60), 'functions': status}
    os.makedirs(os.path.dirname(out_json), exist_ok=True)
    with open(out_json, 'w', encoding='utf-8') as f:
        json.dump(doc, f, indent=1)
    lines = ['| package | functions | lines | translated | % lines | compiles | % lines |', '|---|---|---|---|---|---|---|']

    def pct(a, b):
        return '%.1f%%' % (100.0 * a / b) if b else '-'
    for pkg in sorted(rows, key=lambda p: -rows[p]['lines']):
        r = rows[pkg]
        lines.append('| %s | %d | %d | %d | %s | %d | %s |' % (pkg, r['functions'], r['lines'], r['translated'],
                                                               pct(r['translated_lines'], r['lines']), r['compiles'],
                                                               pct(r['compiles_lines'], r['lines'])))
    lines.append('| **all** | %d | %d | %d | %s | %d | %s |' % (tot['functions'], tot['lines'], tot['translated'],
                                                                pct(tot['translated_lines'], tot['lines']), tot['compiles'],
                                                                pct(tot['compiles_lines'], tot['lines'])))
    lines += ['', '| refusal class | functions | lines |', '|---|---|---|']
    for c, n in classes.most_common():
        lines.append('| %s | %d | %d |' % (c, n, class_lines[c]))
    lines += ['', '| top refusal reasons | functions |', '|---|---|']
    for k, n in reasons.most_common(40):
        lines.append('| %s | %d |' % (k.replace('|', '/'), n))
    text = '\n'.join(lines)
    print(text)
    if out_md:
        with open(out_md, 'w', encoding='utf-8') as f:
            f.write(text + '\n')
    log('wrote ' + os.path.relpath(out_json, ROOT))


def _reason_key(why):
    """a refusal reason with its particulars (names, numbers) taken out, so like ones count together"""
    c, _, d = why.partition(':')
    if c in ('stdlib', 'extern', 'dynamic', 'syntax'):
        d = re.sub(r"\(.*", '', d).strip()
        d = re.sub(r"\bof \w+$", '', d).strip()
        return '%s: %s' % (c, d[:70])
    if c == 'calls-refused':
        return 'calls-refused'
    if c == 'int-range':
        return 'int-range: ' + re.sub(r' in \[.*', '', d).split(' ')[0]
    if c == 'untyped':
        if d.startswith('parameter'):
            return 'untyped: parameter ' + ('(its callers pass values of no type)' if 'callers pass' in d
                                            else '(no annotation, no typed call site)')
        if d.startswith('attribute'):
            return 'untyped: attribute also stored a value of no type'
        return 'untyped: ' + re.sub(r"[\w.]+'?\s*$", '', d)[:50]
    if c == 'conflict':
        d = re.sub(r"'[^']*'", "'…'", d)
        d = re.sub(r'\b\d+\b', 'N', d)
        return 'conflict: ' + d[:70]
    return '%s: %s' % (c, d[:60])


def main(argv):
    if not argv or argv[0] not in ('port', 'coverage'):
        print(__doc__)
        return 2
    os.makedirs(WORK, exist_ok=True)
    return {'port': cmd_port, 'coverage': cmd_coverage}[argv[0]](argv[1:]) or 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
