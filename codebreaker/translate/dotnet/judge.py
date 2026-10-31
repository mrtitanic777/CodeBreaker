"""judge.py -- the judge for whole programs, with the .NET adapter: run a program in its own runtime and its
translation in the target's, and compare what they do (docs/TRANSLATOR_DESIGN.md, "The judge, for every
pair").

What is compared is what a program says to the outside: its standard output, byte for byte after
line endings are made the same, and its exit code. A verdict is one of

    equivalent   both ran; the same output and exit code
    divergent    both ran; the output or the exit code differs (the first differing line is kept)
    refused      the translator declined something in the program (its ledger says what and why), so
                 the translation is not complete and is not run
    no-compile   the translation did not compile (the compiler's first errors are kept)
    no-original  the original did not build or run (nothing to judge against)

Adapters (each -> Run): `csharp` (the installed .NET SDK's csc and runtime, no restore), `java` (javac
and the JVM of the Java lane), `cpp` (the host C++ compiler: a transpiled program and its runtime).

    r = judge.csharp(['Prog.cs'], work)            # Run(ok, exit, out, err, log)
    v = judge.compare(original_run, translated_run)
"""
import os
import shutil
import subprocess

TIMEOUT = 120


class Run(object):
    """One program run: built (ok), its exit code, standard output and error; log: the build's output."""
    __slots__ = ('ok', 'exit', 'out', 'err', 'log')

    def __init__(self, ok, exit=None, out='', err='', log=''):
        self.ok, self.exit, self.out, self.err, self.log = ok, exit, out, err, log

    def __repr__(self):
        return 'Run(ok=%r, exit=%r, out=%d chars)' % (self.ok, self.exit, len(self.out or ''))


def _norm(s):
    return (s or '').replace('\r\n', '\n').rstrip('\n')


def compare(orig, trans, refused=None):
    """-> (verdict, detail)."""
    if refused:
        return 'refused', '; '.join(sorted(set(refused)))[:400]
    if not orig.ok or orig.exit is None:
        return 'no-original', (orig.log or orig.err or '')[-400:]
    if not trans.ok:
        return 'no-compile', (trans.log or '')[:600]
    a, b = _norm(orig.out), _norm(trans.out)
    if trans.exit is None:
        return 'divergent', 'the translation did not finish (timeout)'
    if a == b and orig.exit == trans.exit:
        return 'equivalent', '%d lines' % (a.count('\n') + 1 if a else 0)
    al, bl = a.split('\n'), b.split('\n')
    for i in range(max(len(al), len(bl))):
        x = al[i] if i < len(al) else '<end>'
        y = bl[i] if i < len(bl) else '<end>'
        if x != y:
            return 'divergent', 'line %d: original %r, translation %r' % (i + 1, x[:160], y[:160])
    return 'divergent', 'exit code %r, translation %r' % (orig.exit, trans.exit)


def _fresh(work):
    if os.path.isdir(work):
        shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work, exist_ok=True)
    return work


# ---------------------------------------------------------------------------------------------- .NET
def csharp(files, work, args=(), timeout=TIMEOUT, name='Program'):
    """Compile C# files with the SDK's csc and run the program on the installed runtime."""
    from codebreaker.translate import dotnet
    _fresh(work)
    dll = os.path.join(work, name + '.dll')
    ok, log = dotnet.compile(files, dll, tmp=os.path.join(work, 'tmp'))
    if not ok:
        return Run(False, log=_errors(log))
    code, out, err = dotnet.run(dll, args, timeout=timeout, tmp=os.path.join(work, 'tmp'))
    return Run(True, code, out, err, log)


def _errors(log):
    lines = [l for l in (log or '').splitlines() if ': error ' in l]
    return '\n'.join(lines[:12]) or (log or '')[-800:]


# ---------------------------------------------------------------------------------------------- the JVM
def java(src_dir, main, work, args=(), timeout=TIMEOUT, classpath=()):
    """Compile a Java source tree with javac and run class `main` (qualified) on the JVM."""
    from codebreaker.verify.lanes import java as vj
    javac, jvm = vj.jdk()
    if not javac:
        return Run(False, log='no JDK')
    _fresh(work)
    classes = os.path.join(work, 'classes')
    os.makedirs(classes)
    srcs = []
    for dp, _d, fs in os.walk(src_dir):
        srcs += [os.path.join(dp, f) for f in fs if f.endswith('.java')]
    listing = os.path.join(work, 'files.txt')
    with open(listing, 'w', encoding='utf-8') as f:
        f.write('\n'.join('"%s"' % s.replace('\\', '/') for s in sorted(srcs)))
    cp = os.pathsep.join([classes] + list(classpath))
    r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '-d', classes, '-cp', cp, '@' + listing],
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=600)
    if r.returncode != 0:
        return Run(False, log=r.stdout.decode('utf-8', 'replace')[-1500:])
    try:
        p = subprocess.run([jvm, '-cp', cp, main] + list(args), stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                           timeout=timeout)
    except subprocess.TimeoutExpired:
        return Run(True, None, '', 'timeout')
    return Run(True, p.returncode, p.stdout.decode('utf-8', 'replace'), p.stderr.decode('utf-8', 'replace'))
