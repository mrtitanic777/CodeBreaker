"""javajudge.py -- the Java corpus judged on each back end, beside the JVM (docs/TRANSLATOR_DESIGN.md, phases 4 and 6).

Two ways a Java program reaches native code, and a judge for each:

  * through the transpiler (codebreaker/java): declarations from its front end, every body from the IR
    by a body back end -- cpp03 (the 360's) or C++17 (backends/cpp17.Bodies: cpp03.Bodies17). Judged class by class by
    the java lane and probe.py: each class compiled on its own, its methods run on the same inputs in
    the JVM and natively.        judge_tree(src, work, backend)

  * whole programs from L2 (declarations and bodies), with a runtime of their own: backends/cpp17.program (native)
    and backends/c_java.Program. Judged per program: every static method with primitive or String
    parameters and a primitive or String result is run on probe.py's inputs in the JVM and natively,
    and the program's main is run on both sides and its output compared.   judge_program(src, work, backend)

Verdicts: 'equivalent' (every result alike), 'divergent' (one differs, with the case), 'refused' (the back
end declined -- a J2X-GAP body, a Decline, or what it wrote does not compile), 'not-run' (nothing it could
compare).
"""
import os
import subprocess
import threading

EQUIVALENT, DIVERGENT, REFUSED, NOT_RUN = 'equivalent', 'divergent', 'refused', 'not-run'
_STD = {'cpp03': ('C++03', 'c++03'), 'cpp17': ('C++17', 'c++17')}


# ------------------------------------------------------------------------------ through the transpiler
def judge_tree(src, work, backend='cpp03', only=None, workers=2, log=print):
    """Every class of the Java tree `src` converted by the transpiler with the body back end `backend`
    and judged by the java lane (probe.py). -> {class: (verdict, gaps, detail)}"""
    from codebreaker.java import probe
    from codebreaker.verify import verdicts as V
    from codebreaker.verify.lanes import java as lane
    label, std = _STD[backend]
    saved = lane.CPP_STD, lane.STD, lane.FLAGS, probe.CXX_STD
    lane.CPP_STD, lane.STD, probe.CXX_STD = label, std, std
    lane.FLAGS = ('-std=' + std,) + tuple(f for f in saved[2] if not f.startswith('-std='))
    try:
        m = lane.prepare(src, work)
        log('[%s] converted: %d classes' % (backend, len(m.order)))
        context, items, _dropped = lane.build(m, src, work)
        names = [n for n in m.order if n in items and (only is None or n in only)]
        out, lock = {}, threading.Lock()
        todo = list(names)

        def worker():
            while True:
                with lock:
                    if not todo:
                        return
                    n = todo.pop(0)
                try:
                    verdict, reason, _limits, detail = lane.judge(context, items[n])
                except Exception as e:                       # a judge that fails says so, it is not alike
                    verdict, reason, detail = V.NOT_RUN, 'judge-error', str(e)[:300]
                gaps = m.texts.get(n, '').count('J2X-GAP:')
                with lock:
                    out[n] = (_word(verdict, gaps), gaps, (reason or '') + ': ' + (detail or '')[:400])
                    log('[%s] %s: %s (%d gaps)' % (backend, n, out[n][0], gaps))
        threads = [threading.Thread(target=worker) for _ in range(max(1, workers))]
        for t in threads:
            t.start()
        for t in threads:
            t.join()
        return out
    finally:
        lane.CPP_STD, lane.STD, lane.FLAGS, probe.CXX_STD = saved


def _word(verdict, gaps):
    from codebreaker.verify import verdicts as V
    if verdict == V.NO_COMPILE:
        return REFUSED
    if verdict == V.DIVERGENT:
        return DIVERGENT
    if verdict in V.GOOD:
        return EQUIVALENT
    return NOT_RUN


# ------------------------------------------------------------------------------ whole programs from L2
def java_tools():
    from codebreaker.verify.lanes import java as lane
    javac, java = lane.jdk()
    if not javac:
        raise RuntimeError('no JDK (settings.json: java)')
    return javac, java


def lower_program(src, work, release=None):
    """The Java sources under `src` through javac's tree into L2. -> [ir.Unit]"""
    from codebreaker.translate.frontends import java as fe
    from codebreaker.translate.frontends.java import lower
    units = fe.dump(src, os.path.join(work, 'tree'), release=release)
    errors = [d for d in fe.diagnostics(os.path.join(work, 'tree')) if d['kind'] == 'ERROR']
    if errors:
        raise RuntimeError('javac: %s' % errors[0]['message'][:300])
    return [lower.lower(t, p) for p, t in sorted(units.items())]


