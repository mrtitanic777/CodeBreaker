"""probe.py -- run a Java class and its C++ translation on the same inputs and compare what they do.

    from codebreaker.java import probe
    result = probe.run(java_src_dir, cpp_dir, cls, work, cxx, javac, java)

The behavioural half of the java kind's judge (verify/lanes/java.py). For a class, every method it can call is
called on a fixed, deterministic set of inputs, in the JVM and natively, and what comes back compared:

  * what it can pass: primitives, Strings, and objects of classes it can make -- a class is made with
    its first non-private constructor, else a static factory of the class returning it, whose parameters
    are primitives or Strings (or the implicit constructor). The maker's arguments come from the same
    input set.
  * what it compares: a primitive or String result; an object result by its fields (the non-private,
    non-static primitive and String fields of the declared class and its ancestors); and for a void
    instance method, the object's fields after the call. Integers by value, floats and doubles by their
    bits (any NaN equals any NaN), Strings by their bytes. A null String in Java is the empty string
    here -- the port's model of a String is a value -- and they compare alike.
  * the original: the class's Java source compiled by javac (-sourcepath: it pulls in only what the
    harness needs) and run on the JVM -- twice: a method any of whose results changes between the two
    runs (a Random seeded from the clock, the time) is left out, it says nothing about the translation.
  * the translation: a generated harness linked against the whole emitted program (every .cpp compiled
    once per module, into an archive the linker takes what it needs from), run natively after the
    linked classes' static initialisers in the order j2x_init.cpp gives.

An input on which the Java side throws is not run on the C++ side (Java's exception is C++'s undefined
behaviour) and not counted. A class is judged on the methods it could run and says how many those were.
"""
import itertools
import os
import re
import struct
import subprocess
import threading

from codebreaker.java import twophase

REV = 11
TIMEOUT = 60
_STEP = 180             # seconds any one compile or link may take

# Java primitive -> (C++ type, inputs). Inputs are the same on both sides, written as literals.
_INPUTS = {
    'int': ('int', ['0', '1', '-1', '2', '7', '-13', '100', '255', '256', '1000', '-1000', '65535', '123456',
                    '2147483647', '-2147483648']),
    'long': ('long long', ['0L', '1L', '-1L', '1000000007L', '-123456789012L', '9223372036854775807L']),
    'short': ('short', ['0', '1', '-1', '255', '-32768', '32767']),
    'byte': ('signed char', ['0', '1', '-1', '100', '-128', '127']),
    'char': ('unsigned char', ['0', '32', '65', '97', '122', '167', '255']),
    'boolean': ('bool', ['true', 'false']),
    'float': ('float', ['0.0F', '1.0F', '-1.0F', '0.5F', '-0.5F', '3.14159F', '100.25F', '-1000.75F', '0.001F',
                        '1000000.0F', '-0.0F']),
    'double': ('double', ['0.0', '1.0', '-1.0', '0.5', '-0.5', '3.14159', '100.25', '-1000.75', '0.001',
                          '1000000.0', '-0.0', '12345.678']),
    'String': ('JavaString', ['', 'a', 'Hello World', '123', '-45', 'minecraft', ' pad ', 'A.B.C', 'x=1,y=2',
                              '§cRed']),
}
_MAX_CASES = 48
_CALL_RE = r'\bstatic\s+[\w<>* ]+\b%s\s*\('


# ---------------------------------------------------------------------------------------------- classes
_INDEX = {}


def class_index(java_dir):
    """{class name: .java path} of a staged source tree (cached per tree)."""
    if java_dir not in _INDEX:
        found = {}
        for dp, _dirs, files in os.walk(java_dir):
            for f in files:
                if f.endswith('.java'):
                    found.setdefault(f[:-5], os.path.join(dp, f))
        _INDEX[java_dir] = found
    return _INDEX[java_dir]


_PARSED = {}


def lookup(java_dir, name):
    """The parsed class `name` of the tree, or None."""
    from codebreaker.java import parse
    key = (java_dir, name)
    if key not in _PARSED:
        path = class_index(java_dir).get(name)
        try:
            _PARSED[key] = parse.parse_file(path) if path else None
        except Exception:
            _PARSED[key] = None
    return _PARSED[key]


def _simple(t):
    """A primitive or String type (not an array)."""
    return not t.is_array and t.name in _INPUTS


