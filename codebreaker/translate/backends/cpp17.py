"""cpp17.py -- the translator's C++17 back end (docs/TRANSLATOR_DESIGN.md, phase 4).

Two ways in, one set of idiom rules:

  * Bodies (inside the transpiler, codebreaker/java): the transpiler writes the declarations and its
    runtime (platform/canonical), this writes every body -- cpp03's Writer, with the C++17 forms where the
    language has them: a for-each as a range-for over a container, a switch expression as an immediately
    invoked lambda, `auto` where the initialiser spells the type, nullptr. Whatever has no idiomatic form
    that is provably the same falls back to cpp03's faithful form (it is cpp03's code). This is the path
    Minecraft takes: `transpile.convert(..., cpp_std='C++17')`.

  * Program (a whole Java program from L2, declarations and bodies, on the runtime
    codebreaker/translate/runtime/cpp17/jrt.hpp): what the C++17 back end writes for any program whose
    library use the runtime covers. Its choices, each the idiomatic form where it is provably the same as
    Java and the faithful form elsewhere:
      - references are jrt::ref<T> (std::shared_ptr): reference counting in place of the collector. A class
        whose fields can lead back to itself can form a cycle that is never freed; `Program.cycles` lists
        them -- the place a collector (Boehm) would be needed;
      - a record (final, its constructor reaches no override) and any class nothing extends whose
        constructors never hand `this` out gets real C++ constructors (`std::make_shared<Point>(x, y)`);
        every other class is made in two phases as the JVM makes it (jrt::make: fields zero, then ctor_),
        so a virtual call in a constructor reaches the override;
      - lambdas and method references are C++ lambdas held by the functional interface (jrt::fn<I>), their
        captures copied ([=]: Java captures only effectively-final locals) and `this` held by a strong
        reference (self_), as the Java lambda holds it;
      - for-each is a range-for (over the array's elements, or the Iterable's iterator via jrt::each<T>);
      - a switch expression is an immediately invoked lambda, each `yield` its return;
      - static fields are `static inline` members; a class with a static initialiser runs it on first use,
        as Java does (clinit_ at each static method, constructor and outside static-field access);
      - generics are erased, as Java erases them; a value read through a type variable is cast back to the
        type javac gave it (jrt::cast, which throws ClassCastException where Java's checkcast would);
      - Java's integer semantics: wrapping arithmetic (compiled with -fwrapv), shift counts masked, `/` and
        `%` by zero throw ArithmeticException, (int) of a double saturates;
      - Java's left-to-right evaluation order where C++'s is unspecified and the order matters (cpp03's rule:
        the operands into temporaries, in Java's order);
      - try/finally: a finally no jump leaves runs from a scope guard (jrt::Finally) when the try is left by
        a jump; otherwise the finally block is written twice, on the normal and the exceptional way out;
        try-with-resources closes through the guard, a close failing during an exception suppressed as Java
        suppresses it.
    It declines (Decline) what it cannot write; the judge (codebreaker/translate/javajudge.py) reports a
    declined or non-compiling program as refused.
"""
import os
import re

from codebreaker.translate import desugar, ir
from codebreaker.translate.backends import cpp03
from codebreaker.translate.backends.cpp03 import Decline

RUNTIME = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'runtime', 'cpp17')
FLAGS = ('-std=c++17', '-fwrapv', '-w', '-O1')


# ============================================================================================ transpiler mode
_Base = cpp03.Writer                                 # (the module attribute is swapped while Bodies runs)


class Writer(_Base):
    """cpp03's Writer with the C++17 forms: range-for, switch expressions, auto."""

    def _stmt(self, s, out, ind):
        if isinstance(s, ir.ForEach):
            return self.foreach(s, out, ind)
        if isinstance(s, ir.Local) and s.init is not None and self.spelled(s):
            return self.auto_local(s, out, ind)
        return _Base._stmt(self, s, out, ind)

    def spelled(self, s):
        """The initialiser names the local's own type: `new T(..)` of the same T, or a cast to it."""
        init = s.init
        while isinstance(init, ir.Paren):
            init = init.x
        if not isinstance(init, (ir.New, ir.Cast)) or s.type is None or init.type is None:
            return False
        return init.type == s.type and s.type.kind == 'class' and not (isinstance(init, ir.New) and init.body is not None)

    def auto_local(self, s, out, ind):
        """`auto x = new T(...)` / `auto x = (T*)y`: the type is written once, where it is made."""
        lines = []
        _Base._stmt(self, s, lines, ind)
        ct = self.vars.get(self.name(s.sym.name), '')
        if len(lines) == 1 and ct and lines[0].startswith(ind + ct + ' '):
            star = '*' if cpp03.is_pointer(ct) else ''
            if star or ct.startswith(('JavaString', 'JList', 'JMap', 'JSet')):
                lines[0] = ind + 'auto%s ' % star + lines[0][len(ind + ct) + 1:]
        out.extend(lines)

    def foreach(self, s, out, ind):
        """for (T x : xs): a range-for over a JList (a std::vector, iterated in order as Java's Iterator
        does); over an array, the counted loop (JArray has no iterators)."""
        coll, ct = self.expr(s.iterable)
        name = self.name(s.var.sym.name)
        vt = self.ctype(s.var.type)
        if name in self.vars:
            raise Decline('shadowed loop variable %s' % name)
        if ct.startswith('JList<') or ct.startswith('const JList<'):
            self.vars[name] = vt
            out.append(ind + 'for (%s %s : %s) {' % (vt, name, coll))
            self.loop_body(s.body, out, ind)
            out.append(ind + '}')
            self.vars.pop(name, None)
            return
        if ct.startswith('JArray<'):
            self.temps += 1
            arr, idx = 'j2x_a%d' % self.temps, 'j2x_i%d' % self.temps
            out.append(ind + '{')
            out.append(ind + '\t%s %s = %s;' % (ct, arr, coll))
            out.append(ind + '\tfor (int %s = 0; %s < %s.length; %s++) {' % (idx, idx, arr, idx))
            self.vars[name] = vt
            out.append(ind + '\t\t%s %s = %s[%s];' % (vt, name, arr, idx))
            self.loop_body(s.body, out, ind + '\t')
            out.append(ind + '\t}')
            out.append(ind + '}')
            self.vars.pop(name, None)
            return
        raise Decline('for-each over %s' % ct)

    def x_SwitchExpr(self, e):
        """A switch expression as an immediately invoked lambda: each `yield v` is `return v`."""
        t = self.ctype(e.type)
        x, xt = self.expr(e.expr.x if isinstance(e.expr, ir.Paren) else e.expr)
        if 'JavaString' in xt:
            raise Decline('switch on a string')
        lines = ['[&]() -> %s {' % t, '\tswitch(%s) {' % x]
        saved_ret, self.ret = self.ret, t
        saved_two, self.two_phase = self.two_phase, False
        try:
            for c in e.cases:
                for lab in c.labels:
                    lines.append('\tcase %s:' % self.expr(lab)[0])
                if c.default:
                    lines.append('\tdefault:')
                body = [ir.Return(expr=b.expr, line=b.line, col=b.col) if isinstance(b, ir.Yield) else b for b in c.body]
                for b in ir.walk(ir.Block(stmts=body)):
                    if isinstance(b, ir.Yield):
                        raise Decline('a yield inside a nested statement')
                lines.append('\t{')
                for b in body:
                    self.stmt(b, lines, '\t\t')
                lines.append('\t}')
        finally:
            self.ret, self.two_phase = saved_ret, saved_two
        lines += ['\t}', '\tthrow 0;', '}()']
        return '\n'.join(lines), t


def _with_writer(fn):
    """Run one of cpp03's module-level helpers with this module's Writer in place of cpp03's."""
    def run(*a, **k):
        saved = cpp03.Writer
        cpp03.Writer = Writer
        try:
            return fn(*a, **k)
        finally:
            cpp03.Writer = saved
    return run


class Bodies(cpp03.Bodies):
    """The tree's bodies for one conversion, written by the C++17 Writer. The modern forms C++17 has no
    direct word for (a switch on a String, instanceof patterns) are lowered first (translate/desugar.py)."""

    def __init__(self, ctx, units):
        desugar.run(units, keep=('foreach', 'switchexpr'))
        cpp03.Bodies.__init__(self, ctx, units)

    ctor = _with_writer(cpp03.Bodies.ctor)
    field_init = _with_writer(cpp03.Bodies.field_init)
    enum_args = _with_writer(cpp03.Bodies.enum_args)
    array_items = _with_writer(cpp03.Bodies.array_items)
    instance_inits = _with_writer(cpp03.Bodies.instance_inits)
    static_blocks = _with_writer(cpp03.Bodies.static_blocks)
    body = _with_writer(cpp03.Bodies.body)


# ============================================================================================ whole programs
from codebreaker.translate.backends import jmodel  # noqa: E402

_PRIM = {'bool': 'bool', 'i8': 'int8_t', 'i16': 'int16_t', 'u16': 'char16_t', 'i32': 'int32_t', 'i64': 'int64_t',
         'f32': 'float', 'f64': 'double', 'void': 'void'}
_NUMERIC = ('int8_t', 'int16_t', 'char16_t', 'int32_t', 'int64_t', 'float', 'double')
_INTEGRAL = ('int8_t', 'int16_t', 'char16_t', 'int32_t', 'int64_t')
# java.lang's boxes and the primitive each holds
_BOX = {'java.lang.Integer': 'int32_t', 'java.lang.Long': 'int64_t', 'java.lang.Short': 'int16_t',
        'java.lang.Byte': 'int8_t', 'java.lang.Character': 'char16_t', 'java.lang.Boolean': 'bool',
        'java.lang.Float': 'float', 'java.lang.Double': 'double'}
