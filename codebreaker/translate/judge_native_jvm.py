"""judge_native_jvm.py -- the judge for C/C++ -> Java (docs/TRANSLATOR_DESIGN.md, "The judge, for every pair").

The original runs natively: clang compiles the C or C++ program (-O0, the front end's own target) and
it runs. The translation runs on the JVM: the clang front end lowers each file to L2, the Java back
end writes the program as Java, javac compiles it beside the C runtime (crt), and java runs it. Both
get the same arguments and the same stdin; their stdout, stderr and exit code are compared. Windows
text mode writes "\\r\\n" for "\\n" on the native side, so line ends are compared as "\\n".

Every program gets a verdict:
  equivalent  -- same stdout, stderr and exit code;
  divergent   -- both ran, something differs (the first difference is the reason);
  refused     -- the translator declined, and says why (front end Unsupported, back end Refused);
  no-compile  -- the Java it wrote does not compile (a back end bug: javac's first error is the reason);
  no-original -- the original itself does not build or run here.

    v = judge(['prog.c'], work_dir)            # -> Verdict
    v.verdict, v.reason, v.tier
"""
import hashlib
import os
import shutil
import subprocess

from codebreaker.translate import ir
from codebreaker.translate.backends import java as jback
from codebreaker.translate.frontends import clang as fe

RUN_TIMEOUT = 120


class Verdict(object):
    def __init__(self, name, verdict, reason='', tier=None, java=None, native=None, jvm=None, notes=()):
        self.name, self.verdict, self.reason, self.tier = name, verdict, reason, tier
        self.java, self.native, self.jvm, self.notes = java, native, jvm, list(notes)

    def __repr__(self):
        return 'Verdict(%s: %s%s)' % (self.name, self.verdict, ' -- ' + self.reason if self.reason else '')

    def row(self):
        return {'program': self.name, 'verdict': self.verdict, 'reason': self.reason, 'tier': self.tier}


def _jdk():
    from codebreaker.verify.lanes import java as _vj
    return _vj.jdk()


def runtime_classes():
    """crt compiled once (cached by its sources and the JDK) -> the class folder."""
    from codebreaker.base import settings
    javac, _java = _jdk()
    src = os.path.join(jback.RUNTIME, 'crt')
    files = sorted(os.path.join(src, f) for f in os.listdir(src) if f.endswith('.java'))
    h = hashlib.sha1(repr(javac).encode())
    for f in files:
        with open(f, 'rb') as fh:
            h.update(fh.read())
    folder = settings.state('translate', 'java-crt-' + h.hexdigest()[:12])
    if not os.path.isfile(os.path.join(folder, 'crt', 'LibC.class')):
        os.makedirs(folder, exist_ok=True)
        r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '--release', '17', '-d', folder] + files,
                           capture_output=True, text=True, timeout=300)
        if r.returncode:
            shutil.rmtree(folder, ignore_errors=True)
            raise RuntimeError('the C runtime for Java does not compile: ' + r.stdout + r.stderr)
    return folder


def _run(cmd, stdin, cwd, timeout=RUN_TIMEOUT):
    try:
        r = subprocess.run(cmd, input=stdin, capture_output=True, cwd=cwd, timeout=timeout)
        return r.stdout, r.stderr, r.returncode
    except subprocess.TimeoutExpired:
        return b'', b'(timed out)', 'timeout'


def _norm(b):
    return b.replace(b'\r\n', b'\n')


def native(files, out_exe, language, std=None, defines=(), includes=()):
    """Build the original with clang. -> (ok, message)."""
    exe = fe.clang()
    cmd = [exe] + fe.flags(language, std, includes, (), defines) + ['-O0', '-o', out_exe] + list(files)
    r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', timeout=300)
    return r.returncode == 0, (r.stderr or r.stdout)[-1500:]


def translate(files, name, language=None, std=None, defines=(), includes=()):
    """The program's files -> jback.Program (raises jback.Refused / RuntimeError)."""
    units = []
    for f in files:
        if not f.lower().endswith(('.c', '.cpp', '.cc', '.cxx')):
            continue
        units.append(_unit(f, language or fe.language_of(f), std, includes, (), defines))
    for u in units:
        bad = [x for x in ir.unsupported(u)]
        if bad:
            x = bad[0]
            raise jback.Refused('front end: %s at %s:%s%s' % (x.kind, os.path.basename(u.path or ''), x.line,
                                                             (' (%s)' % x.raw) if x.raw else ''))
    return jback.program(units, name)