def maker(cls):
    """How an instance of `cls` is made: (None, [param types]) for a constructor, (factory, [param types])
    for a static factory of the class returning it, with primitive/String parameters; or None."""
    if cls is None or cls.is_abstract or cls.is_interface or cls.is_enum:
        return None
    ctors = [m for m in cls.methods if m.is_ctor]
    if not ctors:
        return None, []                                   # Java's implicit default constructor
    for m in ctors:
        if m.access != 'private' and all(_simple(p.jtype) for p in m.params):
            return None, [p.jtype.name for p in m.params]
    for m in cls.methods:
        rt = m.return_type
        if (m.is_static and not m.is_ctor and m.access != 'private' and rt is not None and not rt.is_array
                and rt.name == cls.name and all(_simple(p.jtype) for p in m.params)):
            return m.name, [p.jtype.name for p in m.params]
    return None


def fields(java_dir, cls, cpp_dir):
    """[(name, java type)] the class's state is compared by: its own and its ancestors' non-static,
    non-private primitive and String fields that the emitted header declares under that name."""
    out, seen, c = [], set(), cls
    while c is not None and c.name not in seen:
        seen.add(c.name)
        try:
            with open(os.path.join(cpp_dir, c.name + '.h'), encoding='utf-8', errors='replace') as f:
                header = f.read()
        except OSError:
            header = ''
        for f_ in c.fields:
            if (not f_.is_static and f_.access != 'private' and _simple(f_.jtype)
                    and re.search(r'\b%s\s*;' % re.escape(f_.name), header) and f_.name not in [n for n, _t in out]):
                out.append((f_.name, f_.jtype.name))
        c = lookup(java_dir, c.parent) if c.parent else None
    return out


# ------------------------------------------------------------------------------------ what can be run
def _slot(java_dir, t):
    """A parameter's input slot: a primitive/String type name, or ('obj', class, maker); None if it
    cannot be passed."""
    if _simple(t):
        return t.name
    if t.is_array:
        return None
    made = maker(lookup(java_dir, t.name))
    return ('obj', t.name, made) if made is not None else None


def _result(java_dir, cpp_dir, t):
    """How a result is compared: a type name, ('obj', class, fields), or None."""
    if t is None or t.is_array:
        return None
    if t.name in _INPUTS:
        return t.name
    cls = lookup(java_dir, t.name)
    fs = fields(java_dir, cls, cpp_dir) if cls is not None else []
    return ('obj', t.name, fs) if fs else None


_CLOCK = re.compile(r'\b(?:System\s*\.\s*(?:currentTimeMillis|nanoTime)|new\s+Date|[\w.]*getSystemClipboard)\s*\(')


def reads_clock(cls):
    """(methods whose result depends on the clock, whether the constructors read it): a method that reads
    System.currentTimeMillis/nanoTime (or the system clipboard -- what this machine holds, not what the
    code does: GuiScreen.getClipboardString), or calls one of the class's methods that does. Two runs of the
    original already leave out what changes between them; the clock also differs between the original's
    run and the C++ one, which no comparison can allow for (Timer.updateTimer)."""
    def text(m):
        return '\n'.join(t for _ln, t in (m.body or ()))
    bodies = dict((m.name, text(m)) for m in cls.methods if not m.is_ctor)
    clocked = set(n for n, t in bodies.items() if _CLOCK.search(t))
    grew = True
    while grew:
        grew = False
        for n, t in bodies.items():
            if n not in clocked and any(re.search(r'(?<![\w.])(?:this\s*\.\s*)?%s\s*\(' % re.escape(c), t)
                                        for c in clocked):
                clocked.add(n)
                grew = True
    ctor = '\n'.join(text(m) for m in cls.methods if m.is_ctor)
    ctor += '\n'.join(f.initializer for f in cls.instance_fields() if f.initializer)
    in_ctor = bool(_CLOCK.search(ctor)) or any(
        re.search(r'(?<![\w.])(?:this\s*\.\s*)?%s\s*\(' % re.escape(c), ctor) for c in clocked)
    return clocked, in_ctor