# library classes -> the runtime's (jrt.hpp)
_LIB = {
    'java.lang.Object': 'Object', 'java.lang.String': 'String', 'java.lang.CharSequence': 'Object',
    'java.lang.Integer': 'Integer', 'java.lang.Long': 'Long', 'java.lang.Short': 'Short', 'java.lang.Byte': 'Byte',
    'java.lang.Character': 'Character', 'java.lang.Boolean': 'Boolean', 'java.lang.Float': 'Float',
    'java.lang.Double': 'Double', 'java.lang.Number': 'Object', 'java.lang.StringBuilder': 'StringBuilder',
    'java.lang.StringBuffer': 'StringBuilder', 'java.lang.AbstractStringBuilder': 'StringBuilder', 'java.lang.Math': 'Math', 'java.lang.StrictMath': 'Math',
    'java.lang.System': 'System', 'java.lang.Comparable': 'Comparable', 'java.lang.Iterable': 'Iterable',
    'java.lang.Runnable': 'Runnable', 'java.lang.Enum': 'Enum', 'java.lang.Record': 'Object',
    'java.lang.Throwable': 'Throwable', 'java.io.PrintStream': 'PrintStream',
    'java.util.List': 'List', 'java.util.ArrayList': 'ArrayList', 'java.util.LinkedList': 'LinkedList',
    'java.util.Collection': 'Collection', 'java.util.Iterator': 'Iterator', 'java.util.Map': 'Map',
    'java.util.HashMap': 'HashMap', 'java.util.LinkedHashMap': 'LinkedHashMap', 'java.util.TreeMap': 'TreeMap',
    'java.util.Map.Entry': 'MapEntry', 'java.util.Set': 'Set', 'java.util.HashSet': 'HashSet',
    'java.util.LinkedHashSet': 'LinkedHashSet', 'java.util.TreeSet': 'TreeSet', 'java.util.Deque': 'LinkedList',
    'java.util.Queue': 'LinkedList', 'java.util.ArrayDeque': 'ArrayDeque', 'java.util.Arrays': 'Arrays',
    'java.util.Collections': 'Collections', 'java.util.Objects': 'Objects', 'java.util.Optional': 'Optional',
    'java.util.OptionalInt': 'OptionalInt', 'java.util.OptionalDouble': 'OptionalDouble',
    'java.util.Comparator': 'Comparator', 'java.util.stream.Stream': 'Stream', 'java.util.stream.IntStream': 'IntStream',
    'java.util.stream.Collectors': 'Collectors', 'java.util.stream.Collector': 'Collector',
}
for _q in jmodel.SAMS:
    _LIB.setdefault(_q, _q.rsplit('.', 1)[-1])
for _q in jmodel.LIB_SUPERS:
    if _q.endswith(('Exception', 'Error')) or _q == 'java.lang.Throwable':
        _LIB.setdefault(_q, _q.rsplit('.', 1)[-1])
# where a library class's static members live in the runtime, when not in the class itself
_STATICS = {'java.lang.Integer': 'Integer_', 'java.lang.Long': 'Long_', 'java.lang.Double': 'Double_',
            'java.lang.Float': 'Float_', 'java.lang.Short': 'Short_', 'java.lang.Byte': 'Byte_',
            'java.lang.Character': 'Character'}
_STATIC_FIELDS = {('java.lang.System', 'out'): 'System::out_()', ('java.lang.System', 'err'): 'System::err_()',
                  ('java.lang.Character', 'MAX_VALUE'): 'Character_::MAX_VALUE',
                  ('java.lang.Character', 'MIN_VALUE'): 'Character_::MIN_VALUE'}
_KEYWORDS = (cpp03._CPP_KEYWORDS - frozenset(('min', 'max'))) | frozenset(('ref', 'O', 'jrt', 'std', 'self_', 'NULL', 'EOF', 'stdin',
                                             'stdout', 'stderr', 'errno', 'assert', 'offsetof', 'INFINITY', 'NAN'))
_OBJ_T = ir.Type('class', jmodel.OBJECT)
_STR_T = ir.Type('class', 'java.lang.String')


def _ident(n):
    return n + '_' if n in _KEYWORDS else n.replace('$', '_')


def _u16(v):
    """A Java string as a C++ UTF-16 literal's text."""
    out = []
    prev_hex = False
    for ch in v:
        o = ord(ch)
        if ch == '\\':
            s = '\\\\'
        elif ch == '"':
            s = '\\"'
        elif ch == '\n':
            s = '\\n'
        elif ch == '\t':
            s = '\\t'
        elif ch == '\r':
            s = '\\r'
        elif 32 <= o < 127:
            s = ('" u"' + ch) if prev_hex and ch in '0123456789abcdefABCDEF' else ch
            prev_hex = False
            out.append(s)
            continue
        else:
            s = '\\x%04x' % o
            out.append(s)
            prev_hex = True
            continue
        prev_hex = False
        out.append(s)
    return 'u"%s"' % ''.join(out)


def _char(c):
    o = ord(c) if isinstance(c, str) else int(c)
    if 32 <= o < 127 and chr(o) not in '\\\'':
        return "u'%s'" % chr(o)
    return '(char16_t)%d' % o


def _is_ref(ct):
    return ct.startswith('ref<') or ct == 'nullptr_t'


def _cls_of(ct):
    m = re.match(r'^ref<(.*)>$', ct)
    return m.group(1) if m else None


