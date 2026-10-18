"""probe.py -- run a Java class and its C++ translation on the same inputs and compare what they return.

    from codebreaker.java import probe
    result = probe.run(java_src_dir, cpp_dir, "MathHelper", work, cxx, javac, java)

The behavioural half of the java kind's judge (verify/java.py). For a class, every static method whose
parameters and result are primitives is called on a fixed, deterministic set of inputs:

  * the original: the class's Java source compiled by javac (-sourcepath: it pulls in only what the
    class needs from the folder) and run on the JVM, from a generated J2XProbe.java;
  * the translation: the class's emitted .cpp linked with a generated harness (and, when the linker
    asks, the .cpp of the classes it names), run natively after the classes' static initialisers in the
    order j2x_init.cpp gives.

Results are compared exactly: integers by value, floats and doubles by their bits (any NaN equals any
NaN). An input on which the Java method throws is not run on the C++ side (Java's exception is C++'s
undefined behaviour) and not counted. A class whose methods could not all be run -- not static, a
parameter or result that is not a primitive, an input set Java threw on throughout -- is judged on
the ones that could, and says how many were.
"""
import itertools
import os
import re
import struct
import subprocess

REV = 1
TIMEOUT = 60

# Java primitive -> (C++ parameter type, inputs). Inputs are the same on both sides, written as literals.
_INPUTS = {
    'int': ('int', ['0', '1', '-1', '2', '7', '-13', '100', '255', '256', '1000', '-1000', '65535', '123456',
                    '2147483647', '-2147483648']),
    'long': ('long long', ['0L', '1L', '-1L', '1000000007L', '-123456789012L', '9223372036854775807L']),
    'short': ('short', ['0', '1', '-1', '255', '-32768', '32767']),
    'byte': ('signed char', ['0', '1', '-1', '100', '-128', '127']),
    'char': ('char', ['0', '32', '65', '97', '122']),
    'boolean': ('bool', ['true', 'false']),
    'float': ('float', ['0.0F', '1.0F', '-1.0F', '0.5F', '-0.5F', '3.14159F', '100.25F', '-1000.75F', '0.001F',
                        '1000000.0F', '-0.0F']),
    'double': ('double', ['0.0', '1.0', '-1.0', '0.5', '-0.5', '3.14159', '100.25', '-1000.75', '0.001',
                          '1000000.0', '-0.0', '12345.678']),
}
_MAX_CASES = 48
_CALL_RE = r'\bstatic\s+[\w<>* ]+\b%s\s*\('


def cases(param_types):
    """The deterministic input tuples for a parameter list: every combination, or -- for several
    parameters -- a fixed sample of at most _MAX_CASES of them, each input of each parameter used."""
    pools = [_INPUTS[t][1] for t in param_types]
    if not pools:
        return [()]
    every = list(itertools.product(*pools))
    if len(every) <= _MAX_CASES:
        return every
    step = len(every) // _MAX_CASES + 1
    picked = every[::step]
    for k, pool in enumerate(pools):                     # make sure each value of each parameter appears
        for v in pool:
            if not any(c[k] == v for c in picked):
                picked.append(tuple(v if j == k else pools[j][0] for j in range(len(pools))))
    return picked


def probeable(cls, header_text):
    """[(method, [param java types], return java type)] -- static, primitive in and out, present as a
    static member of that name in the emitted header (renamed or dropped methods are left out)."""
    out, seen = [], set()
    for m in cls.methods:
        if m.is_ctor or not m.is_static or m.is_abstract or m.access == 'private':
            continue
        rt = m.return_type
        if rt is None or rt.is_array or rt.name not in _INPUTS:
            continue
        ps = [p.jtype for p in m.params]
        if any(p.is_array or p.name not in _INPUTS for p in ps):
            continue
        key = (m.name, tuple(p.name for p in ps))
        if key in seen or not re.search(_CALL_RE % re.escape(m.name), header_text):
            continue
        seen.add(key)
        out.append((m.name, [p.name for p in ps], rt.name))
    return out


def counted(cls):
    """How many methods a full judgement would have to run (all non-abstract methods and constructors)."""
    return len([m for m in cls.methods if not m.is_abstract])


def _java_literal(t, v):
    if t in ('byte', 'short', 'char'):
        return '(%s)%s' % (t, v)
    return v


def _cpp_literal(t, v):
    if t == 'long':
        return v[:-1] + 'LL'
    if t in ('float', 'double') and v.startswith('-'):
        return '(%s)' % v
    if t in ('int',) and v == '-2147483648':
        return '(-2147483647 - 1)'
    if t == 'long' and v.startswith('-9223'):
        return '(-9223372036854775807LL - 1)'
    if t in ('byte', 'short', 'char'):
        return '(%s)%s' % (_INPUTS[t][0], v)
    return v