def probeable(cls, header_text, java_dir=None, cpp_dir=None):
    """[(method, [slots], result, made)] -- the methods it can call: present under their name in the
    emitted header, every parameter passable, the result comparable (a void instance method compares its
    object afterwards). made is None for a static method, else how the object is made (see `maker`)."""
    out, seen = [], set()
    made = maker(cls)
    clocked, ctor_clocked = reads_clock(cls)
    me = None
    if java_dir is not None and cpp_dir is not None:
        fs = fields(java_dir, cls, cpp_dir)
        me = ('obj', cls.name, fs) if fs else None
    for m in cls.methods:
        if m.is_ctor or m.is_abstract or m.access == 'private' or (not m.is_static and made is None):
            continue
        if m.name in clocked or (ctor_clocked and not m.is_static):    # depends on the time: not comparable
            continue
        rt = m.return_type
        if rt is not None and rt.name == 'void' and not rt.is_array:
            result = ('self',) if (not m.is_static and me is not None) else None
        elif java_dir is not None:
            result = _result(java_dir, cpp_dir, rt)
        else:
            result = rt.name if (rt is not None and _simple(rt)) else None
        if result is None:
            continue
        slots = [(_slot(java_dir, p.jtype) if java_dir is not None else (p.jtype.name if _simple(p.jtype) else None))
                 for p in m.params]
        if any(s is None for s in slots):
            continue
        key = (m.name, tuple(repr(p.jtype) for p in m.params))
        pattern = (_CALL_RE % re.escape(m.name)) if m.is_static else (r'\b%s\s*\(' % re.escape(m.name))
        if key in seen or not re.search(pattern, header_text):
            continue
        seen.add(key)
        if result == ('self',):
            result = ('self', me[1], me[2])                # compared by the object's fields afterwards
        out.append((m.name, slots, result, None if m.is_static else made))
    return out


def counted(cls):
    """How many methods a full judgement would have to run (all non-abstract methods and constructors)."""
    return len([m for m in cls.methods if not m.is_abstract])


# -------------------------------------------------------------------------------------------- inputs
def _flat(slots, made):
    """The primitive/String types a method's inputs are drawn for: the maker's, then each slot's."""
    out = list(made[1]) if made else []
    for s in slots:
        out += list(s[2][1]) if isinstance(s, tuple) else [s]
    return out


def cases(types):
    """The deterministic input tuples for a list of primitive/String types: every combination, or -- for
    several -- a fixed sample of at most _MAX_CASES of them, each input of each type used."""
    pools = [_INPUTS[t][1] for t in types]
    if not pools:
        return [()]
    total = 1
    for p in pools:
        total *= len(p)
    if total <= _MAX_CASES:
        return list(itertools.product(*pools))
    # too many to try them all (or even to list them): a fixed spread -- the k-th parameter steps through
    # its inputs at its own stride -- then every input of every parameter at least once
    picked = []
    for i in range(_MAX_CASES):
        c = tuple(p[(i * (2 * k + 1) + k) % len(p)] for k, p in enumerate(pools))
        if c not in picked:
            picked.append(c)
    for k, pool in enumerate(pools):
        for v in pool:
            if not any(c[k] == v for c in picked):
                picked.append(tuple(v if j == k else pools[j][0] for j in range(len(pools))))
    return picked


def _java_literal(t, v):
    if t == 'String':
        return '"%s"' % ''.join(c if 32 <= ord(c) < 127 and c not in '"\\' else '\\u%04x' % ord(c) for c in v)
    if t in ('byte', 'short', 'char'):
        return '(%s)%s' % (t, v)
    return v


def _cpp_literal(t, v):
    if t == 'String':
        return 'JavaString("%s")' % ''.join(c if 32 <= ord(c) < 127 and c not in '"\\' else '\\x%02x" "' % ord(c)
                                             for c in v)
    if t == 'long':
        return '(-9223372036854775807LL - 1)' if v.startswith('-9223') else v[:-1] + 'LL'
    if t in ('float', 'double') and v.startswith('-'):
        return '(%s)' % v
    if t == 'int' and v == '-2147483648':
        return '(-2147483647 - 1)'
    if t in ('byte', 'short', 'char'):
        return '(%s)%s' % (_INPUTS[t][0], v)
    return v


def _args(slots, made, values, lang, cls_name):
    """(make-expression or None, [argument expressions]) for one input tuple."""
    lit = _java_literal if lang == 'java' else _cpp_literal
    values = list(values)

    def take(types):
        got = values[:len(types)]
        del values[:len(types)]
        return ', '.join(lit(t, v) for t, v in zip(types, got))

    def make(name, how):
        a = take(how[1])
        if how[0] is None:
            return 'new %s(%s)' % (name, a)
        return ('%s.%s(%s)' if lang == 'java' else '%s::%s(%s)') % (name, how[0], a)

    obj = make(cls_name, made) if made is not None else None
    args = [make(s[1], s[2]) if isinstance(s, tuple) else take([s]) for s in slots]
    return obj, args