def _run(cmd, cwd=None, timeout=120, env=None):
    e = dict(os.environ)
    e.update(env or {})
    try:
        r = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout, env=e)
    except subprocess.TimeoutExpired:
        return -999, '', 'timed out'
    return r.returncode, r.stdout.decode('utf-8', 'replace'), r.stderr.decode('utf-8', 'replace')


# the probe harness: written in Java from L2, so the same cases run in the JVM and -- translated by the back
# end under judgement, as any other class of the program -- natively (TRANSLATOR_DESIGN.md, "the judge")
HARNESS = 'J2XProbe'
_FORMAT = {'int': 'fi', 'long': 'fi', 'short': 'fi', 'byte': 'fi', 'char': 'fc', 'boolean': 'fb', 'float': 'ff',
           'double': 'fd', 'String': 'fs'}
_JAVA_PRIM = {'i32': 'int', 'i64': 'long', 'i16': 'short', 'i8': 'byte', 'u16': 'char', 'bool': 'boolean',
              'f32': 'float', 'f64': 'double'}
_HELPERS = '''
  static void out(String k, String v) { System.out.println(k + "|" + v); }
  static String fi(long v) { return Long.toString(v); }
  static String fc(char v) { return Integer.toString((int) v); }
  static String fb(boolean v) { return v ? "1" : "0"; }
  static String ff(float v) { return Integer.toHexString(Float.floatToRawIntBits(v)); }
  static String fd(double v) { return Long.toHexString(Double.doubleToRawLongBits(v)); }
  static String fs(String v) {
    if (v == null) return "null";
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < v.length(); ++i) { int c = v.charAt(i); b.append(c < 256 ? String.format("%02x", c) : "u" + Integer.toHexString(c)); }
    return b.toString();
  }
'''


def _jname(t):
    if t is None:
        return None
    if t.kind in _JAVA_PRIM:
        return _JAVA_PRIM[t.kind]
    if t.kind == 'class' and t.name == 'java.lang.String':
        return 'String'
    return None


def probe_methods(units):
    """[(class reference in Java, method, [param type names], result type name)] -- every static method of a
    class of the default package, not private, whose parameters are primitives or Strings and whose result
    is one."""
    from codebreaker.translate import ir
    out = []

    def visit(c, ref, ok):
        if c.typeparams or c.kind in ('interface', 'annotation'):
            ok = False
        for m in c.members:
            if isinstance(m, ir.ClassDecl):
                visit(m, ref + '.' + m.name, ok and 'private' not in (m.mods or []))
            elif ok and isinstance(m, ir.MethodDecl) and m.kind == 'method' and m.sym is not None and m.sym.static \
                    and 'private' not in (m.mods or []) and m.name != 'main' and not m.typeparams:
                ps = [_jname(p.type) for p in m.params]
                r = _jname(m.ret)
                if r and all(ps):
                    out.append((ref, m.name, ps, r))
    package = _package(units)
    for u in units:
        if (u.package or '') != package:
            continue
        for c in u.types:
            if c.name != HARNESS:
                visit(c, c.name, True)
    return out


def _package(units):
    """The one package of the program's units ('' for the default package)."""
    pkgs = sorted(set(u.package or '' for u in units))
    return pkgs[0] if len(pkgs) == 1 else ''


def harness(methods, package=''):
    """The Java source of J2XProbe for `methods` (probe_methods), on probe.py's inputs. -> (source, keys)"""
    from codebreaker.java import probe
    lines = (['package %s;' % package] if package else []) + \
        ['public class %s {' % HARNESS, _HELPERS, '  public static void main(String[] a) {']
    keys = {}
    for mi, (ref, name, ps, r) in enumerate(methods):
        for ci, values in enumerate(probe.cases(ps)):
            key = '%d.%d' % (mi, ci)
            args = ', '.join(probe._java_literal(t, v) for t, v in zip(ps, values))
            lines.append('    try { out("%s", %s(%s.%s(%s))); } catch (Throwable t) { out("%s", "throw"); }'
                         % (key, _FORMAT[r], ref, name, args, key))
            keys[key] = (ref + '.' + name, values, r)
    lines += ['  }', '}', '']
    return '\n'.join(lines), keys


def _stage(src, dest):
    import shutil
    if os.path.isdir(dest):
        shutil.rmtree(dest)
    os.makedirs(dest)
    for dp, _dirs, files in os.walk(src):
        for f in files:
            if f.endswith('.java'):
                shutil.copy(os.path.join(dp, f), os.path.join(dest, f))
    return dest