_JAVA_OUT = {
    'int': 'Integer.toString(%s)', 'long': 'Long.toString(%s)', 'short': 'Integer.toString(%s)',
    'byte': 'Integer.toString(%s)', 'char': 'Integer.toString((int)%s)', 'boolean': '(%s ? "1" : "0")',
    'float': 'Integer.toHexString(Float.floatToRawIntBits(%s))',
    'double': 'Long.toHexString(Double.doubleToRawLongBits(%s))',
}
_CPP_OUT = {'int': 'pi', 'long': 'pi', 'short': 'pi', 'byte': 'pi', 'char': 'pc', 'boolean': 'pb',
            'float': 'pf', 'double': 'pd'}


def stage_sources(java_dir, dest):
    """The module's .java files laid out by package (javac's -sourcepath needs net/minecraft/src/X.java;
    a decompiled tree is often flat). -> dest"""
    from codebreaker.java import transpile
    for path in transpile.discover(java_dir):
        with open(path, encoding='utf-8', errors='replace') as f:
            text = f.read()
        m = re.search(r'^\s*package\s+([\w.]+)\s*;', text, re.M)
        folder = os.path.join(dest, *(m.group(1).split('.') if m else []))
        os.makedirs(folder, exist_ok=True)
        with open(os.path.join(folder, os.path.basename(path)), 'w', encoding='utf-8') as f:
            f.write(text)
    return dest


def java_harness(cls_name, methods, package=None):
    """J2XProbe, in the class's own package (so package-private methods can be called)."""
    lines = (['package %s;' % package] if package else []) + ['public class J2XProbe {',
             '  static void out(String k, String v) { System.out.println(k + "|" + v); }',
             '  public static void main(String[] a) {']
    target = cls_name
    for mi, (name, ps, rt) in enumerate(methods):
        for ci, args in enumerate(cases(ps)):
            call = '%s.%s(%s)' % (target, name, ', '.join(_java_literal(t, v) for t, v in zip(ps, args)))
            key = '%d.%d' % (mi, ci)
            lines.append('    try { out("%s", %s); } catch (Throwable t) { out("%s", "throw"); }'
                         % (key, _JAVA_OUT[rt] % call, key))
    lines += ['  }', '}', '']
    return '\n'.join(lines)


def cpp_harness(cls_name, methods, keep, init_order):
    """The C++ side for the inputs in `keep` (the keys Java returned on). `init_order`: classes whose
    initStatics() run first, in order."""
    lines = ['#include "stdafx.h"', '#include "JavaRT.h"', '#include "%s.h"' % cls_name] + \
            ['#include "%s.h"' % c for c in init_order if c != cls_name] + [
        '#include <cstdio>', '#include <cstring>',
        'static void pi(const char* k, long long v) { std::printf("%s|%lld\\n", k, v); }',
        'static void pc(const char* k, char v) { std::printf("%s|%d\\n", k, (int)(unsigned char)v); }',
        'static void pb(const char* k, bool v) { std::printf("%s|%d\\n", k, v ? 1 : 0); }',
        'static void pf(const char* k, float v) { unsigned int u; std::memcpy(&u, &v, 4); std::printf("%s|%x\\n", k, u); }',
        'static void pd(const char* k, double v) { unsigned long long u; std::memcpy(&u, &v, 8); '
        'std::printf("%s|%llx\\n", k, u); }',
        'int main() {']
    lines += ['\t%s::initStatics();' % c for c in init_order]
    for mi, (name, ps, rt) in enumerate(methods):
        for ci, args in enumerate(cases(ps)):
            key = '%d.%d' % (mi, ci)
            if key in keep:
                lines.append('\t%s("%s", %s::%s(%s));' % (_CPP_OUT[rt], key, cls_name, name,
                                                         ', '.join(_cpp_literal(t, v) for t, v in zip(ps, args))))
    lines += ['\treturn 0;', '}', '']
    return '\n'.join(lines)


def _results(text):
    out = {}
    for line in text.splitlines():
        k, _, v = line.strip().partition('|')
        if k:
            out[k] = v
    return out


def same(rt, a, b):
    """Do two printed results agree? Floats and doubles by bits, any NaN equal to any NaN."""
    if a == b:
        return True
    if rt in ('float', 'double'):
        try:
            fa, fb = (struct.unpack('<f', struct.pack('<I', int(x, 16)))[0] if rt == 'float' else
                      struct.unpack('<d', struct.pack('<Q', int(x, 16)))[0] for x in (a, b))
        except (ValueError, struct.error):
            return False
        return fa != fa and fb != fb
    return False


def init_order(cpp_dir):
    """Classes in the order j2x_init.cpp runs their static initialisers."""
    try:
        with open(os.path.join(cpp_dir, 'j2x_init.cpp'), encoding='utf-8', errors='replace') as f:
            return re.findall(r'^\s*(\w+)::initStatics\(\);', f.read(), re.M)
    except OSError:
        return []