# ------------------------------------------------------------------------------------------- harness
_JAVA_HELPERS = r'''
  static void out(String k, String v) { System.out.println(k + "|" + v); }
  static String fi(long v) { return Long.toString(v); }
  static String fc(char v) { return Integer.toString((int)v); }
  static String fb(boolean v) { return v ? "1" : "0"; }
  static String ff(float v) { return Integer.toHexString(Float.floatToRawIntBits(v)); }
  static String fd(double v) { return Long.toHexString(Double.doubleToRawLongBits(v)); }
  static String fs(String v) {
    if (v == null) return "";
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < v.length(); ++i) { int c = v.charAt(i); b.append(c < 256 ? String.format("%02x", c) : "u" + Integer.toHexString(c)); }
    return b.toString();
  }
'''
_CPP_HELPERS = r'''#include <cstdio>
#include <cstring>
#include <string>
static void out(const char* k, const std::string& v) { std::printf("%s|%s\n", k, v.c_str()); }
static std::string fi(long long v) { char b[32]; std::sprintf(b, "%lld", v); return b; }
static std::string fc(unsigned char v) { char b[8]; std::sprintf(b, "%d", (int)v); return b; }
static std::string fb(bool v) { return v ? "1" : "0"; }
static std::string ff(float v) { unsigned int u; std::memcpy(&u, &v, 4); char b[16]; std::sprintf(b, "%x", u); return b; }
static std::string fd(double v) { unsigned long long u; std::memcpy(&u, &v, 8); char b[24]; std::sprintf(b, "%llx", u); return b; }
static std::string fs(const JavaString& v) {
	std::string r; char b[4];
	for (std::string::size_type i = 0; i < v.size(); ++i) { std::sprintf(b, "%02x", (unsigned char)v[i]); r += b; }
	return r;
}
'''
_FMT = {'int': 'fi', 'long': 'fi', 'short': 'fi', 'byte': 'fi', 'char': 'fc', 'boolean': 'fb',
        'float': 'ff', 'double': 'fd', 'String': 'fs'}


def _dumpers(methods, lang):
    """A dump_<Class> function per class whose objects are compared by fields."""
    done, out = set(), []
    for _n, _s, result, _m in methods:
        if isinstance(result, tuple) and result[1] not in done:
            done.add(result[1])
            name, fs = result[1], result[2]
            if lang == 'java':
                parts = ' + ","'.join(' + "%s:" + %s(o.%s)' % (f, _FMT[t], f) for f, t in fs)
                out.append('  static String dump_%s(%s o) { if (o == null) return "null"; return ""%s; }'
                           % (name, name, parts))
            else:
                parts = ' + ","'.join(' + "%s:" + %s(o->%s)' % (f, _FMT[t], f) for f, t in fs)
                out.append('static std::string dump_%s(%s* o) { if (o == NULL) return "null"; return std::string()%s; }'
                           % (name, name, parts))
    return out


def java_harness(cls_name, methods, package=None, name='J2XProbe', only=None):
    """The Java harness class `name`, in the class's own package (so package-private members can be
    reached). `only`: the case keys to run (all of them when None)."""
    lines = (['package %s;' % package] if package else []) + ['public class %s {' % name]
    lines += _JAVA_HELPERS.strip('\n').split('\n') + _dumpers(methods, 'java')
    lines.append('  public static void main(String[] a) {')
    for mi, (name, slots, result, made) in enumerate(methods):
        for ci, values in enumerate(cases(_flat(slots, made))):
            key = '%d.%d' % (mi, ci)
            if only is not None and key not in only:
                continue
            obj, args = _args(slots, made, values, 'java', cls_name)
            stmt = _statement(name, slots, result, made, obj, args, 'java', cls_name).replace('{key}', key)
            lines.append('    try { %s } catch (Throwable t) { out("%s", "throw"); }' % (stmt, key))
    lines += ['  }', '}', '']
    return '\n'.join(lines)


def cpp_harness(cls_name, methods, keep, init_order, extra_headers=(), ancestry=()):
    """The C++ side for the inputs in `keep` (the keys Java returned on). `init_order`: classes whose
    initStatics() run first, in order. `ancestry`: (class, base) pairs registered for instanceof first."""
    bases = [c for pair in ancestry for c in pair]
    heads = [cls_name] + [c for c in list(init_order) + sorted(extra_headers) + bases if c != cls_name]
    seen, inc = set(), []
    for h in heads:
        if h not in seen:
            seen.add(h)
            inc.append('#include "%s.h"' % h)
    lines = ['#include "stdafx.h"', '#include "JavaRT.h"'] + inc + _CPP_HELPERS.strip('\n').split('\n') + \
        _dumpers(methods, 'cpp') + ['int main() {']
    # FIXED BUG probe-no-ancestry: the game registers each class's bases at boot (j2x_rtti.cpp) so that
    # instanceof sees subclasses; the harness did not, and Beta's recipe setup crashed in every harness
    # that linked it (x instanceof Block was false for BlockOreStorage)
    lines += ['\tj2xRegisterBase(typeid(%s).name(), typeid(%s).name());' % pair for pair in ancestry]
    lines += ['\t%s::initStatics();' % c for c in init_order]
    for mi, (name, slots, result, made) in enumerate(methods):
        for ci, values in enumerate(cases(_flat(slots, made))):
            key = '%d.%d' % (mi, ci)
            if key in keep:
                obj, args = _args(slots, made, values, 'cpp', cls_name)
                lines.append('\t{ %s }' % _statement(name, slots, result, made, obj, args, 'cpp', cls_name)
                             .replace('{key}', key))
    lines += ['\treturn 0;', '}', '']
    return '\n'.join(lines)