def _unit(path, language, std, includes, quote_includes, defines, own=None):
    """A source file's IR, through the front end's cached L2 document (keyed by the source and the lowering)."""
    from codebreaker.translate import l2
    return l2.from_json(fe.l2_file(path, language, std, includes, quote_includes, defines, own))


def judge(files, work, name=None, language=None, std=None, defines=(), includes=(), stdin=b'', args=()):
    files = [os.path.abspath(f) for f in files]
    language = language or fe.language_of(files[0])
    name = name or _class_name(os.path.splitext(os.path.basename(files[0]))[0])
    os.makedirs(work, exist_ok=True)
    exe = os.path.join(work, name + '.exe')
    ok, msg = native([f for f in files if not f.lower().endswith(('.h', '.hpp'))], exe, language, std, defines, includes)
    if not ok:
        return Verdict(name, 'no-original', 'clang: ' + _first_error(msg))
    nat = _run([exe] + list(args), stdin, work)
    try:
        prog = translate(files, name, language, std, defines, includes)
    except jback.Refused as e:
        return Verdict(name, 'refused', str(e), native=nat)
    except RuntimeError as e:
        return Verdict(name, 'refused', 'front end: ' + str(e)[:300], native=nat)
    jdir = os.path.join(work, 'java')
    shutil.rmtree(jdir, ignore_errors=True)
    os.makedirs(jdir)
    src = os.path.join(jdir, name + '.java')
    with open(src, 'w', encoding='utf-8', newline='\n') as f:
        f.write(prog.source)
    javac, java = _jdk()
    crt = runtime_classes()
    classes = os.path.join(jdir, 'classes')
    os.makedirs(classes)
    r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '--release', '17', '-cp', crt, '-d', classes, src],
                       capture_output=True, text=True, errors='replace', timeout=300)
    if r.returncode:
        return Verdict(name, 'no-compile', 'javac: ' + _first_error(r.stdout + r.stderr), prog.tier, prog.source, nat,
                       notes=prog.notes)
    # (argv[0] is where the program runs from: the native one's own path, given to both)
    jvm = _run([java, '-Xss64m', '-Dc.argv0=' + exe, '-cp', os.pathsep.join([classes, crt]), name] + list(args),
               stdin, work)
    v = _compare(nat, jvm)
    gaps = [n[len('gap: '):] for n in prog.notes if n.startswith('gap: ')]
    if v is not None and b'CodeBreaker gap: ' in jvm[1]:
        # the run reached a part the translator declined: the program is refused, with that part's reason
        hit = jvm[1].decode('utf-8', 'replace').split('CodeBreaker gap: ', 1)[1].splitlines()[0]
        return Verdict(name, 'refused', 'gap reached: ' + hit, prog.tier, prog.source, nat, jvm, notes=prog.notes)
    reason = v or ''
    if gaps:                       # (an explicit gap the run did not reach still says it is there)
        reason = (reason + '; ' if reason else '') + 'gaps: ' + '; '.join(gaps)
    return Verdict(name, 'equivalent' if v is None else 'divergent', reason, prog.tier, prog.source, nat, jvm,
                   notes=prog.notes)


