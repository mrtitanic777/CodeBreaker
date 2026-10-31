"""pairs.py -- the C# kit's language pairs, each judged: the source run in its own runtime beside the
translation in the target's (judge.py).

    from codebreaker.translate.dotnet import pairs
    pairs.csharp_to_csharp(files, work)          # Roslyn front end -> L2 -> C# back end
    pairs.java_to_csharp(src_dir, 'pkg.Main', work)
    pairs.csharp_to_cpp(files, work)             # Roslyn front end -> L2 -> the C++03 back end

Each -> Verdict(verdict, detail, refused, files): `refused` lists what a front or back end declined
(where, kind, why); `files` the translation written.
"""
import os
import shutil

from codebreaker.translate.dotnet import judge


class Verdict(object):
    __slots__ = ('verdict', 'detail', 'refused', 'files', 'orig', 'trans')

    def __init__(self, verdict, detail, refused=(), files=(), orig=None, trans=None):
        self.verdict, self.detail, self.refused, self.files = verdict, detail, list(refused), list(files)
        self.orig, self.trans = orig, trans

    def __repr__(self):
        return 'Verdict(%s: %s)' % (self.verdict, self.detail)


def _clean(path):
    if os.path.isdir(path):
        shutil.rmtree(path, ignore_errors=True)
    os.makedirs(path, exist_ok=True)
    return path


def csharp_units(files, work):
    """C# files -> ([ir.Unit], [refusal]) through the Roslyn front end."""
    from codebreaker.translate.frontends import csharp as fcs
    from codebreaker.translate.frontends.csharp import lower as L
    out = _clean(os.path.join(work, 'dump'))
    trees = fcs.dump(files, out)
    errs = fcs.errors(out)
    if errs:
        raise RuntimeError('C# did not compile: %s:%d %s' % (errs[0]['file'], errs[0]['line'], errs[0]['message']))
    units, refused = [], []
    for n, t in sorted(trees.items()):
        u, r = L.lower_with_refusals(t, n)
        units.append(u)
        refused += [('%s:%d' % (n, line), kind, why) for line, kind, why in r]
    return units, refused


def java_units(src_dir, work):
    """A Java source tree -> [ir.Unit] through the javac front end."""
    from codebreaker.translate.frontends import java as fj
    from codebreaker.translate.frontends.java import lower as L
    out = _clean(os.path.join(work, 'treedump'))
    units = fj.dump(src_dir, out)
    errs = [d for d in fj.diagnostics(out) if d['kind'] == 'ERROR']
    if errs:
        raise RuntimeError('Java did not compile: %s:%d %s' % (errs[0]['file'], errs[0]['line'], errs[0]['message']))
    res = []
    for p, t in sorted(units.items()):
        u = L.lower(t, p)
        u.lang = 'java'
        res.append(u)
    return res


def _write_csharp(units, work):
    from codebreaker.translate.backends import csharp as bcs
    prog = bcs.Program(units)
    paths = prog.write(_clean(os.path.join(work, 'cs')))
    return prog, paths


def csharp_to_csharp(files, work, main=None):
    units, refused = csharp_units(files, work)
    prog, paths = _write_csharp(units, work)
    refused += prog.ledger
    orig = judge.csharp(files, os.path.join(work, 'run-orig'), main=main)
    trans = judge.csharp(paths, os.path.join(work, 'run-trans'), main=main)
    v, d = judge.compare(orig, trans, ['%s %s: %s' % r for r in refused])
    return Verdict(v, d, refused, paths, orig, trans)


def java_to_csharp(src_dir, main, work):
    units = java_units(src_dir, work)
    prog, paths = _write_csharp(units, work)
    refused = list(prog.ledger)
    from codebreaker.translate import ir
    for u in units:
        for n in ir.walk(u):
            if isinstance(n, ir.Unsupported):
                refused.append((u.path, n.kind, n.raw or n.kind))
    orig = judge.java(src_dir, main, os.path.join(work, 'run-orig'))
    trans = judge.csharp(paths, os.path.join(work, 'run-trans'), main=main)
    v, d = judge.compare(orig, trans, ['%s %s: %s' % r for r in refused])
    return Verdict(v, d, refused, paths, orig, trans)


def csharp_to_cpp(files, work, main_class='Program'):
    """C# -> L2 -> the C++03 back end (through cs2cpp), built with the host C++ compiler."""
    from codebreaker.translate.dotnet import cs2cpp
    units, refused = csharp_units(files, work)
    cpp_dir, more, _stats = cs2cpp.convert(units, work)
    refused += more
    orig = judge.csharp(files, os.path.join(work, 'run-orig'))
    if refused:
        v, d = judge.compare(orig, judge.Run(False), ['%s %s: %s' % r for r in refused])
        return Verdict(v, d, refused, [cpp_dir], orig, None)
    trans = judge.cpp(cpp_dir, main_class, os.path.join(work, 'run-trans'))
    v, d = judge.compare(orig, trans)
    return Verdict(v, d, refused, [cpp_dir], orig, trans)