def judge_program(src, work, backend='cpp17', log=print):
    """A whole Java program (a folder of sources, default package) beside the JVM, translated by a
    whole-program back end ('cpp17' or 'c'). -> dict(verdict, methods, cases, differs, main, why)"""
    from codebreaker.java import probe
    javac, java = java_tools()
    res = dict(verdict=NOT_RUN, methods=0, cases=0, differs=[], main=None, why='', backend=backend)
    stage = _stage(src, os.path.join(work, 'src'))
    units = lower_program(stage, os.path.join(work, 'probe-l2'))
    methods = probe_methods(units)
    package = _package(units)
    text, keys = harness(methods, package)
    with open(os.path.join(stage, HARNESS + '.java'), 'w', encoding='utf-8') as f:
        f.write(text)
    mains = [c.name for u in units for c in u.types
             if any(getattr(m, 'name', None) == 'main' and getattr(m, 'sym', None) is not None and m.sym.static
                    for m in c.members)]
    qual = (package + '.') if package else ''
    classes = os.path.join(work, 'classes')
    os.makedirs(classes, exist_ok=True)
    code, _o, err = _run([javac, '-nowarn', '-encoding', 'UTF-8', '-d', classes] +
                         [os.path.join(stage, f) for f in sorted(os.listdir(stage)) if f.endswith('.java')], timeout=300)
    if code != 0:
        res['why'] = 'javac: ' + err[:300]
        return res
    # the original, twice: a result that changes between two runs of Java itself says nothing
    runs = [probe._results(_run([java, '-cp', classes, qual + HARNESS])[1]) for _ in range(2)]
    jres = dict((k, v) for k, v in runs[0].items() if runs[1].get(k) == v and v != 'throw')
    jmain = dict((m, _run([java, '-cp', classes, qual + m])) for m in mains)
    # the translation
    try:
        units = lower_program(stage, os.path.join(work, 'l2'))
        exe, why = build(units, work, backend)
    except Exception as e:                                      # Decline, or a front end failure
        exe, why = None, '%s: %s' % (type(e).__name__, str(e)[:300])
    if exe is None:
        res['verdict'], res['why'] = REFUSED, why
        return res
    _c, out, _e = _run([exe], env={'JRT_MAIN': HARNESS})
    cres = probe._results(out)
    judged = set()
    for k, v in jres.items():
        if k not in keys:
            continue
        name, values, rt = keys[k]
        res['cases'] += 1
        judged.add(name)
        if k not in cres:
            res['differs'].append((name, values, v, 'no result (crashed?)'))
        elif not probe.same(rt, v, cres[k]):
            res['differs'].append((name, values, v, cres[k]))
    res['methods'] = len(judged)
    main_ok = True
    res['main'] = {}
    for m, (jc, jout, _je) in jmain.items():
        cc, cout, cerr = _run([exe], env={'JRT_MAIN': m})
        alike = jc == cc and jout.replace('\r\n', '\n') == cout.replace('\r\n', '\n')
        res['main'][m] = 'alike' if alike else 'differs: exit %s/%s\n--- java\n%s\n--- native\n%s\n%s' % (
            jc, cc, jout[-1500:], cout[-1500:], cerr[-500:])
        main_ok = main_ok and alike
    if res['differs'] or not main_ok:
        res['verdict'] = DIVERGENT
    elif res['cases'] or res['main']:
        res['verdict'] = EQUIVALENT
    else:
        res['why'] = 'nothing it could run and compare'
    return res


def build(units, work, backend):
    """Write a whole program with `backend` and compile it. -> (exe or None, why)"""
    import shutil
    out = os.path.join(work, backend)
    if os.path.isdir(out):
        shutil.rmtree(out)
    os.makedirs(out)
    if backend == 'cpp17':
        from codebreaker.translate.backends import cpp17
        text = cpp17.program(units)
        src = os.path.join(out, 'program.cpp')
        with open(src, 'w', encoding='utf-8') as f:
            f.write(text)
        cxx = _host_cxx()
        exe = os.path.join(out, 'program.exe')
        code, _o, err = _run([cxx] + list(cpp17.FLAGS) + ['-I', cpp17.RUNTIME, src, '-o', exe], timeout=600)
        if code != 0:
            first = next((l for l in err.splitlines() if 'error' in l), err[:300])
            return None, 'does not compile: ' + first[:400]
        return exe, ''
    if backend == 'c':
        from codebreaker.translate.backends import c_java
        return c_java.build(units, out)
    raise ValueError('no whole-program back end %r' % backend)


def _host_cxx():
    from codebreaker.verify.lanes import java as lane
    cxx = lane.compiler()
    if not cxx:
        raise RuntimeError('no C++ compiler (settings.json: host_cxx)')
    return cxx