def roundtrip(java_files, main_class, work, name='RoundTripped'):
    """Java -> C++ -> Java, judged: the Java lane (javac front end -> IR -> the C++03 back end,
    codebreaker/java/transpile.convert) writes the classes as C++; the clang front end reads that C++ back
    (j2x.lift maps the Java runtime in C++ onto Java's library); the Java back end writes Java again. The
    original classes and the round-tripped ones run on the JVM; their output is compared.
    main_class: the qualified class whose main runs ('sample.RoundTrip'). -> Verdict (java: the Java
    written; notes: what was translated)."""
    import re
    from codebreaker.java import transpile
    from codebreaker.translate.frontends.clang import j2x
    javac, java = _jdk()
    shutil.rmtree(work, ignore_errors=True)
    src = os.path.join(work, 'src')
    classes = []
    for f in java_files:
        with open(f, encoding='utf-8') as fh:
            text = fh.read()
        m = re.search(r'^\s*package\s+([\w.]+)\s*;', text, re.M)
        pkg = m.group(1) if m else ''
        d = os.path.join(src, *pkg.split('.')) if pkg else src
        os.makedirs(d, exist_ok=True)
        with open(os.path.join(d, os.path.basename(f)), 'w', encoding='utf-8', newline='\n') as fh:
            fh.write(text)
        classes.append(os.path.splitext(os.path.basename(f))[0])
    # the original, on the JVM
    orig = os.path.join(work, 'orig')
    os.makedirs(orig)
    staged = [os.path.join(dp, f) for dp, _d, fs in os.walk(src) for f in fs if f.endswith('.java')]
    r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '-d', orig] + staged, capture_output=True, text=True,
                       errors='replace', timeout=300)
    if r.returncode:
        return Verdict(name, 'no-original', 'javac: ' + _first_error(r.stdout + r.stderr))
    nat = _run([java, '-cp', orig, main_class], b'', work)
    # Java -> C++ (the Java lane)
    cpp = os.path.join(work, 'cpp')
    try:
        s = transpile.convert(src, cpp, platform=None, write_report=False)
    except Exception as e:
        return Verdict(name, 'refused', 'Java -> C++: %s: %s' % (type(e).__name__, str(e)[:300]), native=nat)
    declined = (s.get('tree_bodies') or (0, {}))[1]
    # C++ -> L2 (clang) -> Java
    units = []
    try:
        for c in classes:
            cf, hf = os.path.join(cpp, c + '.cpp'), os.path.join(cpp, c + '.h')
            units.append(_unit(cf, 'c++', 'c++14', (), (cpp,), (), own=[cf, hf]))
        j2x.lift(units)
        bad = [x for u in units for x in ir.unsupported(u)]
        if bad:
            raise jback.Refused('front end: %s at line %s%s' % (bad[0].kind, bad[0].line,
                                                               (' (%s)' % bad[0].raw) if bad[0].raw else ''))
        prog = jback.program(units, name)
    except jback.Refused as e:
        return Verdict(name, 'refused', 'C++ -> Java: %s' % e, native=nat)
    except RuntimeError as e:
        return Verdict(name, 'refused', 'C++ -> Java: front end: %s' % str(e)[:300], native=nat)
    jdir = os.path.join(work, 'java')
    os.makedirs(jdir)
    out_src = os.path.join(jdir, name + '.java')
    with open(out_src, 'w', encoding='utf-8', newline='\n') as f:
        f.write(prog.source)
    crt = runtime_classes()
    out_classes = os.path.join(jdir, 'classes')
    os.makedirs(out_classes)
    r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '--release', '17', '-cp', crt, '-d', out_classes, out_src],
                       capture_output=True, text=True, errors='replace', timeout=300)
    notes = prog.notes + (['Java -> C++ declined bodies: %r' % (declined,)] if declined else [])
    if r.returncode:
        return Verdict(name, 'no-compile', 'javac: ' + _first_error(r.stdout + r.stderr), prog.tier, prog.source, nat,
                       notes=notes)
    jvm = _run([java, '-cp', os.pathsep.join([out_classes, crt]), name], b'', work)
    v = _compare(nat, jvm)
    return Verdict(name, 'equivalent' if v is None else 'divergent', v or '', prog.tier, prog.source, nat, jvm,
                   notes=notes)


def _compare(a, b):
    """None when the two runs agree, else what differs first."""
    (ao, ae, ac), (bo, be, bc) = a, b
    if _norm(ao) != _norm(bo):
        x, y = _norm(ao), _norm(bo)
        i = next((k for k in range(min(len(x), len(y))) if x[k] != y[k]), min(len(x), len(y)))
        line = x[:i].count(b'\n') + 1
        return 'stdout differs at line %d: C %r, Java %r' % (line, x[i - 20 if i > 20 else 0:i + 30],
                                                              y[i - 20 if i > 20 else 0:i + 30])
    if ac != bc:
        return 'exit code: C %s, Java %s%s' % (ac, bc, (' (%s)' % _first_error(be.decode('utf-8', 'replace')))
                                               if be else '')
    if _norm(ae) != _norm(be):
        return 'stderr differs: C %r, Java %r' % (_norm(ae)[:120], _norm(be)[:120])
    return None


def _first_error(text):
    for line in text.splitlines():
        if 'error' in line.lower() or 'exception' in line.lower():
            return line.strip()[:300]
    return text.strip().splitlines()[0][:300] if text.strip() else ''


def _class_name(stem):
    import re
    n = re.sub(r'\W', '_', stem)
    if not n or n[0].isdigit():
        n = 'P' + n
    return n[:1].upper() + n[1:]