_UNDEF = re.compile(r"undefined reference to `(?:vtable for |typeinfo for )?([A-Za-z_]\w*)::")


def build_native(cxx, cpp_dir, harness, exe, first):
    """Link the harness with `first` (.cpp files), adding the .cpp of any class the linker reports
    missing, a few rounds. -> (ok, why, [the .cpp files linked])"""
    units = list(first)
    for _round in range(8):
        r = subprocess.run([cxx, '-std=c++03', '-w', '-O0', '-iquote', cpp_dir, harness] + units + ['-o', exe],
                           stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        if r.returncode == 0:
            return True, '', units
        err = r.stderr.decode('utf-8', 'replace')
        more = sorted(set(os.path.join(cpp_dir, c + '.cpp') for c in _UNDEF.findall(err)
                          if os.path.isfile(os.path.join(cpp_dir, c + '.cpp'))) - set(units))
        if not more:
            first_err = next((l for l in err.splitlines() if 'error' in l or 'undefined' in l), err[:200])
            return False, first_err.strip()[:200], units
        units += more
    return False, 'link: too many classes pulled in', units


def run(java_dir, cpp_dir, cls, work, cxx, javac, java):
    """`java_dir`: the module's sources laid out by package (stage_sources). -> dict(methods=[...], judged=n, total=n, cases=n, differs=[(method, args, java, cpp)], why='')."""
    header = os.path.join(cpp_dir, cls.name + '.h')
    try:
        with open(header, encoding='utf-8', errors='replace') as f:
            methods = probeable(cls, f.read())
    except OSError:
        methods = []
    res = dict(methods=[m[0] for m in methods], judged=0, total=counted(cls), cases=0, differs=[], why='')
    if not methods:
        res['why'] = 'no static method with primitive parameters and result'
        return res
    folder = os.path.join(work, 'probe', cls.name)
    jdir = os.path.join(folder, 'java')
    pkg = cls.package
    hdir = os.path.join(jdir, *(pkg.split('.') if pkg else []))
    os.makedirs(hdir, exist_ok=True)
    with open(os.path.join(hdir, 'J2XProbe.java'), 'w', encoding='utf-8') as f:
        f.write(java_harness(cls.name, methods, pkg))
    r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '-sourcepath', java_dir, '-d', jdir,
                        os.path.join(hdir, 'J2XProbe.java')], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                       timeout=TIMEOUT * 3)
    if r.returncode != 0:
        res['why'] = 'javac: ' + next((l for l in r.stdout.decode('utf-8', 'replace').splitlines() if 'error' in l),
                                      'failed')[:180]
        return res
    try:
        r = subprocess.run([java, '-cp', jdir, (pkg + '.J2XProbe') if pkg else 'J2XProbe'], stdout=subprocess.PIPE,
                           stderr=subprocess.DEVNULL,
                           timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        res['why'] = 'the Java side did not finish'
        return res
    jres = _results(r.stdout.decode('utf-8', 'replace'))
    keep = set(k for k, v in jres.items() if v != 'throw')
    if not keep:
        res['why'] = 'the Java methods threw on every input'
        return res
    order = init_order(cpp_dir)
    harness = os.path.join(folder, 'probe.cpp')
    exe = os.path.join(folder, 'probe.exe')
    need = [cls.name] if cls.name in order else []
    for _pass in range(2):
        with open(harness, 'w', encoding='utf-8') as f:
            f.write(cpp_harness(cls.name, methods, keep, need))
        ok, why, units = build_native(cxx, cpp_dir, harness, exe, [os.path.join(cpp_dir, cls.name + '.cpp')])
        # the classes linked in may have static initialisers of their own: run all of theirs, in order
        linked = set(os.path.splitext(os.path.basename(u))[0] for u in units)
        wanted = [c for c in order if c in linked]
        if not ok or wanted == need:
            break
        need = wanted
    if not ok:
        res['why'] = 'did not link: ' + why
        return res
    try:
        r = subprocess.run([exe], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        res['why'] = 'the C++ side did not finish'
        return res
    cres = _results(r.stdout.decode('utf-8', 'replace'))
    judged = set()
    for mi, (name, ps, rt) in enumerate(methods):
        for ci, args in enumerate(cases(ps)):
            key = '%d.%d' % (mi, ci)
            if key not in keep:
                continue
            res['cases'] += 1
            judged.add(mi)
            if key not in cres:
                res['differs'].append((name, args, jres[key], 'no result (crashed?)'))
            elif not same(rt, jres[key], cres[key]):
                res['differs'].append((name, args, jres[key], cres[key]))
    res['judged'] = len(judged)
    return res