def _statement(name, slots, result, made, obj, args, lang, cls_name):
    """The statement printing one call's outcome under {key}."""
    a = ', '.join(args)
    sep = '.' if lang == 'java' else '->'
    if made is None:
        target = '%s%s%s(%s)' % (cls_name, '.' if lang == 'java' else '::', name, a)
    else:
        target = '(%s)%s%s(%s)' % (obj, sep, name, a)
    if isinstance(result, tuple) and result[0] == 'self':
        var = ('%s o = %s;' if lang == 'java' else '%s* o = %s;') % (cls_name, obj)
        return '%s o%s%s(%s); out("{key}", dump_%s(o));' % (var, sep, name, a, cls_name)
    if isinstance(result, tuple):
        return 'out("{key}", dump_%s(%s));' % (result[1], target)
    return 'out("{key}", %s(%s));' % (_FMT[result], target)


def _classes_named(methods, cls_name):
    """Classes the harness names besides the class itself (made as arguments, compared as results)."""
    out = set()
    for _n, slots, result, _m in methods:
        out |= set(s[1] for s in slots if isinstance(s, tuple))
        if isinstance(result, tuple) and result[0] == 'obj':
            out.add(result[1])
    out.discard(cls_name)
    return out


# ------------------------------------------------------------------------------------------- compare
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


# ------------------------------------------------------------------------------------------ building
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
    _INDEX.pop(dest, None)
    return dest


_BASE = re.compile(r'j2xRegisterBase\(typeid\((\w+)\)\.name\(\), typeid\((\w+)\)\.name\(\)\);')


def ancestry(cpp_dir):
    """(class, base) for every class of the program, from j2x_rtti.cpp."""
    try:
        with open(os.path.join(cpp_dir, 'j2x_rtti.cpp'), encoding='utf-8', errors='replace') as f:
            return _BASE.findall(f.read())
    except OSError:
        return []


def init_order(cpp_dir):
    """Classes in the order j2x_init.cpp runs their static initialisers."""
    try:
        with open(os.path.join(cpp_dir, 'j2x_init.cpp'), encoding='utf-8', errors='replace') as f:
            return re.findall(r'^\s*(\w+)::initStatics\(\);', f.read(), re.M)
    except OSError:
        return []


def _compile(cxx, cpp_dir, unit, obj, prefix):
    """unit -> obj (written under a temporary name and moved into place). -> '' or the error."""
    if os.path.isfile(obj):
        return ''
    tmp = '%s.%d.tmp' % (obj, os.getpid())
    first = ['-include', prefix] if prefix else []
    r = subprocess.run([cxx, '-std=c++03', '-w', '-O0', '-iquote', cpp_dir] + first + ['-c', unit, '-o', tmp],
                       stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, timeout=_STEP)
    if r.returncode != 0:
        if os.path.exists(tmp):
            os.remove(tmp)
        return (r.stderr.decode('utf-8', 'replace').strip().splitlines() or ['compile failed'])[-1]
    os.replace(tmp, obj)
    return ''


def library(cxx, cpp_dir, work, prefix=None, jobs=None):
    """The whole emitted program as one archive, every .cpp compiled once (in parallel): the harnesses
    link against it and the linker takes what each needs. -> the archive path ('' if it cannot be made).
    A unit that does not compile is left out of it (its class is DOES NOT COMPILE anyway)."""
    from codebreaker.jobs import pool
    folder = os.path.join(work, 'probe', '_objects')
    lib = os.path.join(work, 'probe', 'libj2x.a')
    if os.path.isfile(lib):
        return lib
    os.makedirs(folder, exist_ok=True)
    units = sorted(f for f in os.listdir(cpp_dir) if f.endswith('.cpp'))
    if jobs is None:
        from codebreaker.base import parallel
        jobs = parallel.workers()

    def one(f):
        obj = os.path.join(folder, f[:-4] + '.o')
        try:
            return obj if not _compile(cxx, cpp_dir, os.path.join(cpp_dir, f), obj, prefix) else None
        except subprocess.TimeoutExpired:
            return None
    objects = [o for o in pool.threads(one, units, jobs=jobs) if o]
    tmp = lib + '.%d.tmp' % os.getpid()
    ar = os.path.join(os.path.dirname(cxx), 'ar' + ('.exe' if os.name == 'nt' else ''))
    ar = ar if os.path.isfile(ar) else 'ar'
    listing = tmp + '.rsp'                                 # the member list in a file: the command line
    with open(listing, 'w', encoding='utf-8') as f:        # would be too long for Windows
        f.write('\n'.join(os.path.basename(o) for o in objects))
    r = subprocess.run([ar, 'rcs', tmp, '@' + listing], cwd=folder, stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL)
    os.remove(listing)
    if r.returncode != 0:
        return ''
    os.replace(tmp, lib)
    return lib