class Program(object):
    """A whole Java program (its L2 units) as one C++17 translation unit on jrt.hpp."""

    def __init__(self, units, main=None):
        desugar.run(units, keep=('foreach', 'switchexpr'))
        self.units = units
        self.model = jmodel.Model(units)
        self.model.name_anonymous()
        self.main = main
        self.cnames = set(ci.cname for ci in self.model.classes.values())
        self.used = set()                         # runtime names used (for `using`)
        self.notes = []                           # what was approximated, for the ledger
        self.method_names = set()
        for ci in self.model.classes.values():
            for m in ci.methods():
                self.method_names.add(m.name)
        self.declined = []

    # ---------------------------------------------------------------- names and types
    def rt(self, name):
        """A runtime class by name: bare when the program has no class of that name (a `using` brings it)."""
        if name in self.cnames:
            return 'jrt::' + name
        self.used.add(name)
        return name

    def field_name(self, name):
        """A field's C++ name: Java lets a field and a method share a name (a record's x and x()); C++ does not."""
        n = _ident(name)
        return n + '_' if name in self.method_names else n

    def cls_name(self, qname):
        ci = self.model.info(qname)
        if ci is not None:
            return ci.cname
        if qname in _LIB:
            return self.rt(_LIB[qname])
        raise Decline('library class %s' % qname)

    def ct(self, t):
        """The C++ type of a Java type (type variables erased)."""
        if t is None:
            return 'void'
        if t.kind in _PRIM:
            return _PRIM[t.kind]
        if t.kind == 'null':
            return 'nullptr_t'
        if t.kind in ('typevar', 'wildcard', 'any'):
            if t.kind == 'wildcard' and t.bound and t.bound[0] == 'extends':
                return self.ct(t.bound[1])
            return 'ref<%s>' % self.rt('Object')
        if t.kind == 'array':
            return 'ref<%s<%s>>' % (self.rt('Array'), self.ct(t.elem))
        if t.kind == 'class':
            return 'ref<%s>' % self.cls_name(t.name)
        raise Decline('type %r' % (t,))

    def qname_of_ct(self, ct):
        c = _cls_of(ct)
        if c is None:
            return None
        c = c[5:] if c.startswith('jrt::') else c
        for ci in self.model.classes.values():
            if ci.cname == c:
                return ci.qname
        for q, n in _LIB.items():
            if n == c:
                return q
        return None

    def is_sub_ct(self, a, b):
        """Is C++ reference type a usable where b is wanted without a cast?"""
        if a == b or a == 'nullptr_t':
            return True
        if b == 'ref<%s>' % self.rt('Object'):
            return True
        if 'Array<' in a or 'Array<' in b:
            return False
        qa, qb = self.qname_of_ct(a), self.qname_of_ct(b)
        if qa is None or qb is None:
            return False
        if qa == 'java.util.LinkedList' and qb in ('java.util.Deque', 'java.util.Queue'):
            return True
        return self.model.is_sub(qa, qb)

    def sig(self, ci, m):
        """(C++ parameter types, C++ result type) of a program method: the root declaration's, erased."""
        root = self.model.overrides(ci, m)
        params = root[0] if root else [p.type for p in m.params]
        ret = root[1] if root else m.ret
        return [self.ct(jmodel.erase(p)) for p in params], self.ct(jmodel.erase(ret)) if m.kind == 'method' else 'void'

    def call_sig(self, sym):
        """(params, result, the program's method or None) for a call through a resolved symbol."""
        ci, m = self.model.method_decl(sym)
        if m is not None:
            ps, r = self.sig(ci, m)
            return ps, r, (ci, m)
        return [self.ct(jmodel.erase(p)) for p in sym.params], self.ct(jmodel.erase(sym.ret)), None

    def two_phase(self, ci):
        """Whether the class is made in two phases (jrt::make + ctor_), as the JVM makes it. A record, or a
        class nothing extends whose base is Object and whose constructors never let `this` out, gets real
        C++ constructors instead: with no subclass no virtual call in them can reach an override."""
        if ci.kind in ('interface', 'enum') or ci.anon or ci.local or ci.needs_outer:
            return ci.kind != 'interface'
        if ci.base != jmodel.OBJECT or ci.ifaces and ci.kind != 'record':
            return True
        if self.model.subclassed(ci.qname):
            return True
        for m in ci.methods():
            if m.kind == 'ctor' and m.body is not None:
                for n in ir.walk(m.body):
                    if isinstance(n, (ir.Lambda, ir.MethodRef, ir.New, ir.LocalClass)):
                        return True
                    if isinstance(n, ir.CtorCall) and n.kind.startswith('this'):
                        return True
                    if isinstance(n, ir.This) and not _receiver(m.body, n):
                        return True
                    if isinstance(n, ir.Call) and n.target is None:
                        return True                   # one of its own methods: it could hand `this` out
        for f in ci.fields():
            if f.init is not None and not (f.sym is not None and f.sym.static):
                for n in ir.walk(f.init):
                    if isinstance(n, (ir.This, ir.Lambda, ir.New, ir.Call)):
                        return True
        return False

    def needs_clinit(self, ci):
        for x in ci.members:
            if isinstance(x, ir.MethodDecl) and x.kind == 'static_init':
                return True
            if isinstance(x, ir.FieldDecl) and x.sym is not None and x.sym.static and x.init is not None \
                    and not _constant_init(x):
                return True
            if isinstance(x, ir.EnumConst):
                return True
        return False

    # ---------------------------------------------------------------- the program
    def write(self):
        order = self.ordered()
        body = []
        for ci in order:
            body += self.struct(ci)
        defs = []
        for ci in order:
            defs += self.definitions(ci)
        head = ['// written by CodeBreaker\'s C++17 back end (codebreaker/translate/backends/cpp17.py) from the',
                '// Java program\'s L2; it runs on jrt.hpp, the Java library in C++17.']
        cyc = self.model.cycles()
        if cyc:
            head.append('// reference counting cannot free a cycle: these classes can form one, where a collector')
            head.append('// (Boehm) would be needed to reclaim it: %s' % ', '.join(cyc))
        head += ['#include "jrt.hpp"', '']
        head += ['using jrt::ref;'] + ['using jrt::%s;' % n for n in sorted(self.used) if n not in self.cnames]
        head.append('')
        head += ['struct %s;' % ci.cname for ci in order]
        head.append('')
        mains = [ci for ci in order if any(m.name == 'main' and m.sym is not None and m.sym.static and len(m.params) == 1
                                           for m in ci.methods())]
        tail = ['', 'int main(int argc, char** argv) {']
        if mains:
            pick = self.main or mains[0].cname
            tail.append('\tconst char* which = std::getenv("JRT_MAIN");')
            for ci in mains:
                tail.append('\tif (which && std::strcmp(which, "%s") == 0) return jrt::run_main(&%s::main, argc, argv);'
                            % (ci.qname.rsplit('.', 1)[-1], ci.cname))
            tail.append('\treturn jrt::run_main(&%s::main, argc, argv);' % pick)
        else:
            tail.append('\treturn 0;')
        tail.append('}')
        return '\n'.join(head + body + defs + tail) + '\n'

    def ordered(self):
        """Classes with every base before what extends it."""
        out, seen = [], set()

        def visit(ci):
            if ci.qname in seen:
                return
            seen.add(ci.qname)
            for q in ([ci.base] if ci.base else []) + ci.ifaces:
                b = self.model.info(q)
                if b is not None:
                    visit(b)
            out.append(ci)
        for ci in sorted(self.model.classes.values(), key=lambda c: (c.decl.line, c.cname)):
            visit(ci)
        return out

    # ---------------------------------------------------------------- declarations
    def bases(self, ci):
        out = []
        if ci.base and ci.base != jmodel.OBJECT:
            out.append(self.cls_name(ci.base))
        for q in ci.ifaces:
            if q in ('java.lang.Comparable',):
                continue                              # compareTo is reached through natural()'s dynamic test
            out.append('virtual ' + self.cls_name(q))
        if ci.kind == 'interface' or not out or (ci.base == jmodel.OBJECT):
            out.insert(0, 'virtual ' + self.rt('Object'))
        if any(q == 'java.lang.Comparable' for q in ci.ifaces):
            out.append('virtual ' + self.rt('Comparable'))
        return out

    def struct(self, ci):
        L = ['struct %s : %s {' % (ci.cname, ', '.join(self.bases(ci)))]
        L.append('\tconst char16_t* jclass_() const override { return %s; }' % _u16(ci.qname.strip('<>')))
        if ci.needs_outer:
            L.append('\t%s outer_;' % self.ct(ir.Type('class', ci.outer.qname)))
        for name, t in ci.captures:
            L.append('\t%s cap_%s{};' % (self.ct(t), _ident(name)))
        for f in ci.fields():
            if f.sym is None:
                continue
            t = self.ct(f.type)
            name = self.field_name(f.sym.name)
            if f.sym.static:
                if _constant_init(f):
                    L.append('\tstatic inline %s %s = %s;' % (t, name, Method(self, ci, None).constant(f)))
                else:
                    L.append('\tstatic inline %s %s{};' % (t, name))
            else:
                L.append('\t%s %s{};' % (t, name))
        consts = [x for x in ci.members if isinstance(x, ir.EnumConst)]
        for x in consts:
            L.append('\tstatic inline ref<%s> %s;' % (ci.cname, _ident(x.sym.name)))
        if ci.kind == 'enum':
            L.append('\tstatic inline ref<%s<ref<%s>>> values_;' % (self.rt('Array'), ci.cname))
            L.append('\tstatic ref<%s<ref<%s>>> values() { clinit_(); return values_->clone(); }' % (self.rt('Array'), ci.cname))
            L.append('\tstatic ref<%s> valueOf(ref<%s> n);' % (ci.cname, self.rt('String')))
        if self.needs_clinit(ci):
            L.append('\tstatic inline bool inited_ = false;')
            L.append('\tstatic void clinit_();')
        two = self.two_phase(ci)
        names = set()
        for m in ci.methods():
            names.add(m.name)
            if m.kind == 'ctor':
                ps = self.ctor_params(ci, m)
                if two:
                    L.append('\tvoid ctor_(%s);' % ps)
                else:
                    L.append('\t%s(%s);' % (ci.cname, ps))
                continue
            ps, r = self.sig(ci, m)
            params = ', '.join('%s %s' % (t, self.param_name(p, t, ci, m)) for t, p in zip(ps, m.params))
            static = m.sym is not None and m.sym.static
            if static:
                L.append('\tstatic %s %s(%s);' % (r, _ident(m.name), params))
            elif m.body is None:
                L.append('\tvirtual %s %s(%s) = 0;' % (r, _ident(m.name), params))
            else:
                L.append('\t%s%s %s(%s);' % ('' if 'private' in (m.mods or []) else 'virtual ', r, _ident(m.name), params))
        if two and ci.kind != 'interface':
            if not any(m.kind == 'ctor' for m in ci.methods()):
                L.append('\ttemplate <class... A> void ctor_(A&&... a);')
            L.append('\tvoid finit_();')
        elif ci.kind != 'interface' and not any(m.kind == 'ctor' and not m.params for m in ci.methods()):
            L.append('\t%s() {}' % ci.cname)
        # a method declared here hides the base's overloads of the same name (C++ name lookup): bring them in
        for n in sorted(names):
            for q in self.model.ancestors(ci.qname)[1:]:
                b = self.model.info(q)
                if b is not None and b.kind != 'interface' and any(m.name == n and m.kind == 'method' for m in b.methods()):
                    L.append('\tusing %s::%s;' % (b.cname, _ident(n)))
                    break
        if ci.kind == 'interface' and self.model.sam(ci.qname) is not None:
            L.append('\ttemplate <class L> struct Of;')
        L.append('};')
        sam = self.model.sam(ci.qname) if ci.kind == 'interface' else None
        if sam is not None:
            name, ps, r = sam
            cps = [self.ct(jmodel.erase(p)) for p in ps]
            cr = self.ct(jmodel.erase(r))
            params = ', '.join('%s a%d' % (t, i) for i, t in enumerate(cps))
            args = ', '.join('a%d' % i for i in range(len(cps)))
            L += ['template <class L> struct %s::Of : %s {' % (ci.cname, ci.cname), '\tL l;',
                  '\texplicit Of(L x) : l(std::move(x)) {}',
                  '\t%s %s(%s) override { return l(%s); }' % (cr, _ident(name), params, args), '};']
        L.append('')
        return L

    def param_name(self, p, t, ci, m):
        """A parameter's name: its own, or `name_` when its C++ type is the erased root's and the body
        sees it as javac typed it (a cast at entry)."""
        name = _ident(p.sym.name)
        return name + '_in' if t != self.ct(p.type) else name

    def ctor_params(self, ci, m):
        ps = []
        if ci.kind == 'enum':
            ps += ['ref<%s> name_in' % self.rt('String'), 'int32_t ordinal_in']
        ps += ['%s %s' % (self.ct(p.type), _ident(p.sym.name)) for p in m.params]
        return ', '.join(ps)

    # ---------------------------------------------------------------- definitions
    def definitions(self, ci):
        L = []
        two = self.two_phase(ci)
        clinit = self.needs_clinit(ci)
        for m in ci.methods():
            if m.body is None:
                continue
            try:
                L += Method(self, ci, m).define(two, clinit)
            except Decline as e:
                self.declined.append('%s.%s: %s' % (ci.cname, m.name, e))
                raise
        if two and ci.kind != 'interface':
            if not any(m.kind == 'ctor' for m in ci.methods()):      # an anonymous class: the super's constructor
                base = self.cls_name(ci.base) if ci.base and ci.base != jmodel.OBJECT else None
                L.append('template <class... A> void %s::ctor_(A&&... a) {' % ci.cname)
                if base and not self.model.is_interface(ci.base):
                    L.append('\t%s::ctor_(std::forward<A>(a)...);' % base)
                L.append('\tfinit_();')
                L.append('}')
            L += Method(self, ci, None).finit()
        if clinit:
            L += Method(self, ci, None).clinit()
        if ci.kind == 'enum':
            L += ['ref<%s> %s::valueOf(ref<%s> n) {' % (ci.cname, ci.cname, self.rt('String')), '\tclinit_();',
                  '\tfor (auto& v : *values_) if (v->name()->equals(n)) return v;',
                  '\tjrt::raise_msg(std::make_shared<jrt::IllegalArgumentException>(), u"No enum constant %s." + n->s);'
                  % ci.qname, '}', '']
        return L


def _constant_init(f):
    """A static field whose initialiser is a literal number, char or boolean (no class initialisation needed)."""
    init = f.init
    while isinstance(init, ir.Paren):
        init = init.x
    if isinstance(init, ir.Unary) and init.op == '-' and isinstance(init.x, ir.Lit):
        init = init.x
    return isinstance(init, ir.Lit) and init.value is not None and not isinstance(init.value, str) \
        and f.type is not None and f.type.is_prim


def _receiver(root, node):
    """Whether This `node` is only used as the receiver of a field read or a call (it does not escape)."""
    for n in ir.walk(root):
        if isinstance(n, ir.Select) and n.obj is node:
            return True
        if isinstance(n, ir.Call) and n.target is node:
            return True
    return False