_LOADED = re.compile(r'libj2x\.a\(([A-Za-z_]\w*)\.o\)')


def build_native(cxx, cpp_dir, harness, exe, lib, prefix=None):
    """Compile the harness and link it against the program's archive. -> (ok, why, [classes linked])"""
    try:
        main = os.path.splitext(harness)[0] + '.o'
        if os.path.exists(main):
            os.remove(main)
        err = _compile(cxx, cpp_dir, harness, main, prefix)
        if err:
            return False, 'the harness did not compile: ' + err[:160], []
        mapfile = exe + '.map'                              # the link map names every archive member taken
        r = subprocess.run([cxx, main, '-Wl,--start-group', lib, '-Wl,--end-group', '-Wl,-Map,' + mapfile,
                            '-o', exe], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=_STEP)
    except subprocess.TimeoutExpired:
        return False, 'a compile or link took over %d s' % _STEP, []
    if r.returncode != 0:
        e = r.stderr.decode('utf-8', 'replace')
        missing = sorted(set(re.findall(r"undefined reference to `([^']+)'", e)))
        if missing:                                         # the symbols, not ld's long path in front of them
            return False, 'undefined: ' + ', '.join(missing[:4])[:200], []
        return False, next((l for l in e.splitlines() if 'undefined' in l or 'error' in l), e[:200]).strip()[-200:], []
    try:
        with open(mapfile, encoding='utf-8', errors='replace') as f:
            linked = sorted(set(_LOADED.findall(f.read())))
    except OSError:
        linked = []
    return True, '', linked


# ------------------------------------------------------------------------------------------------ run
def _two_phase(cpp_dir, name):
    """True when the class is built in two phases (its header declares j2xInit; see twophase.py)."""
    try:
        with open(os.path.join(cpp_dir, name + '.h'), encoding='utf-8', errors='replace') as f:
            return ' j2xInit(' in f.read()
    except (IOError, OSError):
        return False


def harness_name(cls_name):
    return 'J2XProbe_' + cls_name


def libraries():
    """The jars the game is compiled and run against (settings.json `java_libs`: a folder of them --
    LWJGL, the sound system), sorted; [] when none is set. Class files only: a method that reaches
    native code (OpenGL, OpenAL) throws in Java and is left out like any other that throws."""
    from codebreaker.base import settings
    folder = settings.get('java_libs')
    if not folder or not os.path.isdir(folder):
        return []
    return sorted(os.path.join(folder, f) for f in os.listdir(folder) if f.lower().endswith('.jar'))


_SHIM_LOCK = threading.Lock()


def shims(work, javac):
    """Compile the stand-ins in probe_shims/ (classes the game was built against in a form no library
    download has -- Minecraft's own build of the sound codec) into work/probe/_shims, once. -> the folder,
    or '' when there are no libraries to compile them against or they did not compile."""
    out = os.path.join(work, 'probe', '_shims')
    done = os.path.join(out, '.compiled')
    libs = libraries()
    if not libs:
        return ''
    with _SHIM_LOCK:
        if not os.path.isfile(done):
            from codebreaker.java._paths import data_path
            root = data_path('probe_shims')
            files = [os.path.join(dp, f) for dp, _d, fs in os.walk(root) for f in fs if f.endswith('.java')]
            if not files:
                return ''
            os.makedirs(out, exist_ok=True)
            try:
                r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '-cp', os.pathsep.join(libs), '-d', out]
                                   + files, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=TIMEOUT * 3)
            except subprocess.TimeoutExpired:
                return ''
            if r.returncode != 0:
                return ''
            open(done, 'w').close()
    return out


def _cp(work, *dirs):
    """A classpath: these folders, the stand-ins (ahead of the library they stand in for), the libraries."""
    shim = os.path.join(work, 'probe', '_shims')
    return os.pathsep.join(list(dirs) + ([shim] if os.path.isfile(os.path.join(shim, '.compiled')) else [])
                           + libraries())


def compile_java(java_dir, todo, work, javac, size=40, jobs=None, tag='batch'):
    """Compile the Java harnesses of `todo` [(cls, methods)] -- in batches of `size`, one javac each
    (javac's start and the classes the harnesses share are paid once a batch, not once a class: it was
    three quarters of a class's time), the batches side by side. A batch javac rejects is split in half
    and tried again, so a class whose source cannot compile (LWJGL not on the classpath, say) fails alone.
    -> {class name: (classes folder, harness class to run) or ('', why)}"""
    from codebreaker.jobs import pool
    out = {}
    root = os.path.join(work, 'probe', '_java')
    shims(work, javac)

    def attempt(group, label):
        jdir = os.path.join(root, label)
        files = []
        for cls, methods in group:
            pkg = cls.package
            folder = os.path.join(jdir, *(pkg.split('.') if pkg else []))
            os.makedirs(folder, exist_ok=True)
            path = os.path.join(folder, harness_name(cls.name) + '.java')
            with open(path, 'w', encoding='utf-8') as f:
                f.write(java_harness(cls.name, methods, pkg, harness_name(cls.name)))
            files.append(path)
        listing = os.path.join(jdir, 'sources.txt')        # an @file: the command line stays short
        with open(listing, 'w', encoding='utf-8') as f:
            f.write('\n'.join('"%s"' % x.replace('\\', '/') for x in files))
        try:
            r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8'] + (['-cp', _cp(work)] if libraries() else []) +
                               ['-sourcepath', java_dir, '-d', jdir,
                                '@' + listing], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               timeout=TIMEOUT * 3 + 10 * len(group))
        except subprocess.TimeoutExpired:
            return False, 'javac did not finish'
        if r.returncode != 0:
            return False, 'javac: ' + next((l for l in r.stdout.decode('utf-8', 'replace').splitlines()
                                             if 'error' in l), 'failed')[:180]
        for cls, _m in group:
            out[cls.name] = (jdir, ((cls.package + '.') if cls.package else '') + harness_name(cls.name))
        return True, ''

    def settle(group, label):
        ok, why = attempt(group, label)
        if ok:
            return
        if len(group) == 1:
            out[group[0][0].name] = ('', why)
            return
        half = len(group) // 2
        settle(group[:half], label + 'a')
        settle(group[half:], label + 'b')

    batches = [todo[i:i + size] for i in range(0, len(todo), size)]
    if jobs is None:
        from codebreaker.base import parallel
        jobs = parallel.workers()
    pool.threads(lambda nb: settle(nb[1], '%s%d' % (tag, nb[0])), enumerate(batches), jobs=min(jobs, len(batches) or 1))
    return out