class Method(object):
    """One method's (or initialiser's) C++ text."""

    def __init__(self, prog, ci, m):
        self.p = prog
        self.ci = ci
        self.m = m
        self.vars = {}
        self.ret = 'void'
        self.this = 'this'
        self.pending = []
        self.temps = 0
        self.order = False
        self.subst = {}
        self.cont_next = None
        self.lambda_depth = 0
        self.uses_self = False
        self.captures = dict((n, 'cap_' + _ident(n)) for n, _t in ci.captures) if ci is not None else {}
        self.static = m is not None and m.sym is not None and m.sym.static

    # ---- whole members
    def define(self, two, clinit):
        p, ci, m = self.p, self.ci, self.m
        L = []
        if m.kind == 'ctor':
            head = ('void %s::ctor_(%s) {' if two else '%s::%s(%s) {')
            ps = p.ctor_params(ci, m)
            L.append(head % (ci.cname, ps) if two else head % (ci.cname, ci.cname, ps))
            for prm in m.params:
                self.vars[_ident(prm.sym.name)] = p.ct(prm.type)
            if clinit:
                L.append('\tclinit_();')
            stmts = list(m.body.stmts)
            first = stmts[0] if stmts and isinstance(stmts[0], ir.CtorCall) else None
            if first is not None:
                stmts = stmts[1:]
            delegated = first is not None and first.kind.startswith('this')
            if ci.kind == 'enum':
                L.append('\tjrt::Enum::ctor_(name_in, ordinal_in);')
            if first is not None and not first.kind.endswith(':implicit') or delegated:
                L += self.ctor_call(first, '\t', two)
            elif ci.base and ci.base != jmodel.OBJECT and ci.kind != 'enum' and two:
                L.append('\t%s::ctor_();' % p.cls_name(ci.base))
            if not delegated:
                if two:
                    L.append('\tfinit_();')
                else:
                    L += self.field_inits('\t')
            self.ret = 'void'
            out = []
            self.stmts(stmts, out, '\t')
            L += out
            L += ['}', '']
            return L
        ps, r = p.sig(ci, m)
        self.ret = r
        params = []
        entry = []
        for t, prm in zip(ps, m.params):
            name = p.param_name(prm, t, ci, m)
            params.append('%s %s' % (t, name))
            own = p.ct(prm.type)
            self.vars[_ident(prm.sym.name)] = own
            if name != _ident(prm.sym.name):                    # erased by the root: cast back at entry
                entry.append('\t%s %s = %s;' % (own, _ident(prm.sym.name), self.conv(name, t, own)))
        L.append('%s %s::%s(%s) {' % (r, ci.cname, _ident(m.name), ', '.join(params)))
        if clinit and self.static:
            L.append('\tclinit_();')
        L += entry
        out = []
        self.stmts(m.body.stmts, out, '\t')
        L += out
        L += ['}', '']
        return L

    def ctor_call(self, call, ind, two):
        p, ci = self.p, self.ci
        if call.kind.startswith('this'):
            ps = [p.ct(x) for x in call.sym.params]
            args = self.args(call.args, ps, call.sym)
            return [ind + 'this->ctor_(%s);' % args]
        base = ci.base
        if base == jmodel.OBJECT or base is None:
            return []
        if call.target is not None:
            raise Decline('qualified super(...)')
        bci, bm = p.model.method_decl(call.sym)
        ps = [p.ct(x.type) for x in bm.params] if bm is not None else [p.ct(jmodel.erase(x)) for x in call.sym.params]
        args = self.args(call.args, ps, call.sym)
        if bci is not None and bci.needs_outer:
            args = (self.outer_for(bci) + (', ' if args else '') + args) if False else args
        return [ind + '%s::ctor_(%s);' % (p.cls_name(base), args)]

    def field_inits(self, ind):
        """The instance field initialisers and instance initialiser blocks, in source order."""
        out = []
        for x in self.ci.members:
            if isinstance(x, ir.FieldDecl) and x.init is not None and x.sym is not None and not x.sym.static:
                saved, self.ret = self.ret, 'void'
                self.stmt(ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Select(obj=ir.This(qualifier=None, type=None),
                                                                                sym=x.sym, type=x.type),
                                                     value=x.init, overflow=None, type=x.type)), out, ind)
                self.ret = saved
            elif isinstance(x, ir.MethodDecl) and x.kind == 'init':
                self.stmts(x.body.stmts, out, ind)
        return out

    def finit(self):
        return ['void %s::finit_() {' % self.ci.cname] + self.field_inits('\t') + ['}', '']

    def clinit(self):
        p, ci = self.p, self.ci
        L = ['void %s::clinit_() {' % ci.cname, '\tif (inited_) return;', '\tinited_ = true;']
        b = p.model.info(ci.base) if ci.base else None
        if b is not None and p.needs_clinit(b):
            L.append('\t%s::clinit_();' % b.cname)
        self.static = True
        consts = [x for x in ci.members if isinstance(x, ir.EnumConst)]
        for k, x in enumerate(consts):
            sub = p.model.by_decl.get(id(x.body)) if x.body is not None else None
            target = sub.cname if sub is not None else ci.cname
            ctor_ps = []
            if x.ctor is not None:
                _cc, cm = p.model.method_decl(x.ctor)
                ctor_ps = [p.ct(q.type) for q in cm.params] if cm is not None else [p.ct(q) for q in x.ctor.params]
            args = self.args(x.args or [], ctor_ps, x.ctor)
            L.append('\t%s = jrt::make<%s>(jrt::lit(%s), %d%s);' % (_ident(x.sym.name), target, _u16(x.sym.name), k,
                                                                     (', ' + args) if args else ''))
        if ci.kind == 'enum':
            L.append('\tvalues_ = jrt::array_of<ref<%s>>({%s});' % (ci.cname, ', '.join(_ident(x.sym.name) for x in consts)))
        for x in ci.members:
            if isinstance(x, ir.FieldDecl) and x.sym is not None and x.sym.static and x.init is not None \
                    and not _constant_init(x):
                out = []
                self.stmt(ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Select(obj=ir.TypeName(type=ir.Type('class', ci.qname)),
                                                                                sym=x.sym, type=x.type),
                                                     value=x.init, overflow=None, type=x.type)), out, '\t')
                L += out
            elif isinstance(x, ir.MethodDecl) and x.kind == 'static_init':
                out = []
                self.stmts(x.body.stmts, out, '\t')
                L += out
        L += ['}', '']
        return L

    def constant(self, f):
        code, ct = self.expr(f.init)
        return self.conv(code, ct, self.p.ct(f.type))

    # ---- statements
    def stmts(self, ss, out, ind):
        for s in ss:
            self.stmt(s, out, ind)

    def block(self, s, out, ind):
        saved = dict(self.vars)
        if isinstance(s, ir.Block):
            self.stmts(s.stmts, out, ind + '\t')
        elif s is not None:
            self.stmt(s, out, ind + '\t')
        self.vars = saved

    def loop_body(self, body, out, ind):
        lab, self.cont_next = self.cont_next, None
        if lab is None:
            self.block(body, out, ind)
            return
        out.append(ind + '\t{')
        self.block(body, out, ind + '\t')
        out.append(ind + '\t}')
        out.append(ind + '\t%s_cont:;' % lab)

    def stmt(self, s, out, ind):
        saved, self.pending = self.pending, []
        saved_order, self.order = self.order, True
        saved_subst = dict(self.subst)
        sub = []
        try:
            self._stmt(s, sub, ind)
        finally:
            decls, self.pending, self.order = self.pending, saved, saved_order
            self.subst = saved_subst
        out.extend(ind + d for d in decls)
        out.extend(sub)

    def cond(self, e):
        e = cpp03._strip_paren(e)
        code, ct = self.expr(e)
        return self.conv(code, ct, 'bool')

    def _stmt(self, s, out, ind):
        p = self.p
        if isinstance(s, ir.Block):
            out.append(ind + '{')
            self.block(s, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.Local):
            name = _ident(s.sym.name)
            ct = p.ct(s.type)
            self.vars[name] = ct
            if s.init is None:
                out.append(ind + '%s %s{};' % (ct, name))
                return
            code, it = self.expr(s.init)
            init = cpp03._strip_paren(s.init)
            if it == ct and isinstance(init, (ir.New, ir.Cast)) and _is_ref(ct):
                out.append(ind + 'auto %s = %s;' % (name, code))        # the type is spelled where it is made
            else:
                out.append(ind + '%s %s = %s;' % (ct, name, self.conv(code, it, ct)))
        elif isinstance(s, ir.ExprStmt):
            code, _t = self.expr(s.expr)
            out.append(ind + code + ';')
        elif isinstance(s, ir.If):
            out.append(ind + 'if (%s) {' % self.cond(s.cond))
            self.block(s.then, out, ind)
            e = s.orelse
            while isinstance(e, ir.If):
                out.append(ind + '} else if (%s) {' % self.cond(e.cond))
                self.block(e.then, out, ind)
                e = e.orelse
            if e is not None:
                out.append(ind + '} else {')
                self.block(e, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.While):
            out.append(ind + 'while (%s) {' % self.cond(s.cond))
            self.loop_body(s.body, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.DoWhile):
            out.append(ind + 'do {')
            self.loop_body(s.body, out, ind)
            out.append(ind + '} while (%s);' % self.cond(s.cond))
        elif isinstance(s, ir.For):
            saved = dict(self.vars)
            init = ''
            if s.init and isinstance(s.init[0], ir.Local):
                ct = p.ct(s.init[0].type)
                parts = []
                for l in s.init:
                    n = _ident(l.sym.name)
                    self.vars[n] = ct
                    if l.init is not None:
                        c, t = self.expr(l.init)
                        parts.append('%s = %s' % (n, self.conv(c, t, ct)))
                    else:
                        parts.append(n)
                init = '%s %s' % (ct, ', '.join(parts))
            elif s.init:
                init = ', '.join(self.expr(i.expr)[0] for i in s.init)
            cond = self.cond(s.cond) if s.cond is not None else ''
            upd = ', '.join(self.expr(u)[0] for u in s.update)
            out.append(ind + 'for (%s; %s; %s) {' % (init, cond, upd))
            self.loop_body(s.body, out, ind)
            out.append(ind + '}')
            self.vars = saved
        elif isinstance(s, ir.ForEach):
            self.foreach(s, out, ind)
        elif isinstance(s, ir.Labeled):
            lab = s.label
            used = [n for n in ir.walk(s.body) if isinstance(n, (ir.Break, ir.Continue)) and n.label == lab]
            if any(isinstance(n, ir.Continue) for n in used):
                self.cont_next = lab
            self.stmt(s.body, out, ind)
            if any(isinstance(n, ir.Break) for n in used):
                out.append(ind + '%s_brk:;' % lab)
        elif isinstance(s, ir.Return):
            if s.expr is None:
                out.append(ind + 'return;')
            else:
                code, t = self.expr(s.expr)
                out.append(ind + 'return %s;' % self.conv(code, t, self.ret))
        elif isinstance(s, ir.Break):
            out.append(ind + ('goto %s_brk;' % s.label if s.label else 'break;'))
        elif isinstance(s, ir.Continue):
            out.append(ind + ('goto %s_cont;' % s.label if s.label else 'continue;'))
        elif isinstance(s, ir.Throw):
            code, _t = self.expr(s.expr)
            out.append(ind + 'jrt::raise(%s);' % code)
        elif isinstance(s, ir.Switch):
            self.switch(s, out, ind)
        elif isinstance(s, ir.Try):
            self.try_(s, out, ind)
        elif isinstance(s, ir.Empty):
            out.append(ind + ';')
        elif isinstance(s, ir.Sync):
            out.append(ind + '{   // synchronized: one thread here')
            self.block(s.body, out, ind)
            out.append(ind + '}')
        elif isinstance(s, ir.Assert):
            out.append(ind + '// assert %s (Java runs without -ea: not checked)' % cpp03._comment(self.expr(s.cond)[0]))
        elif isinstance(s, ir.LocalClass):
            pass                                                # declared at the top level (Model)
        elif isinstance(s, ir.Yield):
            code, t = self.expr(s.expr)
            out.append(ind + 'return %s;' % self.conv(code, t, self.ret))
        elif isinstance(s, ir.CtorCall):
            raise Decline('constructor call not first')
        else:
            raise Decline('statement %s' % type(s).__name__)

    def entry_binding(self, s):
        """for (Map.Entry<K, V> e : m.entrySet()) that only reads e.getKey() / e.getValue() and touches no map
        (no call but into String, StringBuilder, the boxes, Math, printing): (key, value) uses to bind, or None."""
        it = cpp03._strip_paren(s.iterable)
        vt = s.var.type
        if not (isinstance(it, ir.Call) and it.sym is not None and it.sym.name == 'entrySet' and not it.args
                and vt is not None and vt.kind == 'class' and vt.name == 'java.util.Map.Entry' and len(vt.args) == 2):
            return None
        name = s.var.sym.name
        uses = {}
        parents = dict((id(c), n) for n in ir.walk(s.body) for c in n.children())
        for n in ir.walk(s.body):
            if isinstance(n, ir.Name) and n.sym is not None and n.sym.name == name:
                call = parents.get(id(n))
                if not (isinstance(call, ir.Call) and call.target is n and call.sym is not None
                        and call.sym.name in ('getKey', 'getValue') and not call.args):
                    return None
                uses[id(call)] = call.sym.name
            if isinstance(n, ir.Call) and n.sym is not None and n.sym.name not in ('getKey', 'getValue') and \
                    n.sym.owner not in _PURE_LIB:
                return None
            if isinstance(n, ir.Assign) and not isinstance(cpp03._strip_paren(n.target), ir.Name):
                return None
            if isinstance(n, (ir.Lambda, ir.MethodRef, ir.New)) and not (isinstance(n, ir.New) and n.body is None
                                                                       and n.type is not None and n.type.name in _PURE_LIB):
                return None
        return uses

    def foreach(self, s, out, ind):
        p = self.p
        uses = self.entry_binding(s)
        if uses is not None:
            m, _mt = self.expr(cpp03._strip_paren(s.iterable).target)
            k_t, v_t = s.var.type.args
            kc, vc = p.ct(jmodel.erase(k_t)), p.ct(jmodel.erase(v_t))
            base = _ident(s.var.sym.name)
            kn, vn = base + '_key', base + '_value'
            out.append(ind + 'for (auto [%s, %s] : jrt::entries<%s, %s>(%s)) {' % (kn, vn, _cls_of(kc), _cls_of(vc), m))
            self.vars[kn], self.vars[vn] = kc, vc
            saved = dict(self.subst)
            for n in ir.walk(s.body):
                if id(n) in uses:
                    self.subst[id(n)] = (kn, kc) if uses[id(n)] == 'getKey' else (vn, vc)
            self.loop_body(s.body, out, ind)
            self.subst = saved
            out.append(ind + '}')
            return
        coll, ct = self.expr(s.iterable)
        name = _ident(s.var.sym.name)
        vt = p.ct(s.var.type)
        self.vars[name] = vt
        t = s.iterable.type
        if t is not None and t.kind == 'array':
            et = p.ct(t.elem)
            if et == vt:
                out.append(ind + 'for (%s %s : jrt::in(%s)) {' % (vt, name, coll))
            else:
                out.append(ind + 'for (%s %s_e : jrt::in(%s)) {' % (et, name, coll))
                out.append(ind + '\t%s %s = %s;' % (vt, name, self.conv(name + '_e', et, vt)))
        else:
            if vt == 'ref<%s>' % p.rt('Object'):
                out.append(ind + 'for (%s %s : jrt::in(%s)) {' % (vt, name, coll))
            else:
                elem = vt if not _is_ref(vt) else _cls_of(vt)
                out.append(ind + 'for (%s %s : jrt::each<%s>(%s)) {' % (vt, name, elem, coll))
        self.loop_body(s.body, out, ind)
        out.append(ind + '}')

    def case_label(self, lab, enum):
        lab = cpp03._strip_paren(lab)
        if enum is not None:
            if isinstance(lab, ir.Select) and lab.sym is not None and lab.sym.kind == 'enumconst':
                name = lab.sym.name
            elif isinstance(lab, ir.Name):
                name = lab.sym.name
            else:
                raise Decline('enum case label')
            consts = [x.sym.name for x in enum.members if isinstance(x, ir.EnumConst)]
            return '%d /* %s */' % (consts.index(name), name)
        if isinstance(lab, ir.Lit):
            return self.expr(lab)[0]
        if isinstance(lab, ir.Select) and lab.sym is not None and lab.sym.kind == 'field':
            ci = self.p.model.info(lab.sym.owner)
            f = next((x for x in (ci.fields() if ci else []) if x.sym is not None and x.sym.name == lab.sym.name), None)
            if f is not None and f.init is not None:
                return self.case_label(f.init, None)
        if isinstance(lab, ir.Unary) and lab.op == '-' and isinstance(lab.x, ir.Lit):
            return '-' + self.expr(lab.x)[0]
        raise Decline('case label')

    def switch_head(self, e):
        x, xt = self.expr(cpp03._strip_paren(e))
        enum = self.p.model.of_type(e.type)
        if enum is not None and enum.kind == 'enum':
            return 'jrt::nn(%s)->ordinal()' % x, enum
        if _is_ref(xt):
            if xt == 'ref<%s>' % self.p.rt('String'):
                raise Decline('switch on a String not lowered')
            return 'jrt::unbox<%s>(%s)' % (_BOX.get(e.type.name, 'int32_t'), x), None
        return x, None

    def switch(self, s, out, ind):
        x, enum = self.switch_head(s.expr)
        out.append(ind + 'switch (%s) {' % x)
        for c in s.cases:
            for lab in c.labels:
                out.append(ind + 'case %s:' % self.case_label(lab, enum))
            if c.default:
                out.append(ind + 'default:')
            out.append(ind + '{')
            saved = dict(self.vars)
            for b in c.body:
                self.stmt(b, out, ind + '\t')
            self.vars = saved
            out.append(ind + '}')
        out.append(ind + '}')

    def try_(self, s, out, ind):
        p = self.p
        if s.resources:
            out.append(ind + '{')
            inner = ind + '\t'
            saved = dict(self.vars)
            names = []
            for r in s.resources:
                if isinstance(r, ir.Local):
                    self.stmt(r, out, inner)
                    names.append(_ident(r.sym.name))
                else:
                    code, ct = self.expr(r.expr)
                    self.temps += 1
                    n = 'j2x_res%d' % self.temps
                    out.append(inner + 'auto %s = %s;' % (n, code))
                    names.append(n)
            for n in names:                       # closed in reverse order of opening: the guards' order
                self.temps += 1
                out.append(inner + 'jrt::Finally j2x_close%d([&] { if (%s) %s->close(); });' % (self.temps, n, n))
            rest = ir.Try(resources=[], body=s.body, catches=s.catches, final=s.final, line=s.line, col=s.col)
            if s.catches or s.final is not None:
                raise Decline('try-with-resources with catch or finally')
            self.block(rest.body, out, ind)
            self.vars = saved
            out.append(ind + '}')
            return
        if s.final is not None:
            jumps = cpp03._leaves([s.body] + [c.body for c in s.catches])
            if not jumps:
                # no jump out: the finally on the exceptional way out (then rethrown) and after, written twice
                out.append(ind + 'try {')
                self.try_catch(s, out, ind + '\t')
                out.append(ind + '} catch (...) {')
                self.block(s.final, out, ind)
                out.append(ind + '\tthrow;')
                out.append(ind + '}')
                out.append(ind + '{')
                self.block(s.final, out, ind)
                out.append(ind + '}')
                return
            if not _simple_finally(s.final):
                raise Decline('a finally left by a jump that can itself throw or jump')
            out.append(ind + '{')
            self.temps += 1
            fin = []
            self.block(s.final, fin, ind + '\t')
            out.append(ind + '\tjrt::Finally j2x_fin%d([&] {' % self.temps)
            out.extend('\t' + x for x in fin)
            out.append(ind + '\t});')
            self.try_catch(s, out, ind + '\t')
            out.append(ind + '}')
            return
        self.try_catch(s, out, ind)

    def try_catch(self, s, out, ind):
        p = self.p
        if not s.catches:
            out.append(ind + '{')
            self.block(s.body, out, ind)
            out.append(ind + '}')
            return
        out.append(ind + 'try {')
        self.block(s.body, out, ind)
        self.temps += 1
        t = 'j2x_t%d' % self.temps
        out.append(ind + '} catch (jrt::JavaThrow& %s) {' % t)
        first = True
        for c in s.catches:
            name = _ident(c.sym.name)
            if len(c.types) == 1:
                cls = p.cls_name(c.types[0].name) if c.types[0].kind == 'class' else None
                if cls is None:
                    raise Decline('catch type')
                test = 'auto %s = jrt::as<%s>(%s.t)' % (name, cls, t)
                ct = 'ref<%s>' % cls
            else:
                test = ' || '.join('jrt::isa<%s>(%s.t)' % (p.cls_name(x.name), t) for x in c.types)
                ct = 'ref<%s>' % p.rt('Throwable')
            out.append(ind + '\t%sif (%s) {' % ('' if first else '} else ', test))
            first = False
            saved = dict(self.vars)
            self.vars[name] = ct
            if len(c.types) > 1:
                out.append(ind + '\t\t%s %s = %s.t;' % (ct, name, t))
            self.block(c.body, out, ind + '\t')
            self.vars = saved
        out.append(ind + '\t} else {')
        out.append(ind + '\t\tthrow;')
        out.append(ind + '\t}')
        out.append(ind + '}')

    # ---- expressions -> (code, C++ type)
    def expr(self, e):
        if id(e) in self.subst:
            return self.subst[id(e)]
        f = getattr(self, 'x_' + type(e).__name__, None)
        if f is None:
            raise Decline('expression %s' % type(e).__name__)
        pre = self.sequenced(e) if self.order else ''
        code, ct = f(e)
        return ('(%s, %s)' % (pre, code), ct) if pre else (code, ct)

    def sequenced(self, e):
        """Java's left-to-right order where C++'s is unspecified and it matters (cpp03's rule)."""
        if isinstance(e, (ir.Lambda, ir.SwitchExpr)):
            return ''
        nodes = cpp03._operands(e)
        if len(nodes) < 2 or not cpp03._order_matters(nodes):
            return ''
        parts = []
        for n in nodes:
            if cpp03._constant(n) or id(n) in self.subst or isinstance(n, (ir.Lambda, ir.MethodRef)):
                continue
            code, ct = self.expr(n)
            if ct in ('void', 'nullptr_t'):
                return ''
            self.temps += 1
            name = 'j2xo%d' % self.temps
            self.pending.append('%s %s{};' % (ct, name))
            parts.append('%s = %s' % (name, code))
            self.subst[id(n)] = (name, ct)
        return ', '.join(parts)

    def conv(self, code, src, dst):
        """`code` of C++ type src where dst is wanted: Java's assignment conversions (widening, boxing,
        unboxing) and the cast back from an erased type."""
        if src == dst or dst in ('void', '') or src == '':
            return code
        if src == 'nullptr_t':
            return code
        if dst in _NUMERIC + ('bool',) and src in _NUMERIC + ('bool',):
            if src in ('double', 'float') and dst in ('int32_t', 'int64_t', 'int16_t', 'int8_t', 'char16_t'):
                if dst == 'int64_t':
                    return 'jrt::d2l(%s)' % code
                return ('jrt::d2i(%s)' % code) if dst == 'int32_t' else '(%s)jrt::d2i(%s)' % (dst, code)
            return '(%s)(%s)' % (dst, code) if not cpp03._SIMPLE.match(code) else '(%s)%s' % (dst, code)
        if dst in _NUMERIC + ('bool',) and _is_ref(src):
            return 'jrt::unbox<%s>(%s)' % (dst, code)
        if _is_ref(dst) and src in _NUMERIC + ('bool',):
            q = self.p.qname_of_ct(dst)
            want = _BOX.get(q)
            if want and want != src:
                code = '(%s)(%s)' % (want, code)
            return 'jrt::box(%s)' % code
        if _is_ref(dst) and _is_ref(src):
            if self.p.is_sub_ct(src, dst):
                return code
            if 'Array<' in src and 'Array<' in dst:
                return code                      # an array where an erased one is wanted: the runtime's templates take it
            return 'jrt::cast<%s>(%s)' % (_cls_of(dst), code)
        return code

    def x_Paren(self, e):
        x, t = self.expr(e.x)
        return '(%s)' % x, t

    def x_Lit(self, e):
        v, k = e.value, e.type.kind if e.type is not None else 'null'
        if v is None:
            return 'nullptr', 'nullptr_t'
        if k == 'bool':
            return ('true' if v else 'false'), 'bool'
        if k == 'i32':
            return (str(v) if v != -2147483648 else '(-2147483647 - 1)'), 'int32_t'
        if k == 'i64':
            return ('%dLL' % v if v != -9223372036854775808 else '(-9223372036854775807LL - 1)'), 'int64_t'
        if k == 'f32':
            return cpp03._float(v, 'F').replace('F', 'f'), 'float'
        if k == 'f64':
            return cpp03._float(v, ''), 'double'
        if k == 'u16':
            return _char(v), 'char16_t'
        if isinstance(v, str):
            return 'jrt::lit(%s)' % _u16(v), 'ref<%s>' % self.p.rt('String')
        raise Decline('literal %r' % (v,))

    def x_Name(self, e):
        n = _ident(e.sym.name)
        if n in self.vars:
            return n, self.vars[n]
        if e.sym.name in self.captures:
            return '%s->%s' % (self.this_x(), self.captures[e.sym.name]), self.p.ct(e.type)
        raise Decline('unknown local %s' % n)

    def this_x(self):
        if self.lambda_depth:
            self.uses_self = True
            return 'self_'
        return 'this'

    def x_This(self, e):
        q = e.qualifier if e.qualifier is not None else e.type
        if q is not None and q.kind == 'class' and q.name != self.ci.qname and self.ci.outer is not None \
                and not self.p.model.is_sub(self.ci.qname, q.name):
            return self.outer_of(q.name)
        code = self.this_x()
        if code == 'this':
            return 'jrt::self(this)', 'ref<%s>' % self.ci.cname
        return code, 'ref<%s>' % self.ci.cname

    def outer_of(self, qname):
        """The enclosing instance of class qname, through the outer_ chain."""
        code, ci = self.this_x(), self.ci
        while ci is not None and not (ci.qname == qname or self.p.model.is_sub(ci.qname, qname)):
            if not ci.needs_outer:
                raise Decline('no enclosing instance of %s' % qname)
            code = '%s->outer_' % code
            ci = ci.outer
        if ci is None:
            raise Decline('no enclosing instance of %s' % qname)
        return code, 'ref<%s>' % ci.cname

    def receiver(self, obj, sym):
        """The object a member is read on: (code, C++ type, accessor)."""
        if obj is None or isinstance(obj, (ir.This, ir.Super)) and (obj.qualifier is None) and (
                obj.type is None or obj.type.name == self.ci.qname or self.p.model.is_sub(self.ci.qname, obj.type.name)
                or isinstance(obj, ir.Super)):
            if sym is not None and sym.owner and sym.owner != self.ci.qname and self.ci.outer is not None \
                    and not self.p.model.is_sub(self.ci.qname, sym.owner) and (obj is None or isinstance(obj, ir.This)):
                code, ct = self.outer_of(sym.owner)
                return code, ct
            return self.this_x(), 'ref<%s>' % self.ci.cname
        code, ct = self.expr(obj)
        if obj.type is not None and obj.type.kind in ('typevar', 'wildcard') and sym is not None and sym.owner:
            want = 'ref<%s>' % self.p.cls_name(sym.owner)
            if not self.p.is_sub_ct(ct, want):
                code, ct = 'jrt::cast<%s>(%s)' % (_cls_of(want), code), want
        return code, ct

    def x_Select(self, e):
        p, s = self.p, e.sym
        if isinstance(e.obj, ir.TypeName) or s.static:
            owner = s.owner
            ci = p.model.info(owner)
            if ci is None:
                key = (owner, s.name)
                if key in _STATIC_FIELDS:
                    return 'jrt::' + _STATIC_FIELDS[key], p.ct(e.type)
                holder = _STATICS.get(owner) or _LIB.get(owner)
                if holder is None:
                    raise Decline('library static %s.%s' % (owner, s.name))
                return 'jrt::%s::%s' % (holder, s.name), p.ct(e.type)
            name = _ident(s.name) if s.kind == 'enumconst' else p.field_name(s.name)
            code = '%s::%s' % (ci.cname, name)
            if p.needs_clinit(ci) and ci is not self.ci:
                code = '(%s::clinit_(), %s)' % (ci.cname, code)
            f = next((x for x in ci.fields() if x.sym is not None and x.sym.name == s.name), None)
            ct = p.ct(f.type) if f is not None else p.ct(e.type)
            return code, ct
        recv, rt = self.receiver(e.obj, s)
        ci = p.model.info(s.owner)
        if ci is None:
            raise Decline('library field %s.%s' % (s.owner, s.name))
        f = next((x for x in ci.fields() if x.sym is not None and x.sym.name == s.name), None)
        declared = p.ct(jmodel.erase(f.type)) if f is not None else p.ct(jmodel.erase(e.type))
        code = '%s->%s' % (recv, p.field_name(s.name))
        want = p.ct(e.type)
        if declared != want and _is_ref(want) and not p.is_sub_ct(declared, want):
            return 'jrt::cast<%s>(%s)' % (_cls_of(want), code), want
        return code, declared

    def x_TypeName(self, e):
        raise Decline('a type as a value')

    def x_Length(self, e):
        a, _t = self.expr(e.arr)
        return 'jrt::nn(%s)->length' % a, 'int32_t'

    def x_Index(self, e):
        a, t = self.expr(e.arr)
        i, it = self.expr(e.index)
        m = re.match(r'^ref<(?:jrt::)?Array<(.*)>>$', t)
        if not m:
            raise Decline('index into %s' % t)
        return '(*jrt::nn(%s))[%s]' % (a, self.conv(i, it, 'int32_t')), m.group(1)

    def unboxed(self, code, ct, t):
        """An operand of an arithmetic operator: a box is its primitive (Java unboxes it)."""
        if _is_ref(ct) and t is not None and t.kind == 'class' and t.name in _BOX:
            return 'jrt::unbox<%s>(%s)' % (_BOX[t.name], code), _BOX[t.name]
        return code, ct

    def x_Unary(self, e):
        x, t = self.expr(e.x)
        if e.op in ('++', '--'):
            if _is_ref(t):
                raise Decline('++ on a box')
            return ('%s%s' % (x, e.op) if e.postfix else '%s%s' % (e.op, x)), t
        x, t = self.unboxed(x, t, e.x.type)
        rt = self.p.ct(e.type)
        if e.op == '-' and t in ('int32_t', 'int64_t'):
            return '-%s' % x, rt                     # wraps (-fwrapv): -MIN_VALUE is MIN_VALUE, as in Java
        if e.op == '!':
            return '!%s' % x, 'bool'
        return '%s%s' % (e.op, x), rt

    def x_Binary(self, e):
        p = self.p
        l, lt = self.expr(e.left)
        r, rt = self.expr(e.right)
        op = e.op
        if op in ('==', '!=') and _is_ref(lt) and _is_ref(rt):
            return '%s %s %s' % (l, op, r), 'bool'
        if op in ('==', '!=') and 'nullptr_t' in (lt, rt):
            return '%s %s %s' % (l, op, r), 'bool'
        l, lt = self.unboxed(l, lt, e.left.type)
        r, rt = self.unboxed(r, rt, e.right.type)
        if op in ('&&', '||'):
            return '%s %s %s' % (l, op, r), 'bool'
        res = p.ct(e.type)
        if op in cpp03._BITS:
            if isinstance(e.left, ir.Binary) and e.left.op != op:
                l = '(%s)' % l
            if isinstance(e.right, ir.Binary) and e.right.op != op:
                r = '(%s)' % r
        if op in ('<<', '>>', '>>>'):
            width = 64 if lt == 'int64_t' else 32
            lit = cpp03._strip_paren(e.right)
            if not (isinstance(lit, ir.Lit) and isinstance(lit.value, int) and 0 <= lit.value < width):
                r = '(%s & %d)' % (r, width - 1)
            if op == '>>>':
                if lt not in ('int32_t', 'int64_t'):
                    l = '(int32_t)%s' % l
                return 'jrt::ushr(%s, %s)' % (l, r), res
            return '%s %s %s' % (l, op, r), res
        if op in ('/', '%') and res in _INTEGRAL:
            lit = cpp03._strip_paren(e.right)
            if not (isinstance(lit, ir.Lit) and isinstance(lit.value, int) and lit.value not in (0, -1)):
                fn = 'div' if op == '/' else 'rem'
                return 'jrt::%s<%s>(%s, %s)' % (fn, res, l, r), res
        if op == '%' and res in ('float', 'double'):
            return '(%s)std::fmod(%s, %s)' % (res, l, r), res
        return '%s %s %s' % (l, op, r), (res if op not in ('==', '!=', '<', '<=', '>', '>=') else 'bool')

    def x_Concat(self, e):
        parts = []
        for x in e.parts:
            code, ct = self.expr(x)
            if ct == 'nullptr_t':
                code = 'ref<%s>()' % self.p.rt('Object')
            elif ct in ('int8_t', 'int16_t'):
                code = '(int32_t)%s' % code
            parts.append(code)
        return 'jrt::concat(%s)' % ', '.join(parts), 'ref<%s>' % self.p.rt('String')

    def x_Assign(self, e):
        p = self.p
        t, tt = self.expr(e.target)
        v, vt = self.expr(e.value)
        if e.op is None:
            return '%s = %s' % (t, self.conv(v, vt, tt)), tt
        if tt == 'ref<%s>' % p.rt('String') and e.op == '+':
            return '%s = jrt::concat(%s, %s)' % (t, t, v), tt
        plain = isinstance(cpp03._strip_paren(e.target), ir.Name) or (
            isinstance(cpp03._strip_paren(e.target), ir.Select) and isinstance(cpp03._strip_paren(e.target).obj, (ir.This, ir.TypeName)))
        if _is_ref(tt):
            raise Decline('compound assignment to a box')
        v, vt = self.unboxed(v, vt, e.value.type)
        if e.op in ('/', '%') and tt in _INTEGRAL and vt in _INTEGRAL:
            if not plain:
                raise Decline('integer /= on a complex target')
            wide = 'int64_t' if 'int64_t' in (tt, vt) else 'int32_t'
            return '%s = (%s)jrt::%s<%s>(%s, %s)' % (t, tt, 'div' if e.op == '/' else 'rem', wide, t, v), tt
        if e.op in ('<<', '>>', '>>>'):
            width = 64 if tt == 'int64_t' else 32
            if e.op == '>>>':
                if not plain:
                    raise Decline('>>>= on a complex target')
                base = t if tt in ('int32_t', 'int64_t') else '(int32_t)%s' % t
                return '%s = (%s)jrt::ushr(%s, %s)' % (t, tt, base, '(%s & %d)' % (v, width - 1)), tt
            return '%s %s= (%s & %d)' % (t, e.op, v, width - 1), tt
        if tt in _INTEGRAL and vt in ('float', 'double'):
            if not plain:
                raise Decline('integer op= double on a complex target')
            fn = 'jrt::d2l' if tt == 'int64_t' else 'jrt::d2i'
            return '%s = (%s)%s(%s %s %s)' % (t, tt, fn, t, e.op, v), tt
        if e.op == '%' and tt in ('float', 'double'):
            if not plain:
                raise Decline('%= on a complex target')
            return '%s = (%s)std::fmod(%s, %s)' % (t, tt, t, v), tt
        return '%s %s= %s' % (t, e.op, v), tt

    def x_Cond(self, e):
        c = self.cond(e.cond)
        a, at = self.expr(e.then)
        b, bt = self.expr(e.orelse)
        t = self.p.ct(e.type) if e.type is not None and e.type.kind != 'null' else (at if at != 'nullptr_t' else bt)
        return '(%s ? %s : %s)' % (c, self.conv(a, at, t), self.conv(b, bt, t)), t

    def x_Cast(self, e):
        x, xt = self.expr(e.x)
        ct = self.p.ct(e.type)
        if ct == xt:
            return x, ct
        if ct in _NUMERIC + ('bool',) and xt in _NUMERIC + ('bool',):
            if xt in ('double', 'float') and ct != 'double' and ct != 'float':
                return self.conv(x, xt, ct), ct
            return '(%s)%s' % (ct, x if cpp03._SIMPLE.match(x) else '(%s)' % x), ct
        if _is_ref(ct) and _is_ref(xt):
            if self.p.is_sub_ct(xt, ct):
                return x, xt if xt != 'nullptr_t' else ct
            return 'jrt::cast<%s>(%s)' % (_cls_of(ct), x), ct
        return self.conv(x, xt, ct), ct

    def x_InstanceOf(self, e):
        if e.binding is not None:
            raise Decline('instanceof pattern not lowered')
        x, _xt = self.expr(e.x)
        t = e.test
        if t.kind == 'array':
            return 'jrt::isa<%s>(%s)' % (_cls_of(self.p.ct(t)), x), 'bool'
        return 'jrt::isa<%s>(%s)' % (self.p.cls_name(t.name), x), 'bool'

    def args(self, args, params, sym):
        """The arguments of a call, each converted to its parameter's C++ type; a program's varargs packed
        into the array, a library's passed one by one (the runtime's are variadic templates)."""
        args = list(args or [])
        varargs = sym is not None and sym.varargs and params
        lib = sym is not None and self.p.model.method_decl(sym)[1] is None
        if varargs and not (len(args) == len(params) and args[-1].type is not None
                            and args[-1].type.kind in ('array', 'null')):
            fixed = len(params) - 1
            head = [self.conv(*(self.expr(a) + (params[i],))) for i, a in enumerate(args[:fixed])]
            elem_t = sym.params[-1].elem if sym.params[-1].kind == 'array' else None
            ect = self.p.ct(jmodel.erase(elem_t)) if elem_t is not None else 'ref<%s>' % self.p.rt('Object')
            rest = [self.conv(*(self.expr(a) + (ect,))) for a in args[fixed:]]
            if lib:
                return ', '.join(head + rest)
            packed = 'jrt::array_of<%s>({%s})' % (ect, ', '.join(rest))
            return ', '.join(head + [packed])
        out = []
        for i, a in enumerate(args):
            want = params[i] if i < len(params) else ''
            if isinstance(cpp03._strip_paren(a), (ir.Lambda, ir.MethodRef)):
                code, ct = self.functional(cpp03._strip_paren(a), sym.params[i] if sym and i < len(sym.params) else None)
            else:
                code, ct = self.expr(a)
            out.append(self.conv(code, ct, want))
        return ', '.join(out)

    def x_Call(self, e):
        p, s = self.p, e.sym
        if s is None or s.kind != 'method':
            raise Decline('call %s' % (s.kind if s is not None else 'unresolved'))
        params, ret, decl = p.call_sig(s)
        want = p.ct(e.type)
        t = e.target
        if t is not None and t.type is not None and t.type.kind == 'array' and s.name == 'clone':
            a, at = self.expr(t)
            return 'jrt::nn(%s)->clone()' % a, at
        if t is not None and t.type is not None and t.type.kind == 'array' and s.name in ('length',):
            raise Decline('array method %s' % s.name)
        if s.owner == 'java.lang.Comparable' and s.name == 'compareTo' and t is not None \
                and not isinstance(t, (ir.This, ir.Super, ir.TypeName)):
            # Comparable is an interface the runtime's own classes (String, the boxes, enums) do not derive
            # from: the comparison goes through natural(), which knows each of them
            a, at = self.expr(t)
            b, bt = self.expr(e.args[0])
            return 'jrt::natural(%s, %s)' % (a, self.conv(b, bt, 'ref<%s>' % p.rt('Object'))), 'int32_t'
        args = self.args(e.args, params, s)
        name = _ident(s.name)
        if s.static or isinstance(t, ir.TypeName):
            ci = p.model.info(s.owner)
            if ci is not None:
                code = '%s::%s(%s)' % (ci.cname, name, args)
            else:
                holder = _STATICS.get(s.owner) or _LIB.get(s.owner)
                if holder is None:
                    raise Decline('library static %s.%s' % (s.owner, s.name))
                if s.owner == 'java.lang.String' and s.name == 'valueOf' and e.args and e.args[0].type is not None \
                        and e.args[0].type.kind in ('i8', 'i16'):
                    args = '(int32_t)' + args
                code = 'jrt::%s::%s(%s)' % (holder, name, args)
        elif isinstance(t, ir.Super):
            base = self.ci.base if not (s.owner and p.model.is_interface(s.owner)) else s.owner
            prefix = '' if self.this_x() == 'this' else self.this_x() + '->'
            code = '%s%s::%s(%s)' % (prefix, p.cls_name(base), name, args)
        else:
            recv, rt = self.receiver(t, s)
            if s.owner == jmodel.OBJECT and s.name == 'getClass':
                raise Decline('getClass')
            code = '%s->%s(%s)' % (recv if recv in ('this', 'self_') or t is None else 'jrt::nn(%s)' % recv, name, args)
            if recv == 'this':
                code = 'this->%s(%s)' % (name, args)
        if ret != want and _is_ref(want) and _is_ref(ret) and not p.is_sub_ct(ret, want):
            return 'jrt::cast<%s>(%s)' % (_cls_of(want), code), want
        if ret != want and want in _NUMERIC + ('bool',) and _is_ref(ret):
            return 'jrt::unbox<%s>(%s)' % (want, code), want
        return code, ret

    def x_New(self, e):
        p = self.p
        t = e.type
        if e.body is not None:
            return self.new_local(p.model.by_decl[id(e.body)], e)
        if t is None or t.kind != 'class':
            raise Decline('new of %r' % (t,))
        ci = p.model.info(t.name)
        if ci is not None and (ci.local or ci.anon):
            return self.new_local(ci, e)
        params, _r, decl = p.call_sig(e.sym) if e.sym is not None else ([], 'void', None)
        if ci is not None and decl is not None:
            params = [p.ct(x.type) for x in decl[1].params]
        args = self.args(e.args, params, e.sym)
        name = p.cls_name(t.name)
        if ci is not None and ci.needs_outer:
            outer = self.expr(e.outer)[0] if e.outer is not None else self.outer_of(ci.outer.qname)[0]
            if outer == 'this':
                outer = 'jrt::self(this)'
            return '[&] { auto o_ = std::make_shared<%s>(); o_->outer_ = %s; o_->ctor_(%s); return o_; }()' % (
                name, outer, args), 'ref<%s>' % name
        if ci is not None and not p.two_phase(ci):
            return 'std::make_shared<%s>(%s)' % (name, args), 'ref<%s>' % name
        return 'jrt::make<%s>(%s)' % (name, args), 'ref<%s>' % name

    def new_local(self, ci, e):
        """A local or anonymous class: made, its captured locals and outer instance set, then constructed."""
        p = self.p
        sets = []
        if ci.needs_outer:
            sets.append('o_->outer_ = %s;' % ('jrt::self(this)' if self.this_x() == 'this' else self.this_x()))
        for n, _t in ci.captures:
            code, _ct = self.x_Name(ir.Name(sym=ir.Sym('local', n), type=_t))
            sets.append('o_->cap_%s = %s;' % (_ident(n), code))
        params = []
        if e.sym is not None:
            _ps, _r, decl = p.call_sig(e.sym)
            params = [p.ct(x.type) for x in decl[1].params] if decl is not None else _ps
        args = self.args(e.args, params, e.sym)
        return '[&] { auto o_ = std::make_shared<%s>(); %s o_->ctor_(%s); return o_; }()' % (
            ci.cname, ' '.join(sets), args), 'ref<%s>' % ci.cname

    def x_NewArray(self, e):
        p = self.p
        ct = p.ct(e.type)
        elem = re.match(r'^ref<(?:jrt::)?Array<(.*)>>$', ct).group(1)
        if e.init is not None:
            items = []
            for x in e.init:
                if isinstance(x, ir.NewArray) and x.type is None:
                    x.type = e.type.elem
                code, xt = self.expr(x)
                items.append(self.conv(code, xt, elem))
            return 'jrt::array_of<%s>({%s})' % (elem, ', '.join(items)), ct
        dims = [self.conv(*(self.expr(d) + ('int32_t',))) for d in e.dims]
        if len(dims) == 1:
            return 'jrt::new_array<%s>(%s)' % (elem, dims[0]), ct
        if len(dims) == 2:
            inner = re.match(r'^ref<(?:jrt::)?Array<(.*)>>$', elem).group(1)
            return 'jrt::new_array2<%s>(%s, %s)' % (inner, dims[0], dims[1]), ct
        raise Decline('array of %d dimensions' % len(dims))

    def x_ClassLit(self, e):
        raise Decline('class literal')

    def x_SwitchExpr(self, e):
        """An immediately invoked lambda: each arm returns its value."""
        t = self.p.ct(e.type)
        if desugar._is_string(e.expr.type):
            return self.string_switch_expr(e, t)
        x, enum = self.switch_head(e.expr)
        lines = ['[&]() -> %s {' % t, '\tswitch (%s) {' % x]
        saved_ret, self.ret = self.ret, t
        try:
            for c in e.cases:
                for lab in c.labels:
                    lines.append('\tcase %s:' % self.case_label(lab, enum))
                if c.default:
                    lines.append('\tdefault:')
                lines.append('\t{')
                saved = dict(self.vars)
                for b in c.body:
                    self.stmt(b, lines, '\t\t')
                self.vars = saved
                lines.append('\t}')
        finally:
            self.ret = saved_ret
        lines += ['\t}', '\tjrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");', '}()']
        return '\n'.join(lines), t

    def string_switch_expr(self, e, t):
        """A switch expression on a String: the same lambda, its arms an if/else chain on equals()."""
        x, _xt = self.expr(cpp03._strip_paren(e.expr))
        lines = ['[&]() -> %s {' % t, '\tauto s_ = jrt::nn(%s);' % x]
        saved_ret, self.ret = self.ret, t
        default = None
        try:
            for c in e.cases:
                if c.default:
                    default = c
                    if not c.labels:
                        continue
                conds = ' || '.join('s_->equals(%s)' % self.expr(lab)[0] for lab in c.labels)
                lines.append('\tif (%s) {' % conds)
                saved = dict(self.vars)
                for b in c.body:
                    self.stmt(b, lines, '\t\t')
                self.vars = saved
                lines.append('\t}')
            if default is not None:
                lines.append('\t{')
                for b in default.body:
                    self.stmt(b, lines, '\t\t')
                lines.append('\t}')
        finally:
            self.ret = saved_ret
        lines += ['\tjrt::raise_new<jrt::IllegalStateException>(u"no switch arm matched");', '}()']
        return '\n'.join(lines), t

    # ---- lambdas and method references
    def functional(self, e, target=None):
        if isinstance(e, ir.Lambda):
            return self.x_Lambda(e)
        return self.x_MethodRef(e)

    def sam_of(self, t):
        if t is None or t.kind != 'class':
            raise Decline('lambda without a functional interface type')
        sam = self.p.model.sam(t.name)
        if sam is None:
            raise Decline('not a functional interface: %s' % t.name)
        name, ps, r = sam
        return name, [self.p.ct(jmodel.erase(x)) for x in ps], self.p.ct(jmodel.erase(r))

    def capture_list(self, inner_uses_self):
        if self.lambda_depth == 0 and inner_uses_self and not self.static:
            return '[=, self_ = jrt::self(this)]'
        return '[=]'

    def x_Lambda(self, e):
        p = self.p
        iface = p.cls_name(e.type.name)
        _name, ps, r = self.sam_of(e.type)
        saved = (dict(self.vars), self.ret, self.pending, self.uses_self, self.subst)
        self.lambda_depth += 1
        self.uses_self = False
        self.subst = {}
        params, entry = [], []
        for i, prm in enumerate(e.params):
            want = ps[i] if i < len(ps) else p.ct(prm.type)
            own = p.ct(prm.type)
            n = _ident(prm.sym.name)
            if own == want:
                params.append('%s %s' % (want, n))
            else:
                params.append('%s %s_in' % (want, n))
                entry.append('%s %s = %s;' % (own, n, self.conv(n + '_in', want, own)))
            self.vars[n] = own
        self.ret = r
        body = []
        try:
            if isinstance(e.body, ir.Block):
                self.stmts(e.body.stmts, body, '\t')
            else:
                self.stmt(ir.Return(expr=e.body) if r != 'void' else ir.ExprStmt(expr=e.body), body, '\t')
        finally:
            used = self.uses_self
            self.lambda_depth -= 1
            self.vars, self.ret, self.pending, self.uses_self, self.subst = saved
            self.uses_self = self.uses_self or (used and self.lambda_depth > 0)
        cap = self.capture_list(used)
        lines = ['jrt::fn<%s>(%s(%s) -> %s {' % (iface, cap, ', '.join(params), r)]
        lines += ['\t' + x for x in entry] + body + ['})']
        if len(lines) == 3 and not entry:                # a one-line body stays on one line
            return 'jrt::fn<%s>(%s(%s) -> %s { %s })' % (iface, cap, ', '.join(params), r, body[0].strip()), 'ref<%s>' % iface
        return '\n'.join(lines), 'ref<%s>' % iface

    def x_MethodRef(self, e):
        """A method reference as the lambda it stands for: Type::staticM, expr::m (the receiver evaluated
        once, where the reference is made), Type::instanceM (the first argument the receiver), Type::new."""
        p, s = self.p, e.sym
        iface = p.cls_name(e.type.name)
        _name, ps, r = self.sam_of(e.type)
        names = ['a%d' % i for i in range(len(ps))]
        params = ', '.join('%s %s' % (t, n) for t, n in zip(ps, names))
        cap = '[=]'
        if s is None:
            raise Decline('unresolved method reference')
        if e.mode == 'new':
            t = e.target.type if e.target is not None else None
            if t is not None and t.kind == 'array':
                call, ct = 'jrt::new_array<%s>(%s)' % (p.ct(t.elem), names[0]), p.ct(t)
            else:
                cps, _r, decl = p.call_sig(s)
                if decl is not None:
                    cps = [p.ct(x.type) for x in decl[1].params]
                argv = ', '.join(self.conv(n, pt, want) for n, pt, want in zip(names, ps, cps))
                ci = p.model.info(t.name) if t is not None else None
                name = p.cls_name(t.name)
                call = ('std::make_shared<%s>(%s)' if ci is not None and not p.two_phase(ci) else 'jrt::make<%s>(%s)') % (name, argv)
                ct = 'ref<%s>' % name
        else:
            cps, cr, _decl = p.call_sig(s)
            unbound = isinstance(e.target, ir.TypeName) and not s.static
            if s.static:
                ci = p.model.info(s.owner)
                holder = ci.cname if ci is not None else 'jrt::' + (_STATICS.get(s.owner) or _LIB.get(s.owner) or '?')
                argv = ', '.join(self.conv(n, pt, want) for n, pt, want in zip(names, ps, cps))
                call = '%s::%s(%s)' % (holder, _ident(s.name), argv)
            elif unbound:
                owner = 'ref<%s>' % p.cls_name(s.owner)
                recv = self.conv(names[0], ps[0], owner)
                argv = ', '.join(self.conv(n, pt, want) for n, pt, want in zip(names[1:], ps[1:], cps))
                call = 'jrt::nn(%s)->%s(%s)' % (recv, _ident(s.name), argv)
            else:
                rc, rct = self.expr(e.target) if not isinstance(e.target, (ir.This, ir.Super)) else (self.this_x(), '')
                if rc == 'this':
                    rc = 'jrt::self(this)'
                cap = '[=, r_ = %s]' % rc
                argv = ', '.join(self.conv(n, pt, want) for n, pt, want in zip(names, ps, cps))
                call = 'r_->%s(%s)' % (_ident(s.name), argv)
            ct = cr
        if r == 'void':
            return 'jrt::fn<%s>(%s(%s) -> void { %s; })' % (iface, cap, params, call), 'ref<%s>' % iface
        return 'jrt::fn<%s>(%s(%s) -> %s { return %s; })' % (iface, cap, params, r, self.conv(call, ct, r)), \
            'ref<%s>' % iface


_PURE_LIB = frozenset(('java.lang.String', 'java.lang.StringBuilder', 'java.lang.AbstractStringBuilder',
                       'java.io.PrintStream', 'java.lang.Math', 'java.lang.Integer', 'java.lang.Long',
                       'java.lang.Double', 'java.lang.Character', 'java.lang.Boolean', 'java.util.Objects'))


def _simple_finally(block):
    """A finally body that cannot jump and cannot throw (no calls into the program, no throw, no
    allocation): running it from a scope guard is the same as running it in place."""
    for n in ir.walk(block):
        if isinstance(n, (ir.Return, ir.Break, ir.Continue, ir.Throw, ir.New, ir.NewArray, ir.Index)):
            return False
        if isinstance(n, ir.Call):
            if n.sym is None or n.sym.owner not in ('java.io.PrintStream', 'java.lang.StringBuilder'):
                return False
        if isinstance(n, ir.Binary) and n.op in ('/', '%'):
            return False
    return True


def program(units, main=None):
    """The C++17 text of a whole program, or raise Decline."""
    return Program(units, main).write()