def _java_again(java_dir, cls, methods, work, javac, java, jdir, only):
    """Run the original once more on just the cases in `only`, in their order. -> {key: result}, or None
    when it could not be compiled or run."""
    kdir = os.path.join(work, 'probe', cls.name, '_java_only')
    pkg = cls.package
    folder = os.path.join(kdir, *(pkg.split('.') if pkg else []))
    os.makedirs(folder, exist_ok=True)
    name = harness_name(cls.name) + 'Only'
    path = os.path.join(folder, name + '.java')
    with open(path, 'w', encoding='utf-8') as f:
        f.write(java_harness(cls.name, methods, pkg, name, only))
    cp = _cp(work, kdir, jdir)
    try:
        r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '-cp', _cp(work, jdir), '-sourcepath', java_dir, '-d', kdir,
                            path], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=TIMEOUT * 3)
        if r.returncode != 0:
            return None
        r = subprocess.run([java, '-cp', cp, ((pkg + '.') if pkg else '') + name],
                           stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        return None
    return _results(r.stdout.decode('utf-8', 'replace'))


def plan(java_dir, cpp_dir, names):
    """[(cls, methods)] of the named classes that have something to run (see `probeable`)."""
    out = []
    for name in names:
        cls = lookup(java_dir, name)
        if cls is None:
            continue
        try:
            with open(os.path.join(cpp_dir, name + '.h'), encoding='utf-8', errors='replace') as f:
                methods = probeable(cls, f.read(), java_dir, cpp_dir)
        except OSError:
            continue
        if methods:
            out.append((cls, methods))
    return out


def run(java_dir, cpp_dir, cls, work, cxx, javac, java, prefix=None, compiled=None):
    """`java_dir`: the module's sources laid out by package (stage_sources); `compiled`: compile_java's
    answer for the module, when the harnesses were compiled in batches beforehand. -> dict(methods=[...],
    judged=n, total=n, cases=n, differs=[(method, args, java, cpp)], why='', unsteady=n)."""
    header = os.path.join(cpp_dir, cls.name + '.h')
    try:
        with open(header, encoding='utf-8', errors='replace') as f:
            methods = probeable(cls, f.read(), java_dir, cpp_dir)
    except OSError:
        methods = []
    res = dict(methods=[m[0] for m in methods], judged=0, total=counted(cls), cases=0, differs=[], why='',
               unsteady=0)
    if not methods:
        res['why'] = ('it reads the clock or the clipboard (they differ between the two runs)' if any(reads_clock(cls)) else
                      'no method it can call and compare (its arguments made, its result or object read back)')
        return res
    folder = os.path.join(work, 'probe', cls.name)
    os.makedirs(folder, exist_ok=True)
    got = (compiled or {}).get(cls.name)
    if got is None:                                       # not compiled with the module's batch: alone
        got = compile_java(java_dir, [(cls, methods)], work, javac, tag=cls.name).get(cls.name)
    jdir, main = got
    if not jdir:
        res['why'] = main
        return res
    # the original runs twice: a method any of whose results changes between two runs of Java itself
    # (a Random seeded from the clock, the time, an identity hash) is left out whole
    runs = []
    for _twice in range(2):
        try:
            r = subprocess.run([java, '-cp', _cp(work, jdir), main],
                               stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, timeout=TIMEOUT)
        except subprocess.TimeoutExpired:
            res['why'] = 'the Java side did not finish'
            return res
        runs.append(_results(r.stdout.decode('utf-8', 'replace')))
    jres = runs[0]
    shaky = set(k.split('.')[0] for k in jres if runs[1].get(k) != jres[k])
    unsteady = set(k for k in jres if k.split('.')[0] in shaky)
    res['unsteady'] = len(unsteady)
    keep = set(k for k, v in jres.items() if v != 'throw' and k not in unsteady)
    if not keep:
        res['why'] = ('Java gives a different result on every run' if unsteady else
                      'the Java methods threw on every input')
        return res
    # The cases run one after another in one program, and a case can leave state behind for the next ones
    # (an Item's constructor registers it in Item.itemsList, even when the rest of that call throws). The C++
    # side runs only the cases kept, so when some were left out the original is run again on exactly
    # those, in the same order, and that is what C++ is compared with; repeated until no kept case throws.
    settled = keep == set(jres)
    for _again in range(4):
        if settled:
            break
        sub = _java_again(java_dir, cls, methods, work, javac, java, jdir, keep)
        if sub is None:
            res['why'] = 'the original could not be run again on the cases kept'
            return res
        now = set(k for k in keep if sub.get(k) not in (None, 'throw'))
        for k in now:
            jres[k] = sub[k]
        settled = now == keep
        keep = now
        if not keep:
            break
    if not keep or not settled:
        res['why'] = 'the cases depend on what the ones left out did (no steady set to compare)'
        return res
    lib = library(cxx, cpp_dir, work, prefix)
    if not lib:
        res['why'] = 'the program could not be archived for linking'
        return res
    order = init_order(cpp_dir)
    harness = os.path.join(folder, 'probe.cpp')
    exe = os.path.join(folder, 'probe.exe')
    named = _classes_named(methods, cls.name)
    need = [c for c in order if c == cls.name or c in named]
    made_in_two = [c for c in {cls.name} | set(named) if _two_phase(cpp_dir, c)]
    edges = ancestry(cpp_dir)
    present = {cls.name} | set(named)
    for _pass in range(4):
        pairs = [(d, b) for d, b in edges if d in present and b in present]
        with open(harness, 'w', encoding='utf-8') as f:
            f.write(twophase.rewrite_news(cpp_harness(cls.name, methods, keep, need, named, pairs), made_in_two)[0])
        ok, why, linked = build_native(cxx, cpp_dir, harness, exe, lib, prefix)
        # the classes linked in may have static initialisers of their own, and bases to register: all of
        # theirs, in order
        now = set(linked) | {cls.name} | set(named)
        wanted = [c for c in order if c in now]
        if not ok or (wanted == need and now == present):
            break
        need, present = wanted, now
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
    for mi, (name, slots, result, made) in enumerate(methods):
        for ci, values in enumerate(cases(_flat(slots, made))):
            key = '%d.%d' % (mi, ci)
            if key not in keep:
                continue
            res['cases'] += 1
            judged.add(mi)
            kind = result if isinstance(result, str) else 'obj'
            if key not in cres:
                res['differs'].append((name, values, jres[key], 'no result (crashed?)'))
            elif not same(kind, jres[key], cres[key]):
                res['differs'].append((name, values, jres[key], cres[key]))
    res['judged'] = len(judged)
    return res
