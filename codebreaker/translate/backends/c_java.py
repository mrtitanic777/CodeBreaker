"""c_java.py -- C out for Java programs (docs/TRANSLATOR_DESIGN.md, phase 6), beside backends/c.py (which writes
the decompiler's C, and is left as it is).

A whole Java program from L2 as one C11 translation unit on the runtime codebreaker/translate/runtime/c
(jrt.h, jrt.c), in the faithful tier of the design's table:

  * a class is a struct whose first member is its superclass's struct (`B super_;`), so a pointer to it is a
    pointer to each base; its fields follow;
  * its virtual methods are a function table (`struct X_vt`): Object's three slots, then every slot of its
    program bases in the same order, then its own -- so a table is a prefix of each subclass's; a call goes
    through a small inline helper (X_m_v) that reads the table once;
  * an interface method is found through the class's interface tables (itabs), so a class implements any
    number of them; lambdas and method references are closure records -- a struct holding the captured
    values (Java captures only effectively-final locals: copies) -- implementing the functional interface;
  * a constructor is X_init_N(self, ...), run after allocation as the JVM runs it (fields zero first, so a
    virtual call in a constructor reaches the override); X_new_N allocates and initialises;
  * static state is globals; a class with a static initialiser runs it on first use (X_clinit);
  * exceptions are setjmp/longjmp through a handler chain (jrt_try); a return or jump out of a try pops it;
    locals of a function with a try are volatile (their values survive the longjmp);
  * generics are erased; a value read through a type variable is cast (checked) to what javac said;
  * Java's integer semantics: -fwrapv, masked shift counts, `/` and `%` by zero throw, (int) of a double
    saturates; Java's left-to-right operand order where C's is unspecified (cpp03's rule);
  * memory: objects are never freed while the program runs (jrt.h says why, and where a collector would go).

Modern constructs C has no word for are lowered first (translate/desugar.py: for-each, switch expressions,
switches on strings, instanceof patterns). What it cannot write raises Decline; the judge
(translate/javajudge.py) reports that program as refused.
"""
import os
import re
import subprocess

from codebreaker.translate import desugar, ir
from codebreaker.translate.backends import cpp03, jmodel
from codebreaker.translate.backends.cpp03 import Decline
from codebreaker.translate.backends.jmodel import LIB as _LIB, char_literal as _char, constant_init as _constant_init

RUNTIME = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'runtime', 'c')
CLANG = r'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang.exe'
FLAGS = ('-std=c11', '-fwrapv', '-O1', '-w')

_PRIM = {'bool': 'bool', 'i8': 'int8_t', 'i16': 'int16_t', 'u16': 'char16_t', 'i32': 'int32_t', 'i64': 'int64_t',
         'f32': 'float', 'f64': 'double', 'void': 'void'}
_CODE = {'bool': 'Z', 'i8': 'B', 'i16': 'S', 'u16': 'C', 'i32': 'I', 'i64': 'J', 'f32': 'F', 'f64': 'D'}
_NUMERIC = ('int8_t', 'int16_t', 'char16_t', 'int32_t', 'int64_t', 'float', 'double')
_INTEGRAL = ('int8_t', 'int16_t', 'char16_t', 'int32_t', 'int64_t')
_BOX = {'java.lang.Integer': 'I', 'java.lang.Long': 'J', 'java.lang.Short': 'S', 'java.lang.Byte': 'B',
        'java.lang.Character': 'C', 'java.lang.Boolean': 'Z', 'java.lang.Float': 'F', 'java.lang.Double': 'D'}
_CT_OF_CODE = {'Z': 'bool', 'B': 'int8_t', 'S': 'int16_t', 'C': 'char16_t', 'I': 'int32_t', 'J': 'int64_t',
               'F': 'float', 'D': 'double'}
_OBJ = 'jobject*'
_STR = 'jstring*'
_ARR = 'jarray*'
_SB = 'jrt_StringBuilder*'
_STATIC_FIELDS = {('java.lang.Integer', 'MAX_VALUE'): ('JRT_INT_MAX', 'int32_t'),
                  ('java.lang.Integer', 'MIN_VALUE'): ('JRT_INT_MIN', 'int32_t'),
                  ('java.lang.Long', 'MAX_VALUE'): ('JRT_LONG_MAX', 'int64_t'),
                  ('java.lang.Long', 'MIN_VALUE'): ('JRT_LONG_MIN', 'int64_t'),
                  ('java.lang.Math', 'PI'): ('JRT_PI', 'double'), ('java.lang.Math', 'E'): ('JRT_E', 'double'),
                  ('java.lang.Double', 'NaN'): ('NAN', 'double'),
                  ('java.lang.Double', 'POSITIVE_INFINITY'): ('INFINITY', 'double'),
                  ('java.lang.Double', 'NEGATIVE_INFINITY'): ('(-INFINITY)', 'double'),
                  ('java.lang.System', 'out'): ('jrt_System_out', 'void*'),
                  ('java.lang.System', 'err'): ('jrt_System_err', 'void*')}
_KEYWORDS = frozenset('''auto break case char const continue default do double else enum extern float for goto if
inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void
volatile while bool true false self self_ NULL errno stdin stdout stderr EOF jrt'''.split())
_THROWABLES = set(q for q in jmodel.LIB_SUPERS if q.endswith(('Exception', 'Error')))


def _ident(n):
    return n + '_' if n in _KEYWORDS else n.replace('$', '_')


def _u16(v):
    """(C char16_t literal text, length) of a Java string."""
    out, prev_hex = [], False
    for ch in v:
        o = ord(ch)
        if ch in '\\"':
            out.append('\\' + ch)
            prev_hex = False
        elif ch == '\n':
            out.append('\\n')
            prev_hex = False
        elif 32 <= o < 127:
            out.append(('" u"' + ch) if prev_hex and ch in '0123456789abcdefABCDEF' else ch)
            prev_hex = False
        else:
            out.append('\\x%04x' % o)
            prev_hex = True
    return 'u"%s"' % ''.join(out), len(v)


def _code(t):
    """A Java type's letter(s) in a runtime function's name: I J ... Str, O, and arrays as the element's + A."""
    if t is None:
        return ''
    if t.kind in _CODE:
        return _CODE[t.kind]
    if t.kind == 'array':
        e = t.elem
        return (_CODE.get(e.kind, 'O') if e is not None else 'O') + 'A'
    if t.kind == 'class' and t.name == 'java.lang.String':
        return 'Str'
    return 'O'


class Program(object):
    def __init__(self, units, main=None):
        desugar.run(units)
        self.units = units
        self.model = jmodel.Model(units)
        self.model.name_anonymous()
        self.main = main
        self.lits = {}                            # literal text -> slot name
        self.lambdas = []                         # closure records: lines of C
        self.nlambda = 0
        self.vfuncs = []                          # the virtual-call helpers, after the tables
        sigs = {}
        for ci in self.model.classes.values():
            for m in ci.methods():
                if m.kind == 'method':
                    sigs.setdefault(m.name, set()).add(tuple(_code(jmodel.erase(p.type)) for p in m.params))
        self.overloaded = set(n for n, s in sigs.items() if len(s) > 1)

    # ---- names and types
    def cname(self, qname):
        ci = self.model.info(qname)
        if ci is None:
            raise Decline('not a class of the program: %s' % qname)
        return ci.cname

    def mname(self, name, params):
        """A method's name in C: its own, or with its parameter codes when the program overloads it."""
        base = _ident(name)
        if name in self.overloaded:
            return base + '_' + ''.join(_code(jmodel.erase(p)) for p in params)
        return base

    def ct(self, t):
        if t is None:
            return 'void'
        if t.kind in _PRIM:
            return _PRIM[t.kind]
        if t.kind == 'null':
            return 'nullptr_t'
        if t.kind in ('typevar', 'wildcard', 'any'):
            return _OBJ
        if t.kind == 'array':
            return _ARR
        if t.kind == 'class':
            if t.name == 'java.lang.String':
                return _STR
            if t.name in ('java.lang.StringBuilder', 'java.lang.StringBuffer'):
                return _SB
            ci = self.model.info(t.name)
            if ci is not None:
                return _OBJ if ci.kind == 'interface' else ci.cname + '*'
            if t.name == 'java.io.PrintStream':
                return 'void*'
            return _OBJ
        raise Decline('type %r' % (t,))

    def class_ref(self, qname):
        """&X_class of a class or interface, program or runtime."""
        ci = self.model.info(qname)
        if ci is not None:
            return '&%s_class' % ci.cname
        if qname == 'java.lang.String':
            return '&jrt_String_class'
        name = _LIB.get(qname)
        if name in (None, 'Object') and qname != jmodel.OBJECT:
            if qname in ('java.lang.CharSequence',):
                return '&jrt_CharSequence_class'
            raise Decline('runtime class %s' % qname)
        if qname in ('java.util.ArrayList', 'java.util.List', 'java.util.LinkedList'):
            return '&jrt_ArrayList_class'
        if qname in ('java.util.HashMap', 'java.util.Map', 'java.util.LinkedHashMap', 'java.util.TreeMap'):
            return '&jrt_HashMap_class'
        if qname in ('java.util.Map.Entry',):
            return '&jrt_node_class'
        return '&jrt_%s_class' % name

    def sig(self, ci, m):
        root = self.model.overrides(ci, m)
        params = root[0] if root else [p.type for p in m.params]
        ret = root[1] if root else m.ret
        return [self.ct(jmodel.erase(p)) for p in params], (self.ct(jmodel.erase(ret)) if m.kind == 'method' else 'void'), \
            [jmodel.erase(p) for p in params]

    def base_struct(self, ci):
        if ci.kind == 'enum':
            return 'jrt_Enum'
        if ci.base and ci.base != jmodel.OBJECT:
            b = self.model.info(ci.base)
            if b is not None:
                return b.cname
            if ci.base in _THROWABLES or ci.base == 'java.lang.Throwable':
                return 'jrt_Throwable'
            if ci.base == 'java.lang.Enum':
                return 'jrt_Enum'
            raise Decline('extends the runtime class %s' % ci.base)
        return None

    def base_class_ref(self, ci):
        if ci.kind == 'enum':
            return '&jrt_Enum_class'
        if ci.base and ci.base != jmodel.OBJECT:
            return self.class_ref(ci.base)
        return '&jrt_Object_class'

    def is_throwable(self, ci):
        q = ci.base
        while q:
            if q in _THROWABLES or q == 'java.lang.Throwable':
                return True
            b = self.model.info(q)
            q = b.base if b is not None else None
        return False

    def lit(self, v):
        if v not in self.lits:
            self.lits[v] = 'lit_%d' % (len(self.lits) + 1)
        text, n = _u16(v)
        return 'jrt_lit(&%s, %s, %d)' % (self.lits[v], text, n)

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

    # ---- the table of virtual slots
    def slots(self, ci):
        """[(slot name, C params after self, C result, implementing function)] of class ci, base slots first."""
        if getattr(ci, '_slots', None) is not None:
            return ci._slots
        out = []
        b = self.model.info(ci.base) if ci.base else None
        if b is not None and b.kind != 'interface':
            out = [list(s) for s in self.slots(b)]
        elif ci.kind != 'interface' and self.is_throwable(ci):
            out = [['getMessage', [], _STR, 'jrt_Throwable_getMessage']]
        for m in ci.methods():
            if m.kind != 'method' or (m.sym is not None and m.sym.static) or 'private' in (m.mods or []):
                continue
            if m.name in ('toString', 'equals', 'hashCode') and len(m.params) == (1 if m.name == 'equals' else 0):
                continue                                      # Object's slots, in the jobject_vt part
            ps, r, eps = self.sig(ci, m)
            name = self.mname(m.name, eps)
            impl = '%s_%s' % (ci.cname, name) if m.body is not None else '0'
            for s in out:
                if s[0] == name:
                    s[3] = impl if m.body is not None else s[3]
                    break
            else:
                out.append([name, ps, r, impl])
        ci._slots = out
        return out

    def object_slots(self, ci):
        """The implementations of toString, equals, hashCode for class ci."""
        impl = {'toString': 'jrt_Object_toString', 'equals': 'jrt_Object_equals', 'hashCode': 'jrt_Object_hashCode'}
        chain = []
        q = ci.qname
        while q:
            c = self.model.info(q)
            if c is None:
                if q in _THROWABLES or q == 'java.lang.Throwable':
                    impl['toString'] = 'jrt_Throwable_toString'
                if q == 'java.lang.Enum':
                    impl['toString'] = 'jrt_Enum_toString'
                break
            chain.append(c)
            q = c.base if c.kind != 'enum' else 'java.lang.Enum'
        for c in reversed(chain):
            for m in c.methods():
                if m.name in impl and m.body is not None and m.kind == 'method' and \
                        len(m.params) == (1 if m.name == 'equals' else 0):
                    impl[m.name] = '%s_%s' % (c.cname, m.name)
        return impl

    def iface_methods(self, qname):
        """[(slot name, C params, C result, java name, erased params)] of an interface, its super-interfaces' first."""
        ci = self.model.info(qname)
        if ci is None:
            sam = jmodel.SAMS.get(qname)
            if sam is not None:
                name, ps, r = sam
                return [(name, [self.ct(jmodel.erase(p)) for p in ps], self.ct(jmodel.erase(r)), name, ps)]
            if qname == 'java.lang.Iterable':
                return [('iterator', [], _OBJ, 'iterator', [])]
            if qname == 'java.util.Iterator':
                return [('hasNext', [], 'bool', 'hasNext', []), ('next', [], _OBJ, 'next', [])]
            if qname == 'java.lang.Comparable':
                return [('compareTo', [_OBJ], 'int32_t', 'compareTo', [ir.Type('typevar', 'T')])]
            if qname == 'java.lang.CharSequence':
                return []
            raise Decline('interface %s' % qname)
        out = []
        for q in ci.ifaces:
            for x in self.iface_methods(q):
                if x[0] not in [y[0] for y in out]:
                    out.append(x)
        for m in ci.methods():
            if m.kind == 'method' and not (m.sym is not None and m.sym.static):
                if m.name in ('toString', 'equals', 'hashCode'):
                    continue
                ps, r, eps = self.sig(ci, m)
                name = self.mname(m.name, eps)
                if name not in [y[0] for y in out]:
                    out.append((name, ps, r, m.name, eps))
        return out

    def all_ifaces(self, ci):
        out = []
        for q in self.model.ancestors(ci.qname)[1:]:
            if self.model.is_interface(q) and q not in out and (self.model.info(q) is not None or q in jmodel.SAMS
                                                                 or q in ('java.lang.Iterable', 'java.util.Iterator',
                                                                          'java.lang.Comparable')):
                out.append(q)
        return out

    def vt_name(self, qname):
        ci = self.model.info(qname)
        if ci is not None:
            return 'struct %s_vt' % ci.cname
        return 'jrt_%s_vt' % qname.rsplit('.', 1)[-1]

    def find_impl(self, ci, java_name, eps):
        """The function implementing an interface method in class ci (its own or a base's), or None."""
        q = ci.qname
        while q:
            c = self.model.info(q)
            if c is None:
                break
            for m in c.methods():
                if m.kind == 'method' and m.name == java_name and m.body is not None and len(m.params) == len(eps) \
                        and not (m.sym is not None and m.sym.static):
                    _ps, _r, meps = self.sig(c, m)
                    return '%s_%s' % (c.cname, self.mname(m.name, meps))
            q = c.base
        for q in self.model.ancestors(ci.qname):                # a default method of an interface
            c = self.model.info(q)
            if c is None or c.kind != 'interface':
                continue
            for m in c.methods():
                if m.kind == 'method' and m.name == java_name and m.body is not None and len(m.params) == len(eps) \
                        and not (m.sym is not None and m.sym.static):
                    _ps, _r, meps = self.sig(c, m)
                    return '%s_%s' % (c.cname, self.mname(m.name, meps))
        return None

    # ---- the program
    def ordered(self):
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

    def write(self):
        order = self.ordered()
        structs, protos, tables, defs = [], [], [], []
        for ci in order:
            structs += self.struct(ci)
        for ci in order:
            protos += self.prototypes(ci)
        for ci in order:
            defs += self.definitions(ci)
        for ci in order:
            tables += self.tables(ci)
        head = ['/* written by CodeBreaker\'s C back end for Java (codebreaker/translate/backends/c_java.py) from',
                '   the program\'s L2; it runs on jrt.h / jrt.c, the Java library in C. */',
                '#include "jrt.h"', '']
        head += ['typedef struct %s %s;' % (ci.cname, ci.cname) for ci in order if ci.kind != 'interface']
        head += ['extern const jclass %s_class;' % ci.cname for ci in order]
        head.append('')
        lits = ['static jstring* %s;' % slot for slot in self.lits.values()]
        mains = [ci for ci in order if any(m.name == 'main' and m.sym is not None and m.sym.static and len(m.params) == 1
                                           for m in ci.methods())]
        tail = ['', 'int main(int argc, char** argv) {']
        if mains:
            tail.append('\tconst char* which = getenv("JRT_MAIN");')
            for ci in mains:
                tail.append('\tif (which && strcmp(which, "%s") == 0) return jrt_run_main(%s_main, argc, argv);'
                            % (ci.qname.rsplit('.', 1)[-1], ci.cname))
            tail.append('\treturn jrt_run_main(%s_main, argc, argv);' % (self.main or mains[0].cname))
        else:
            tail.append('\treturn 0;')
        tail.append('}')
        return '\n'.join(head + structs + lits + [''] + protos + self.vfuncs + self.lambdas + defs + tables + tail) + '\n'

    def struct(self, ci):
        L = []
        if ci.kind == 'interface':
            ms = self.iface_methods(ci.qname)
            L.append('struct %s_vt {' % ci.cname)
            L.append('\tjobject_vt o_;')
            for name, ps, r, _j, _e in ms:
                L.append('\t%s (*%s)(%s);' % (r, name, ', '.join([_OBJ] + ps)))
            L.append('};')
            L.append('')
            return L
        base = self.base_struct(ci)
        L.append('struct %s {' % ci.cname)
        L.append('\t%s super_;' % base if base else '\tjobject hdr_;')
        if ci.needs_outer:
            L.append('\t%s outer_;' % self.ct(ir.Type('class', ci.outer.qname)))
        for name, t in ci.captures:
            L.append('\t%s cap_%s;' % (self.ct(t), _ident(name)))
        for f in ci.fields():
            if f.sym is not None and not f.sym.static:
                L.append('\t%s %s;' % (self.ct(f.type), _ident(f.sym.name)))
        if len(L) == 2 and not base:
            pass
        L.append('};')
        # the table type: Object's slots, then every slot of the class and its bases, in order
        L.append('struct %s_vt {' % ci.cname)
        L.append('\tjobject_vt o_;')
        for name, ps, r, _impl in self.slots(ci):
            L.append('\t%s (*%s)(%s);' % (r, name, ', '.join([_OBJ] + ps)))
        L.append('};')
        for f in ci.fields():
            if f.sym is not None and f.sym.static:
                init = Method(self, ci, None).constant(f) if _constant_init(f) else ('0' if f.type.is_prim else 'NULL')
                L.append('static %s %s_%s = %s;' % (self.ct(f.type), ci.cname, _ident(f.sym.name), init))
        for x in ci.members:
            if isinstance(x, ir.EnumConst):
                L.append('static %s* %s_%s;' % (ci.cname, ci.cname, _ident(x.sym.name)))
        if ci.kind == 'enum':
            L.append('static jarray* %s_values_;' % ci.cname)
        if self.needs_clinit(ci):
            L.append('static bool %s_inited_;' % ci.cname)
        L.append('')
        return L

    def ctor_name(self, ci, m):
        k = [x for x in ci.methods() if x.kind == 'ctor']
        if len(k) <= 1:
            return '%s_init' % ci.cname
        return '%s_init_%s' % (ci.cname, ''.join(_code(jmodel.erase(p.type)) for p in m.params) or 'V')

    def ctor_params(self, ci, m):
        ps = ['%s* self' % ci.cname]
        if ci.kind == 'enum':
            ps += ['jstring* name_in', 'int32_t ordinal_in']
        ps += ['%s %s' % (self.ct(p.type), _ident(p.sym.name)) for p in m.params]
        return ps

    def new_params(self, ci, m):
        ps = []
        if ci.needs_outer:
            ps.append('%s outer_in' % self.ct(ir.Type('class', ci.outer.qname)))
        for n, t in ci.captures:
            ps.append('%s cap_%s' % (self.ct(t), _ident(n)))
        if ci.kind == 'enum':
            ps += ['jstring* name_in', 'int32_t ordinal_in']
        ps += ['%s %s' % (self.ct(p.type), _ident(p.sym.name)) for p in (m.params if m is not None else [])]
        return ps

    def prototypes(self, ci):
        L = []
        if ci.kind == 'interface':
            for m in ci.methods():
                if m.body is not None:                        # static and default methods
                    ps, r, eps = self.sig(ci, m)
                    static = m.sym is not None and m.sym.static
                    L.append('static %s %s_%s(%s);' % (r, ci.cname, self.mname(m.name, eps),
                                                       ', '.join(([] if static else [_OBJ]) + ps) or 'void'))
            for name, ps, r, j, eps in self.iface_methods(ci.qname):
                args = ', '.join('a%d' % i for i in range(len(ps)))
                params = ', '.join(['jobject* o'] + ['%s a%d' % (t, i) for i, t in enumerate(ps)])
                self.vfuncs.append('static inline %s %s_%s_i(%s) { return ((const struct %s_vt*)jrt_ivt(o, &%s_class))->%s(%s); }'
                                   % (r, ci.cname, name, params, ci.cname, ci.cname, name, ', '.join(['o'] + (args.split(', ') if args else []))))
            return L
        for m in ci.methods():
            if m.kind == 'ctor':
                L.append('static void %s(%s);' % (self.ctor_name(ci, m), ', '.join(self.ctor_params(ci, m))))
                L.append('static %s* %s(%s);' % (ci.cname, self.ctor_name(ci, m).replace('_init', '_new', 1),
                                                 ', '.join(self.new_params(ci, m)) or 'void'))
                continue
            if m.body is None:
                continue
            ps, r, eps = self.sig(ci, m)
            name = '%s_%s' % (ci.cname, self.mname(m.name, eps))
            if m.sym is not None and m.sym.static:
                L.append('static %s %s(%s);' % (r, name, ', '.join(ps) or 'void'))
            elif m.name in ('toString', 'equals', 'hashCode') and len(m.params) == (1 if m.name == 'equals' else 0):
                r = {'toString': _STR, 'equals': 'bool', 'hashCode': 'int32_t'}[m.name]
                L.append('static %s %s(%s);' % (r, '%s_%s' % (ci.cname, m.name), ', '.join([_OBJ] + ps)))
            else:
                L.append('static %s %s(%s);' % (r, name, ', '.join([_OBJ] + ps)))
        if not any(m.kind == 'ctor' for m in ci.methods()):
            L.append('static %s* %s_new(%s);' % (ci.cname, ci.cname, ', '.join(self.new_params(ci, None)) or 'void'))
        L.append('static void %s_finit(%s* self);' % (ci.cname, ci.cname))
        if self.needs_clinit(ci):
            L.append('static void %s_clinit(void);' % ci.cname)
        if ci.kind == 'enum':
            L.append('static jarray* %s_values(void);' % ci.cname)
        for name, ps, r, _impl in self.slots(ci):
            if any(s[0] == name for s in (self.slots(self.model.info(ci.base)) if self.model.info(ci.base) is not None
                                          and self.model.info(ci.base).kind != 'interface' else [])):
                continue                                      # the base's helper serves
            args = ', '.join(['o'] + ['a%d' % i for i in range(len(ps))])
            params = ', '.join(['jobject* o'] + ['%s a%d' % (t, i) for i, t in enumerate(ps)])
            self.vfuncs.append('static inline %s %s_%s_v(%s) { return ((const struct %s_vt*)jrt_vt(o))->%s(%s); }'
                               % (r, ci.cname, name, params, ci.cname, name, args))
        return L

    def definitions(self, ci):
        L = []
        if ci.kind == 'interface':
            for m in ci.methods():
                if m.body is not None:
                    L += Method(self, ci, m).define()
            return L
        for m in ci.methods():
            if m.body is None:
                continue
            L += Method(self, ci, m).define()
        if not any(m.kind == 'ctor' for m in ci.methods()):
            L += Method(self, ci, None).anon_new()
        L += Method(self, ci, None).finit()
        if self.needs_clinit(ci):
            L += Method(self, ci, None).clinit()
        if ci.kind == 'enum':
            L += ['static jarray* %s_values(void) {' % ci.cname, '\t%s_clinit();' % ci.cname,
                  '\treturn jrt_array_clone(%s_values_);' % ci.cname, '}', '']
        return L

    def tables(self, ci):
        L = []
        if ci.kind == 'interface':
            L.append('const jclass %s_class = {"%s", 0, 0, 0, 0, 0, 1};' % (ci.cname, ci.qname.strip('<>')))
            L.append('')
            return L
        obj = self.object_slots(ci)
        objs = '{%s, %s, %s}' % (obj['toString'], obj['equals'], obj['hashCode'])
        slots = self.slots(ci)
        L.append('static const struct %s_vt %s_vt_ = {%s%s};' % (
            ci.cname, ci.cname, objs, ''.join(', (%s (*)(%s))%s' % (r, ', '.join([_OBJ] + ps), impl)
                                             for _n, ps, r, impl in slots)))
        itabs = []
        for q in self.all_ifaces(ci):
            ms = self.iface_methods(q)
            if q == 'java.lang.Comparable' and self.find_impl(ci, 'compareTo', [None]) is None:
                continue                                      # an enum's: jrt_natural compares by ordinal
            impls = []
            for name, ps, r, j, eps in ms:
                impl = self.find_impl(ci, j, eps)
                if impl is None and 'abstract' in (ci.decl.mods or []):
                    impl = '0'                                # never reached: an instance is of a subclass,
                if impl is None:                              # whose own table is found first
                    raise Decline('%s does not implement %s.%s' % (ci.cname, q, j))
                impls.append('(%s (*)(%s))%s' % (r, ', '.join([_OBJ] + ps), impl))
            vt = 'struct %s_vt' % self.model.info(q).cname if self.model.info(q) is not None else 'jrt_%s_vt' % q.rsplit('.', 1)[-1]
            name = '%s_as_%s' % (ci.cname, q.rsplit('.', 1)[-1].replace('<', '').replace('>', ''))
            L.append('static const %s %s = {%s%s};' % (vt, name, objs, ''.join(', ' + x for x in impls)))
            itabs.append('{%s, &%s}' % (self.class_ref(q), name))
        if itabs:
            L.append('static const jitab %s_itabs[] = {%s};' % (ci.cname, ', '.join(itabs)))
        L.append('const jclass %s_class = {"%s", %s, sizeof(%s), &%s_vt_, %s, 0};' % (
            ci.cname, ci.qname.strip('<>'), self.base_class_ref(ci), ci.cname, ci.cname,
            ('JRT_ITABS(%s_itabs)' % ci.cname) if itabs else '0, 0'))
        L.append('')
        return L


class Method(object):
    """One function's C text."""

    def __init__(self, prog, ci, m, lam=None):
        self.p = prog
        self.ci = ci
        self.m = m
        self.vars = {}
        self.ret = 'void'
        self.pending = []
        self.temps = 0
        self.order = False
        self.subst = {}
        self.cont_next = None
        self.tries = []                           # the handlers open around the statement: (name, loops)
        self.loops = 0
        self.vol = False                          # locals volatile (the function has a try)
        self.captures = dict((n, 'self->cap_' + _ident(n)) for n, _t in ci.captures) if ci is not None else {}
        self.static = m is not None and m.sym is not None and m.sym.static
        self.lam = lam                            # in a closure: {name: field} of what it captured, and 'this'
        self.self_x = 'self'

    # ---- members
    def define(self):
        p, ci, m = self.p, self.ci, self.m
        self.vol = any(isinstance(n, ir.Try) for n in ir.walk(m.body))
        L = []
        if m.kind == 'ctor':
            name = p.ctor_name(ci, m)
            L.append('static void %s(%s) {' % (name, ', '.join(p.ctor_params(ci, m))))
            for prm in m.params:
                self.vars[_ident(prm.sym.name)] = p.ct(prm.type)
            stmts = list(m.body.stmts)
            first = stmts[0] if stmts and isinstance(stmts[0], ir.CtorCall) else None
            if first is not None:
                stmts = stmts[1:]
            delegated = first is not None and first.kind.startswith('this')
            if ci.kind == 'enum':
                L.append('\t((jrt_Enum*)self)->name = name_in;')
                L.append('\t((jrt_Enum*)self)->ordinal = ordinal_in;')
            if first is not None and (not first.kind.endswith(':implicit') or delegated):
                L += self.ctor_call(first, '\t')
            elif ci.base and ci.base != jmodel.OBJECT and ci.kind != 'enum':
                b = p.model.info(ci.base)
                if b is not None:
                    bm = next((x for x in b.methods() if x.kind == 'ctor' and not x.params), None)
                    if bm is not None:
                        L.append('\t%s((%s*)self);' % (p.ctor_name(b, bm), b.cname))
            if not delegated:
                L.append('\t%s_finit(self);' % ci.cname)
            out = []
            self.stmts(stmts, out, '\t')
            L += out + ['}', '']
            newname = name.replace('_init', '_new', 1)
            L.append('static %s* %s(%s) {' % (ci.cname, newname, ', '.join(p.new_params(ci, m)) or 'void'))
            if p.needs_clinit(ci):
                L.append('\t%s_clinit();' % ci.cname)
            L.append('\t%s* o = (%s*)jrt_alloc(&%s_class);' % (ci.cname, ci.cname, ci.cname))
            if ci.needs_outer:
                L.append('\to->outer_ = outer_in;')
            for n, _t in ci.captures:
                L.append('\to->cap_%s = cap_%s;' % (_ident(n), _ident(n)))
            args = (['name_in', 'ordinal_in'] if ci.kind == 'enum' else []) + [_ident(x.sym.name) for x in m.params]
            L.append('\t%s(%s);' % (name, ', '.join(['o'] + args)))
            L.append('\treturn o;')
            L += ['}', '']
            return L
        ps, r, eps = p.sig(ci, m)
        self.ret = r
        static = m.sym is not None and m.sym.static
        params, entry = [], []
        if not static:
            params.append('jobject* self_')
            entry.append('\t%s* self = (%s*)self_;' % (ci.cname, ci.cname) if ci.kind != 'interface' else '\tjobject* self = self_;')
        for t, prm in zip(ps, m.params):
            own = p.ct(prm.type)
            n = _ident(prm.sym.name)
            self.vars[n] = own
            if own != t:
                params.append('%s %s_in' % (t, n))
                entry.append('\t%s %s = %s;' % (own, n, self.conv(n + '_in', t, own)))
            else:
                params.append('%s %s' % (t, n))
        if m.name in ('toString', 'equals', 'hashCode') and not static and len(m.params) == (1 if m.name == 'equals' else 0):
            fname = '%s_%s' % (ci.cname, m.name)
        else:
            fname = '%s_%s' % (ci.cname, p.mname(m.name, eps))
        L.append('static %s %s(%s) {' % (r, fname, ', '.join(params) or 'void'))
        if static and p.needs_clinit(ci):
            L.append('\t%s_clinit();' % ci.cname)
        L += entry
        out = []
        self.stmts(m.body.stmts, out, '\t')
        L += out + ['}', '']
        return L

    def ctor_call(self, call, ind):
        p, ci = self.p, self.ci
        if call.kind.startswith('this'):
            _c, tm = p.model.method_decl(call.sym)
            if tm is None:
                raise Decline('this(...) target')
            args = self.args(call.args, [p.ct(x.type) for x in tm.params], call.sym)
            extra = ['name_in', 'ordinal_in'] if ci.kind == 'enum' else []
            return [ind + '%s(%s);' % (p.ctor_name(ci, tm), ', '.join(['self'] + extra + ([args] if args else [])))]
        if ci.base in (None, jmodel.OBJECT):
            return []
        b = p.model.info(ci.base)
        if b is None:                                         # a runtime Throwable
            args = [self.conv(*(self.expr(a) + (_STR if i == 0 else _OBJ,))) for i, a in enumerate(call.args or [])]
            if len(args) == 1 and call.args[0].type is not None and call.args[0].type.name != 'java.lang.String':
                return [ind + 'jrt_Throwable_init_cause((jobject*)self, NULL, %s);' % args[0]]
            if len(args) == 2:
                return [ind + 'jrt_Throwable_init_cause((jobject*)self, %s, %s);' % tuple(args)]
            return [ind + 'jrt_Throwable_init((jobject*)self, %s);' % (args[0] if args else 'NULL')]
        _bc, bm = p.model.method_decl(call.sym)
        if bm is None:
            raise Decline('super(...) target')
        args = self.args(call.args, [p.ct(x.type) for x in bm.params], call.sym)
        return [ind + '%s(%s);' % (p.ctor_name(b, bm), ', '.join(['(%s*)self' % b.cname] + ([args] if args else [])))]

    def anon_new(self):
        """X_new for a class with no constructor of its own (an anonymous class): the super's constructor with
        the arguments, then the field initialisers."""
        p, ci = self.p, self.ci
        sup = None
        b = p.model.info(ci.base) if ci.base else None
        n = getattr(ci, 'new', None)
        params = p.new_params(ci, None)
        sargs = []
        if b is not None and b.kind != 'interface' and n is not None and n.sym is not None:
            _bc, bm = p.model.method_decl(n.sym)
            if bm is not None:
                for i, x in enumerate(bm.params):
                    params.append('%s a%d' % (p.ct(x.type), i))
                    sargs.append('a%d' % i)
                sup = '\t%s((%s*)o%s);' % (p.ctor_name(b, bm), b.cname, ''.join(', ' + a for a in sargs))
        L = ['static %s* %s_new(%s) {' % (ci.cname, ci.cname, ', '.join(params) or 'void')]
        if p.needs_clinit(ci):
            L.append('\t%s_clinit();' % ci.cname)
        L.append('\t%s* o = (%s*)jrt_alloc(&%s_class);' % (ci.cname, ci.cname, ci.cname))
        if ci.needs_outer:
            L.append('\to->outer_ = outer_in;')
        for nm, _t in ci.captures:
            L.append('\to->cap_%s = cap_%s;' % (_ident(nm), _ident(nm)))
        if sup:
            L.append(sup)
        L.append('\t%s_finit(o);' % ci.cname)
        L += ['\treturn o;', '}', '']
        return L

    def finit(self):
        out = []
        for x in self.ci.members:
            if isinstance(x, ir.FieldDecl) and x.init is not None and x.sym is not None and not x.sym.static:
                self.stmt(ir.ExprStmt(expr=ir.Assign(op=None, target=ir.Select(obj=ir.This(qualifier=None, type=None),
                                                                                sym=x.sym, type=x.type),
                                                     value=x.init, overflow=None, type=x.type)), out, '\t')
            elif isinstance(x, ir.MethodDecl) and x.kind == 'init':
                self.stmts(x.body.stmts, out, '\t')
        return ['static void %s_finit(%s* self) {' % (self.ci.cname, self.ci.cname)] + out + ['}', '']

    def clinit(self):
        p, ci = self.p, self.ci
        self.static = True
        L = ['static void %s_clinit(void) {' % ci.cname, '\tif (%s_inited_) return;' % ci.cname,
             '\t%s_inited_ = true;' % ci.cname]
        b = p.model.info(ci.base) if ci.base else None
        if b is not None and p.needs_clinit(b):
            L.append('\t%s_clinit();' % b.cname)
        consts = [x for x in ci.members if isinstance(x, ir.EnumConst)]
        for k, x in enumerate(consts):
            sub = p.model.by_decl.get(id(x.body)) if x.body is not None else None
            target = sub if sub is not None else ci
            cm = None
            if x.ctor is not None:
                _cc, cm = p.model.method_decl(x.ctor)
            args = self.args(x.args or [], [p.ct(q.type) for q in cm.params] if cm is not None else [], x.ctor)
            if sub is not None:
                L.append('\t%s_%s = (%s*)%s_new(%s);' % (ci.cname, _ident(x.sym.name), ci.cname, target.cname,
                                                       ', '.join([p.lit(x.sym.name), str(k)] + ([args] if args else []))))
            else:
                ctor = p.ctor_name(ci, cm).replace('_init', '_new', 1) if cm is not None else '%s_new' % ci.cname
                L.append('\t%s_%s = %s(%s);' % (ci.cname, _ident(x.sym.name), ctor,
                                                ', '.join([p.lit(x.sym.name), str(k)] + ([args] if args else []))))
        if ci.kind == 'enum':
            L.append('\t%s_values_ = jrt_new_array(sizeof(jobject*), %d);' % (ci.cname, len(consts)))
            for k, x in enumerate(consts):
                L.append('\tJA(jobject*, %s_values_, %d) = (jobject*)%s_%s;' % (ci.cname, k, ci.cname, _ident(x.sym.name)))
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
        self.loops += 1
        try:
            if lab is None:
                self.block(body, out, ind)
                return
            out.append(ind + '\t{')
            self.block(body, out, ind + '\t')
            out.append(ind + '\t}')
            out.append(ind + '\t%s_cont:;' % lab)
        finally:
            self.loops -= 1

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
        code, ct = self.expr(cpp03._strip_paren(e))
        return self.conv(code, ct, 'bool')

    def decl(self, ct, name, init=None):
        if self.vol:
            q = '%s volatile' % ct if ct.endswith('*') else 'volatile %s' % ct
        else:
            q = ct
        return '%s %s%s;' % (q, name, ' = %s' % init if init is not None else (' = 0' if ct in _NUMERIC + ('bool',)
                                                                               else ' = NULL'))

    def pops(self, depth):
        """jrt_pop for each handler opened since `depth` (innermost first)."""
        return ''.join('jrt_pop(&%s); ' % t for t, _l in reversed(self.tries[depth:]))

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
                out.append(ind + self.decl(ct, name))
                return
            code, it = self.expr(s.init)
            out.append(ind + self.decl(ct, name, self.conv(code, it, ct)))
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
            out.append(ind + '{')
            inner = ind + '\t'
            for i in s.init:
                self.stmt(i, out, inner)
            cond = self.cond(s.cond) if s.cond is not None else '1'
            upd = ', '.join(self.expr(u)[0] for u in s.update)
            out.append(inner + 'for (; %s; %s) {' % (cond, upd))
            self.loop_body(s.body, out, inner)
            out.append(inner + '}')
            out.append(ind + '}')
            self.vars = saved
        elif isinstance(s, ir.Labeled):
            lab = s.label
            used = [n for n in ir.walk(s.body) if isinstance(n, (ir.Break, ir.Continue)) and n.label == lab]
            if any(isinstance(n, ir.Continue) for n in used):
                self.cont_next = lab
            self.stmt(s.body, out, ind)
            if any(isinstance(n, ir.Break) for n in used):
                out.append(ind + '%s_brk:;' % lab)
        elif isinstance(s, ir.Return):
            pops = self.pops(0)
            if s.expr is None:
                out.append(ind + '%sreturn;' % pops)
            else:
                code, t = self.expr(s.expr)
                v = self.conv(code, t, self.ret)
                if pops:
                    out.append(ind + '{ %s r_ = %s; %sreturn r_; }' % (self.ret, v, pops))
                else:
                    out.append(ind + 'return %s;' % v)
        elif isinstance(s, (ir.Break, ir.Continue)):
            depth = len([t for t in self.tries if t[1] < self.loops]) if not s.label else 0
            pops = self.pops(depth) if not s.label else self.pops(0) if self.tries else ''
            word = ('goto %s_%s;' % (s.label, 'brk' if isinstance(s, ir.Break) else 'cont')) if s.label else \
                ('break;' if isinstance(s, ir.Break) else 'continue;')
            out.append(ind + pops + word)
        elif isinstance(s, ir.Throw):
            code, _t = self.expr(s.expr)
            out.append(ind + 'jrt_throw((jobject*)%s);' % code)
        elif isinstance(s, ir.Switch):
            self.switch(s, out, ind)
        elif isinstance(s, ir.Try):
            self.try_(s, out, ind)
        elif isinstance(s, ir.Empty):
            out.append(ind + ';')
        elif isinstance(s, ir.Sync):
            self.block(s.body, out, ind)
        elif isinstance(s, ir.Assert):
            pass
        elif isinstance(s, ir.LocalClass):
            pass
        elif isinstance(s, ir.CtorCall):
            raise Decline('constructor call not first')
        else:
            raise Decline('statement %s' % type(s).__name__)

    def case_label(self, lab, enum):
        lab = cpp03._strip_paren(lab)
        if enum is not None:
            name = lab.sym.name if isinstance(lab, (ir.Select, ir.Name)) and lab.sym is not None else None
            consts = [x.sym.name for x in enum.members if isinstance(x, ir.EnumConst)]
            if name not in consts:
                raise Decline('enum case label')
            return '%d' % consts.index(name)
        if isinstance(lab, ir.Lit):
            return self.expr(lab)[0]
        if isinstance(lab, ir.Unary) and lab.op == '-' and isinstance(lab.x, ir.Lit):
            return '-' + self.expr(lab.x)[0]
        if isinstance(lab, ir.Select) and lab.sym is not None:
            ci = self.p.model.info(lab.sym.owner)
            f = next((x for x in (ci.fields() if ci else []) if x.sym is not None and x.sym.name == lab.sym.name), None)
            if f is not None and f.init is not None:
                return self.case_label(f.init, None)
        raise Decline('case label')

    def switch(self, s, out, ind):
        x, xt = self.expr(cpp03._strip_paren(s.expr))
        enum = self.p.model.of_type(s.expr.type)
        if enum is not None and enum.kind == 'enum':
            x = 'jrt_Enum_ordinal_(%s)' % x
        elif xt == _OBJ:
            x = self.conv(x, xt, _CT_OF_CODE.get(_BOX.get(s.expr.type.name, 'I')))
            enum = None
        else:
            enum = None
        out.append(ind + 'switch (%s) {' % x)
        self.loops += 1
        self.tries.append(('', self.loops - 1)) if False else None
        try:
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
        finally:
            self.loops -= 1
        out.append(ind + '}')

    def try_(self, s, out, ind):
        if s.resources:
            raise Decline('try-with-resources')
        if s.final is not None:
            if cpp03._leaves([s.body] + [c.body for c in s.catches]):
                raise Decline('a finally left by a jump')
            self.temps += 1
            f = 'f%d' % self.temps
            out.append(ind + '{')
            out.append(ind + '\tjrt_try %s;' % f)
            out.append(ind + '\tjrt_push(&%s);' % f)
            out.append(ind + '\tif (setjmp(%s.jb) == 0) {' % f)
            self.tries.append((f, self.loops))
            try:
                self.try_catch(s, out, ind + '\t\t')
            finally:
                self.tries.pop()
            out.append(ind + '\t\tjrt_pop(&%s);' % f)
            out.append(ind + '\t} else {')
            out.append(ind + '\t\tjrt_pop(&%s);' % f)
            self.block(s.final, out, ind + '\t')
            out.append(ind + '\t\tjrt_throw(%s.ex);' % f)
            out.append(ind + '\t}')
            self.block(s.final, out, ind)
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
        self.temps += 1
        t = 't%d' % self.temps
        out.append(ind + '{')
        out.append(ind + '\tjrt_try %s;' % t)
        out.append(ind + '\tjrt_push(&%s);' % t)
        out.append(ind + '\tif (setjmp(%s.jb) == 0) {' % t)
        self.tries.append((t, self.loops))
        try:
            self.block(s.body, out, ind + '\t')
        finally:
            self.tries.pop()
        out.append(ind + '\t\tjrt_pop(&%s);' % t)
        out.append(ind + '\t} else {')
        out.append(ind + '\t\tjrt_pop(&%s);' % t)
        first = True
        for c in s.catches:
            name = _ident(c.sym.name)
            test = ' || '.join('jrt_instanceof(%s.ex, %s)' % (t, p.class_ref(x.name)) for x in c.types)
            ct = p.ct(c.types[0]) if len(c.types) == 1 else 'jrt_Throwable*'
            if ct == _OBJ:
                ct = 'jrt_Throwable*'
            out.append(ind + '\t\t%sif (%s) {' % ('' if first else '} else ', test))
            first = False
            saved = dict(self.vars)
            self.vars[name] = ct
            out.append(ind + '\t\t\t%s %s = (%s)%s.ex;' % (ct, name, ct, t))
            self.block(c.body, out, ind + '\t\t')
            self.vars = saved
        out.append(ind + '\t\t} else {')
        out.append(ind + '\t\t\tjrt_throw(%s.ex);' % t)
        out.append(ind + '\t\t}')
        out.append(ind + '\t}')
        out.append(ind + '}')

    # ---- expressions -> (code, C type)
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
        if isinstance(e, (ir.Lambda, ir.MethodRef)):
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
            name = 'o%d' % self.temps
            self.pending.append(self.decl(ct, name))
            parts.append('%s = %s' % (name, code))
            self.subst[id(n)] = (name, ct)
        return ', '.join(parts)

    def is_sub(self, src, dst):
        if src == dst or dst in (_OBJ, 'void*') or src == 'nullptr_t':
            return True
        a = self.p.model.classes
        ca = next((ci for ci in a.values() if ci.cname + '*' == src), None)
        cb = next((ci for ci in a.values() if ci.cname + '*' == dst), None)
        if ca is not None and cb is not None:
            return self.p.model.is_sub(ca.qname, cb.qname)
        return False

    def conv(self, code, src, dst):
        if src == dst or dst in ('void', '') or src == '':
            return code
        if src == 'nullptr_t':
            return 'NULL'
        prim = _NUMERIC + ('bool',)
        if dst in prim and src in prim:
            if src in ('double', 'float') and dst in _INTEGRAL:
                if dst == 'int64_t':
                    return 'jrt_d2l(%s)' % code
                return 'jrt_d2i(%s)' % code if dst == 'int32_t' else '(%s)jrt_d2i(%s)' % (dst, code)
            return '(%s)(%s)' % (dst, code)
        codes = dict((v, k) for k, v in _CT_OF_CODE.items())
        if dst in prim and src.endswith('*'):
            return 'jrt_unbox_%s((jobject*)%s)' % (codes[dst], code)
        if dst.endswith('*') and src in prim:
            return '%sjrt_box_%s(%s)' % ('' if dst == _OBJ else '(%s)' % dst, codes[src], code)
        if dst.endswith('*') and src.endswith('*'):
            if self.is_sub(src, dst):
                return code if src == dst else '(%s)%s' % (dst, code)
            if dst == _STR:
                return '(jstring*)jrt_cast((jobject*)%s, &jrt_String_class)' % code
            ci = next((c for c in self.p.model.classes.values() if c.cname + '*' == dst), None)
            if ci is not None:
                return '(%s)jrt_cast((jobject*)%s, &%s_class)' % (dst, code, ci.cname)
            return '(%s)%s' % (dst, code)
        if dst == 'void*' or src == 'void*':
            return code
        return code

    def x_Paren(self, e):
        x, t = self.expr(e.x)
        return '(%s)' % x, t

    def x_Lit(self, e):
        v, k = e.value, e.type.kind if e.type is not None else 'null'
        if v is None:
            return 'NULL', 'nullptr_t'
        if k == 'bool':
            return ('true' if v else 'false'), 'bool'
        if k == 'i32':
            return (str(v) if v != -2147483648 else 'JRT_INT_MIN'), 'int32_t'
        if k == 'i64':
            return ('%dLL' % v if v != -9223372036854775808 else 'JRT_LONG_MIN'), 'int64_t'
        if k == 'f32':
            return cpp03._float(v, 'F').replace('F', 'f'), 'float'
        if k == 'f64':
            return cpp03._float(v, ''), 'double'
        if k == 'u16':
            return _char(v).replace("u'", "u'"), 'char16_t'
        if isinstance(v, str):
            return self.p.lit(v), _STR
        raise Decline('literal %r' % (v,))

    def x_Name(self, e):
        n = _ident(e.sym.name)
        if n in self.vars:
            return n, self.vars[n]
        if self.lam is not None and e.sym.name in self.lam:
            return self.lam[e.sym.name]
        if e.sym.name in self.captures:
            return self.captures[e.sym.name], self.p.ct(e.type)
        raise Decline('unknown local %s' % n)

    def this_ref(self):
        if self.lam is not None:
            if '#this' not in self.lam:
                raise Decline('this in a closure that did not capture it')
            return self.lam['#this']
        if self.static:
            raise Decline('this in a static context')
        return 'self', (_OBJ if self.ci.kind == 'interface' else self.ci.cname + '*')

    def x_This(self, e):
        q = e.qualifier if e.qualifier is not None else e.type
        if q is not None and q.kind == 'class' and q.name != self.ci.qname and self.ci.outer is not None \
                and not self.p.model.is_sub(self.ci.qname, q.name):
            return self.outer_of(q.name)
        code, ct = self.this_ref()
        return code, ct

    def outer_of(self, qname):
        code, ct = self.this_ref()
        ci = self.ci
        while ci is not None and not (ci.qname == qname or self.p.model.is_sub(ci.qname, qname)):
            if not ci.needs_outer:
                raise Decline('no enclosing instance of %s' % qname)
            code = '%s->outer_' % code
            ci = ci.outer
        if ci is None:
            raise Decline('no enclosing instance of %s' % qname)
        return code, ci.cname + '*'

    def receiver(self, obj, sym):
        if obj is None or (isinstance(obj, (ir.This, ir.Super)) and obj.qualifier is None and (
                obj.type is None or obj.type.name == self.ci.qname or self.p.model.is_sub(self.ci.qname, obj.type.name)
                or isinstance(obj, ir.Super))):
            if sym is not None and sym.owner and sym.owner != self.ci.qname and self.ci.outer is not None \
                    and not self.p.model.is_sub(self.ci.qname, sym.owner) and (obj is None or isinstance(obj, ir.This)):
                return self.outer_of(sym.owner)
            return self.this_ref()
        return self.expr(obj)

    def x_Select(self, e):
        p, s = self.p, e.sym
        if isinstance(e.obj, ir.TypeName) or s.static:
            ci = p.model.info(s.owner)
            if ci is None:
                got = _STATIC_FIELDS.get((s.owner, s.name))
                if got is None:
                    raise Decline('library static %s.%s' % (s.owner, s.name))
                return got
            code = '%s_%s' % (ci.cname, _ident(s.name))
            if p.needs_clinit(ci) and ci is not self.ci:
                code = '(%s_clinit(), %s)' % (ci.cname, code)
            if s.kind == 'enumconst':
                return code, ci.cname + '*'
            f = next((x for x in ci.fields() if x.sym is not None and x.sym.name == s.name), None)
            return code, p.ct(f.type if f is not None else e.type)
        recv, rt = self.receiver(e.obj, s)
        ci = p.model.info(s.owner)
        if ci is None:
            raise Decline('library field %s.%s' % (s.owner, s.name))
        f = next((x for x in ci.fields() if x.sym is not None and x.sym.name == s.name), None)
        declared = p.ct(jmodel.erase(f.type)) if f is not None else p.ct(jmodel.erase(e.type))
        acc = '%s->%s' % (recv if rt == ci.cname + '*' else '((%s*)jrt_nn(%s))' % (ci.cname, recv), _ident(s.name))
        if rt == ci.cname + '*' and recv not in ('self',) and not recv.startswith('self->'):
            acc = '((%s*)jrt_nn(%s))->%s' % (ci.cname, recv, _ident(s.name))
        want = p.ct(e.type)
        if declared != want and want.endswith('*') and declared == _OBJ:
            return self.conv(acc, declared, want), want
        return acc, declared

    def x_Length(self, e):
        a, _t = self.expr(e.arr)
        return 'JLEN(%s)' % a, 'int32_t'

    def x_Index(self, e):
        a, _t = self.expr(e.arr)
        i, it = self.expr(e.index)
        et = self.p.ct(e.arr.type.elem) if e.arr.type is not None and e.arr.type.kind == 'array' else _OBJ
        store = 'jobject*' if et.endswith('*') and et not in (_STR, _ARR, _SB) else et
        code = 'JA(%s, %s, %s)' % (store, a, self.conv(i, it, 'int32_t'))
        return code, store

    def unboxed(self, code, ct, t):
        if ct.endswith('*') and t is not None and t.kind == 'class' and t.name in _BOX:
            c = _BOX[t.name]
            return 'jrt_unbox_%s((jobject*)%s)' % (c, code), _CT_OF_CODE[c]
        return code, ct

    def x_Unary(self, e):
        x, t = self.expr(e.x)
        if e.op in ('++', '--'):
            if t.endswith('*'):
                raise Decline('++ on a box')
            return ('%s%s' % (x, e.op) if e.postfix else '%s%s' % (e.op, x)), t
        x, t = self.unboxed(x, t, e.x.type)
        if e.op == '!':
            return '!%s' % x, 'bool'
        return '%s%s' % (e.op, x), self.p.ct(e.type)

    def x_Binary(self, e):
        p = self.p
        l, lt = self.expr(e.left)
        r, rt = self.expr(e.right)
        op = e.op
        if op in ('==', '!=') and (lt.endswith('*') or lt == 'nullptr_t') and (rt.endswith('*') or rt == 'nullptr_t'):
            return '(jobject*)%s %s (jobject*)%s' % (l, op, r), 'bool'
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
                return 'jrt_ushr_%s(%s, %s)' % ('J' if lt == 'int64_t' else 'I', l if lt in ('int32_t', 'int64_t') else '(int32_t)%s' % l, r), res
            return '%s %s %s' % (l, op, r), res
        if op in ('/', '%') and res in _INTEGRAL:
            lit = cpp03._strip_paren(e.right)
            if not (isinstance(lit, ir.Lit) and isinstance(lit.value, int) and lit.value not in (0, -1)):
                return 'jrt_%s_%s(%s, %s)' % ('div' if op == '/' else 'rem', 'J' if res == 'int64_t' else 'I', l, r), res
        if op == '%' and res in ('float', 'double'):
            return '(%s)fmod(%s, %s)' % (res, l, r), res
        return '%s %s %s' % (l, op, r), (res if op not in ('==', '!=', '<', '<=', '>', '>=') else 'bool')

    def as_string(self, x):
        """String.valueOf of an operand, as concatenation applies it."""
        code, ct = self.expr(x)
        codes = dict((v, k) for k, v in _CT_OF_CODE.items())
        if isinstance(cpp03._strip_paren(x), ir.Lit) and ct == _STR:
            return code
        if ct in codes:
            c = codes[ct]
            if c in ('B', 'S'):
                return 'jrt_str_I((int32_t)%s)' % code
            return 'jrt_str_%s(%s)' % (c, code)
        if ct == 'nullptr_t':
            return 'jrt_str_O(NULL)'
        return 'jrt_str_O((jobject*)%s)' % code

    def x_Concat(self, e):
        parts = [self.as_string(x) for x in e.parts]
        return 'jrt_concat(%d, %s)' % (len(parts), ', '.join(parts)), _STR

    def x_Assign(self, e):
        p = self.p
        t, tt = self.expr(e.target)
        v, vt = self.expr(e.value)
        if e.op is None:
            return '%s = %s' % (t, self.conv(v, vt, tt)), tt
        if tt == _STR and e.op == '+':
            return '%s = jrt_concat(2, jrt_str_O((jobject*)%s), %s)' % (t, t, self.as_string(e.value)), tt
        plain = isinstance(cpp03._strip_paren(e.target), ir.Name) or (
            isinstance(cpp03._strip_paren(e.target), ir.Select) and isinstance(cpp03._strip_paren(e.target).obj, (ir.This, ir.TypeName)))
        if tt.endswith('*'):
            raise Decline('compound assignment to a reference')
        v, vt = self.unboxed(v, vt, e.value.type)
        if e.op in ('/', '%') and tt in _INTEGRAL and vt in _INTEGRAL:
            if not plain:
                raise Decline('integer /= on a complex target')
            wide = 'J' if 'int64_t' in (tt, vt) else 'I'
            return '%s = (%s)jrt_%s_%s(%s, %s)' % (t, tt, 'div' if e.op == '/' else 'rem', wide, t, v), tt
        if e.op in ('<<', '>>', '>>>'):
            width = 64 if tt == 'int64_t' else 32
            if e.op == '>>>':
                if not plain:
                    raise Decline('>>>= on a complex target')
                return '%s = (%s)jrt_ushr_%s(%s, (%s & %d))' % (t, tt, 'J' if tt == 'int64_t' else 'I',
                                                                t if tt in ('int32_t', 'int64_t') else '(int32_t)' + t,
                                                                v, width - 1), tt
            return '%s %s= (%s & %d)' % (t, e.op, v, width - 1), tt
        if tt in _INTEGRAL and vt in ('float', 'double'):
            if not plain:
                raise Decline('integer op= double on a complex target')
            return '%s = (%s)%s(%s %s %s)' % (t, tt, 'jrt_d2l' if tt == 'int64_t' else 'jrt_d2i', t, e.op, v), tt
        if e.op == '%' and tt in ('float', 'double'):
            if not plain:
                raise Decline('%= on a complex target')
            return '%s = (%s)fmod(%s, %s)' % (t, tt, t, v), tt
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
        prim = _NUMERIC + ('bool',)
        if ct in prim and xt in prim:
            if xt in ('double', 'float') and ct not in ('double', 'float'):
                return self.conv(x, xt, ct), ct
            return '(%s)%s' % (ct, x if cpp03._SIMPLE.match(x) else '(%s)' % x), ct
        if ct.endswith('*') and xt.endswith('*') or xt == 'nullptr_t':
            if self.is_sub(xt, ct):
                return x if xt == ct else '(%s)%s' % (ct, x), ct
            if e.type.kind == 'class':
                return '(%s)jrt_cast((jobject*)%s, %s)' % (ct, x, self.p.class_ref(e.type.name)), ct
            return '(%s)%s' % (ct, x), ct
        return self.conv(x, xt, ct), ct

    def x_InstanceOf(self, e):
        if e.binding is not None:
            raise Decline('instanceof pattern not lowered')
        x, _xt = self.expr(e.x)
        if e.test.kind == 'array':
            return 'jrt_instanceof((jobject*)%s, &jrt_Array_class)' % x, 'bool'
        return 'jrt_instanceof((jobject*)%s, %s)' % (x, self.p.class_ref(e.test.name)), 'bool'

    def args(self, args, params, sym, lib=False):
        args = list(args or [])
        if sym is not None and sym.varargs and params and not (len(args) == len(params) and args[-1].type is not None
                                                              and args[-1].type.kind in ('array', 'null')):
            if lib:
                raise Decline('library varargs %s' % sym.name)
            fixed = len(params) - 1
            head = [self.conv(*(self.expr(a) + (params[i],))) for i, a in enumerate(args[:fixed])]
            et = self.p.ct(jmodel.erase(sym.params[-1].elem)) if sym.params[-1].kind == 'array' else _OBJ
            store = 'jobject*' if et.endswith('*') else et
            rest = [self.conv(*(self.expr(a) + (store,))) for a in args[fixed:]]
            if rest:
                packed = 'jrt_array_of(sizeof(%s), %d, (%s[]){%s})' % (store, len(rest), store, ', '.join(rest))
            else:
                packed = 'jrt_new_array(sizeof(%s), 0)' % store
            return ', '.join(head + [packed])
        out = []
        for i, a in enumerate(args):
            want = params[i] if i < len(params) else ''
            a0 = cpp03._strip_paren(a)
            if isinstance(a0, (ir.Lambda, ir.MethodRef)):
                code, ct = self.closure(a0)
            else:
                code, ct = self.expr(a)
            out.append(self.conv(code, ct, want))
        return ', '.join(out)

    def lib_call(self, e):
        """A call into the runtime: jrt_<Class>_<name>_<parameter codes>(receiver, arguments)."""
        p, s = self.p, e.sym
        owner = _LIB.get(s.owner, s.owner.rsplit('.', 1)[-1])
        if s.owner == 'java.util.Map.Entry':
            owner = 'MapEntry'
        if s.owner in ('java.util.List', 'java.util.Set', 'java.util.Map') and s.name == 'of':
            vals = [self.conv(*(self.expr(a) + (_OBJ,))) for a in e.args]
            return 'jrt_%s_of(%d%s)' % (owner, len(vals), ''.join(', ' + v for v in vals)), _OBJ
        if s.owner == 'java.lang.String' and s.name == 'format':
            vals = [self.conv(*(self.expr(a) + (_OBJ,))) for a in e.args[1:]]
            f = self.conv(*(self.expr(e.args[0]) + (_STR,)))
            return 'jrt_String_format(%s, %d%s)' % (f, len(vals), ''.join(', ' + v for v in vals)), _STR
        if s.name in ('toString', 'equals', 'hashCode') and len(s.params) == (1 if s.name == 'equals' else 0) \
                and not s.static and e.target is not None and not isinstance(e.target, ir.TypeName) \
                and s.owner != 'java.lang.String':
            recv, rt = self.receiver(e.target, s)             # Object's methods: through the class's table
            fn = {'toString': 'jrt_toString', 'equals': 'jrt_equals', 'hashCode': 'jrt_hashCode'}[s.name]
            args = [self.conv(recv, rt, _OBJ)] + [self.conv(*(self.expr(a) + (_OBJ,))) for a in e.args]
            return '%s(%s)' % (fn, ', '.join(args)), {'toString': _STR, 'equals': 'bool', 'hashCode': 'int32_t'}[s.name]
        if s.owner == 'java.lang.Comparable' and s.name == 'compareTo':
            a, at = self.expr(e.target)
            b, bt = self.expr(e.args[0])
            return 'jrt_natural((jobject*)%s, %s)' % (a, self.conv(b, bt, _OBJ)), 'int32_t'
        params = []
        for t in s.params:
            et = jmodel.erase(t)
            params.append(self.p.ct(et) if et.kind in _PRIM else (_STR if _code(et) == 'Str' else
                                                                  (_ARR if et.kind == 'array' else _OBJ)))
        if s.owner == 'java.lang.StringBuilder' or s.owner == 'java.lang.StringBuffer':
            params = [(_STR if x == _STR else x) for x in params]
        codes = ''.join(_code(jmodel.erase(t)) for t in s.params)
        name = 'jrt_%s_%s_%s' % (owner, s.name, codes)
        args = self.args(e.args, params, s, lib=True)
        ret = p.ct(jmodel.erase(s.ret))
        if ret.endswith('*') and ret not in (_STR, _ARR, _SB):
            ret = _OBJ
        if s.owner == 'java.lang.StringBuilder' and ret == _OBJ:
            ret = _SB
        if s.static or isinstance(e.target, ir.TypeName):
            code = '%s(%s)' % (name, args)
        else:
            recv, rt = self.receiver(e.target, s)
            if e.target is not None and e.target.type is not None and e.target.type.kind == 'array' and s.name == 'clone':
                return 'jrt_array_clone(%s)' % recv, _ARR
            rwant = _STR if s.owner == 'java.lang.String' else (_SB if owner == 'StringBuilder' else
                                                                ('void*' if owner == 'PrintStream' else _OBJ))
            code = '%s(%s)' % (name, ', '.join([self.conv(recv, rt, rwant)] + ([args] if args else [])))
        want = p.ct(e.type)
        if ret != want and want.endswith('*') and ret == _OBJ:
            return self.conv(code, ret, want), want
        if ret != want and want in _NUMERIC + ('bool',) and ret.endswith('*'):
            return self.conv(code, ret, want), want
        return code, ret

    def x_Call(self, e):
        p, s = self.p, e.sym
        if s is None or s.kind != 'method':
            raise Decline('call %s' % (s.kind if s is not None else 'unresolved'))
        ci, m = p.model.method_decl(s)
        eci = p.model.info(s.owner)
        if m is None and eci is not None and eci.kind == 'enum' and s.name == 'values' and not e.args:
            return '%s_values()' % eci.cname, _ARR
        if m is None:
            return self.lib_call(e)
        ps, r, eps = p.sig(ci, m)
        args = self.args(e.args, ps, s)
        name = p.mname(m.name, eps)
        t = e.target
        static = m.sym is not None and m.sym.static
        if static or isinstance(t, ir.TypeName):
            code = '%s_%s(%s)' % (ci.cname, name, args)
        elif isinstance(t, ir.Super):
            recv, _rt = self.this_ref()
            code = '%s_%s(%s)' % (ci.cname, name, ', '.join(['(jobject*)%s' % recv] + ([args] if args else [])))
        else:
            recv, rt = self.receiver(t, s)
            if m.name in ('toString', 'equals', 'hashCode') and len(m.params) == (1 if m.name == 'equals' else 0):
                fn = {'toString': 'jrt_toString', 'equals': 'jrt_equals', 'hashCode': 'jrt_hashCode'}[m.name]
                code = '%s(%s)' % (fn, ', '.join(['(jobject*)%s' % recv] + ([args] if args else [])))
            elif ci.kind == 'interface':
                code = '%s_%s_i(%s)' % (ci.cname, name, ', '.join(['(jobject*)%s' % recv] + ([args] if args else [])))
            elif 'private' in (m.mods or []):
                code = '%s_%s(%s)' % (ci.cname, name, ', '.join(['(jobject*)%s' % recv] + ([args] if args else [])))
            else:
                root = self.slot_owner(ci, name)
                code = '%s_%s_v(%s)' % (root.cname, name, ', '.join(['(jobject*)%s' % recv] + ([args] if args else [])))
        want = p.ct(e.type)
        if r != want and want.endswith('*') and r.endswith('*') and not self.is_sub(r, want):
            return self.conv(code, r, want), want
        if r != want and want in _NUMERIC + ('bool',) and r.endswith('*'):
            return self.conv(code, r, want), want
        return code, r

    def slot_owner(self, ci, name):
        """The topmost class whose table first has the slot (its _v helper is the one written)."""
        best = ci
        q = ci.base
        while q:
            b = self.p.model.info(q)
            if b is None or b.kind == 'interface':
                break
            if any(sl[0] == name for sl in self.p.slots(b)):
                best = b
            q = b.base
        return best

    def x_New(self, e):
        p = self.p
        t = e.type
        if e.body is not None:
            return self.new_class(p.model.by_decl[id(e.body)], e)
        if t is None or t.kind != 'class':
            raise Decline('new of %r' % (t,))
        ci = p.model.info(t.name)
        if ci is None:
            owner = _LIB.get(t.name, t.name.rsplit('.', 1)[-1])
            if t.name in _THROWABLES or t.name == 'java.lang.Throwable':
                xs = list(e.args or [])
                msg, cause = 'NULL', 'NULL'
                if len(xs) == 1 and xs[0].type is not None and xs[0].type.kind == 'class' \
                        and xs[0].type.name != 'java.lang.String':
                    cause = self.conv(*(self.expr(xs[0]) + (_OBJ,)))
                elif xs:
                    msg = self.conv(*(self.expr(xs[0]) + (_STR,)))
                    if len(xs) > 1:
                        cause = self.conv(*(self.expr(xs[1]) + (_OBJ,)))
                return 'jrt_new_throwable(&jrt_%s_class, %s, %s)' % (owner, msg, cause), _OBJ
            codes = ''.join(_code(jmodel.erase(x)) for x in (e.sym.params if e.sym is not None else []))
            params = [self.p.ct(jmodel.erase(x)) if jmodel.erase(x).kind in _PRIM else
                      (_STR if _code(jmodel.erase(x)) == 'Str' else (_ARR if jmodel.erase(x).kind == 'array' else _OBJ))
                      for x in (e.sym.params if e.sym is not None else [])]
            args = self.args(e.args, params, e.sym, lib=True)
            ret = _SB if owner == 'StringBuilder' else (_STR if owner == 'String' else _OBJ)
            return 'jrt_%s_new_%s(%s)' % (owner, codes, args), ret
        if ci.local or ci.anon:
            return self.new_class(ci, e)
        _c, m = p.model.method_decl(e.sym)
        if m is None and any(x.kind == 'ctor' for x in ci.methods()):
            raise Decline('constructor of %s' % ci.cname)
        params = [p.ct(x.type) for x in m.params] if m is not None else []
        args = self.args(e.args, params, e.sym)
        pre = []
        if ci.needs_outer:
            pre.append(self.expr(e.outer)[0] if e.outer is not None else self.outer_of(ci.outer.qname)[0])
        name = p.ctor_name(ci, m).replace('_init', '_new', 1) if m is not None else '%s_new' % ci.cname
        return '%s(%s)' % (name, ', '.join(pre + ([args] if args else []))), ci.cname + '*'

    def new_class(self, ci, e):
        p = self.p
        pre = []
        if ci.needs_outer:
            pre.append('%s' % self.this_ref()[0])
        for n, t in ci.captures:
            code, _ct = self.x_Name(ir.Name(sym=ir.Sym('local', n), type=t))
            pre.append(code)
        params = []
        ms = [x for x in ci.methods() if x.kind == 'ctor']
        if ms:
            _c, m = p.model.method_decl(e.sym)
            params = [p.ct(x.type) for x in m.params] if m is not None else []
            name = p.ctor_name(ci, m).replace('_init', '_new', 1)
        else:
            b = p.model.info(ci.base) if ci.base else None
            if b is not None and b.kind != 'interface' and e.sym is not None:
                _bc, bm = p.model.method_decl(e.sym)
                params = [p.ct(x.type) for x in bm.params] if bm is not None else []
            name = '%s_new' % ci.cname
        args = self.args(e.args, params, e.sym)
        return '%s(%s)' % (name, ', '.join(pre + ([args] if args else []))), ci.cname + '*'

    def x_NewArray(self, e):
        p = self.p
        et = p.ct(e.type.elem)
        store = 'jobject*' if et.endswith('*') and et not in (_STR, _ARR, _SB) else et
        if e.init is not None:
            items = []
            for x in e.init:
                if isinstance(x, ir.NewArray) and x.type is None:
                    x.type = e.type.elem
                code, xt = self.expr(x)
                items.append(self.conv(code, xt, store))
            if not items:
                return 'jrt_new_array(sizeof(%s), 0)' % store, _ARR
            return 'jrt_array_of(sizeof(%s), %d, (%s[]){%s})' % (store, len(items), store, ', '.join(items)), _ARR
        dims = [self.conv(*(self.expr(d) + ('int32_t',))) for d in e.dims]
        if len(dims) == 1:
            return 'jrt_new_array(sizeof(%s), %s)' % (store, dims[0]), _ARR
        if len(dims) == 2:
            inner = p.ct(e.type.elem.elem)
            inner = 'jobject*' if inner.endswith('*') and inner not in (_STR, _ARR, _SB) else inner
            return 'jrt_new_array2(sizeof(%s), %s, %s)' % (inner, dims[0], dims[1]), _ARR
        raise Decline('array of %d dimensions' % len(dims))

    def x_SwitchExpr(self, e):
        raise Decline('a switch expression inside an expression')

    def x_ClassLit(self, e):
        raise Decline('class literal')

    def x_TypeName(self, e):
        raise Decline('a type as a value')

    # ---- closures
    def closure(self, e):
        if isinstance(e, ir.Lambda):
            return self.x_Lambda(e)
        return self.x_MethodRef(e)

    def sam(self, t):
        if t is None or t.kind != 'class':
            raise Decline('lambda without a functional interface type')
        p = self.p
        sam = p.model.sam(t.name)
        if sam is None:
            raise Decline('not a functional interface: %s' % t.name)
        name, ps, r = sam
        ci = p.model.info(t.name)
        if ci is not None:
            ms = p.iface_methods(t.name)
            slot = next(x for x in ms if x[3] == name)
            return t.name, slot[0], slot[1], slot[2], ms
        cps = [p.ct(jmodel.erase(x)) for x in ps]
        return t.name, name, cps, p.ct(jmodel.erase(r)), [(name, cps, p.ct(jmodel.erase(r)), name, ps)]

    def x_Lambda(self, e):
        p = self.p
        q, slot, ps, r, ms = self.sam(e.type)
        if len(ms) != 1:
            raise Decline('a lambda for an interface with default methods')
        p.nlambda += 1
        name = 'Lambda%d' % p.nlambda
        # what the body reads from outside: locals and parameters of the enclosing code, and `this`
        inner = set(_ident(x.sym.name) for x in e.params)
        for n in ir.walk(e.body):
            if isinstance(n, ir.Local) and n.sym is not None:
                inner.add(_ident(n.sym.name))
        caps, seen = [], set()
        uses_this = False
        for n in ir.walk(e.body):
            if isinstance(n, ir.Name) and n.sym is not None:
                k = _ident(n.sym.name)
                if k not in inner and k not in seen:
                    code, ct = self.x_Name(n)
                    caps.append((k, code, ct))
                    seen.add(k)
            if isinstance(n, (ir.This, ir.Super)) or (isinstance(n, ir.Call) and n.target is None and n.sym is not None
                                                        and not n.sym.static) or \
                    (isinstance(n, ir.Select) and isinstance(n.obj, ir.This)):
                uses_this = True
        this_code = this_ct = None
        if uses_this:
            this_code, this_ct = self.this_ref()
        fields = ['\tjobject hdr_;'] + ['\t%s cap_%s;' % (ct, k) for k, _c, ct in caps]
        if uses_this:
            fields.append('\t%s cap_this;' % this_ct)
        body = Method(p, self.ci, None)
        body.lam = dict((k, ('self->cap_%s' % k, ct)) for k, _c, ct in caps)
        if uses_this:
            body.lam['#this'] = ('self->cap_this', this_ct)
        body.static = self.static
        body.ret = r
        params, entry = ['jobject* self_'], ['\tstruct %s* self = (struct %s*)self_;' % (name, name), '\t(void)self;']
        for i, prm in enumerate(e.params):
            want = ps[i]
            own = p.ct(prm.type)
            n = _ident(prm.sym.name)
            body.vars[n] = own
            if own != want:
                params.append('%s %s_in' % (want, n))
                entry.append('\t%s %s = %s;' % (own, n, body.conv(n + '_in', want, own)))
            else:
                params.append('%s %s' % (want, n))
        out = []
        if isinstance(e.body, ir.Block):
            body.vol = any(isinstance(n, ir.Try) for n in ir.walk(e.body))
            body.stmts(e.body.stmts, out, '\t')
        else:
            body.stmt(ir.Return(expr=e.body) if r != 'void' else ir.ExprStmt(expr=e.body), out, '\t')
        vt = 'struct %s_vt' % p.model.info(q).cname if p.model.info(q) is not None else 'jrt_%s_vt' % q.rsplit('.', 1)[-1]
        cls_ref = p.class_ref(q)
        L = ['struct %s {' % name] + fields + ['};']
        L.append('static %s %s_fn(%s) {' % (r, name, ', '.join(params)))
        L += entry + out + ['}']
        L.append('static const %s %s_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, %s_fn};' % (vt, name, name))
        L.append('static const jitab %s_itabs[] = {{%s, &%s_vt}};' % (name, cls_ref, name))
        L.append('static const jclass %s_class = {"%s$$Lambda", &jrt_Object_class, sizeof(struct %s), &jrt_Object_vt, JRT_ITABS(%s_itabs), 0};'
                 % (name, self.ci.qname.strip('<>'), name, name))
        cparams = ['%s c_%s' % (ct, k) for k, _c, ct in caps] + (['%s c_this' % this_ct] if uses_this else [])
        L.append('static jobject* %s_new(%s) {' % (name, ', '.join(cparams) or 'void'))
        L.append('\tstruct %s* o = (struct %s*)jrt_alloc(&%s_class);' % (name, name, name))
        L += ['\to->cap_%s = c_%s;' % (k, k) for k, _c, _ct in caps]
        if uses_this:
            L.append('\to->cap_this = c_this;')
        L += ['\treturn (jobject*)o;', '}', '']
        p.lambdas += L
        args = [c for _k, c, _ct in caps] + ([this_code] if uses_this else [])
        return '%s_new(%s)' % (name, ', '.join(args)), _OBJ

    def x_MethodRef(self, e):
        """A method reference as the lambda it stands for, written as one."""
        s = e.sym
        if s is None:
            raise Decline('unresolved method reference')
        q, slot, ps, r, ms = self.sam(e.type)
        sam_ps = [x for x in (self.p.model.sam(q) or (None, [], None))[1]]
        params = []
        for i in range(len(ps)):
            t = sam_ps[i] if i < len(sam_ps) else ir.Type('class', jmodel.OBJECT)
            t = jmodel.erase(t)
            params.append(ir.Param(sym=ir.Sym('param', 'a%d' % i, type=t), type=t, mods=[]))
        names = [ir.Name(sym=x.sym, type=x.type) for x in params]
        if e.mode == 'new':
            t = e.target.type if e.target is not None else None
            if t is not None and t.kind == 'array':
                body = ir.NewArray(elem=t.elem, dims=[names[0]], init=None, type=t)
            else:
                body = ir.New(sym=s, args=self.ref_args(names, s.params), body=None, outer=None, type=t)
        elif s.static:
            body = ir.Call(target=ir.TypeName(type=ir.Type('class', s.owner)), sym=s,
                           args=self.ref_args(names, s.params), typeargs=[], type=s.ret)
        elif isinstance(e.target, ir.TypeName):
            recv = ir.Cast(x=names[0], type=ir.Type('class', s.owner))
            body = ir.Call(target=recv, sym=s, args=self.ref_args(names[1:], s.params), typeargs=[], type=s.ret)
        else:
            # the receiver is evaluated where the reference is made: held in a local the lambda captures
            code, ct = self.expr(e.target) if not isinstance(e.target, (ir.This, ir.Super)) else self.this_ref()
            self.temps += 1
            rname = 'mr%d' % self.temps
            self.pending.append(self.decl(ct, rname))
            self.vars[rname] = ct
            rt = e.target.type
            body = ir.Call(target=ir.Name(sym=ir.Sym('local', rname, type=rt), type=rt), sym=s,
                           args=self.ref_args(names, s.params), typeargs=[], type=s.ret)
            lam = ir.Lambda(params=params, body=body, type=e.type)
            got, gt = self.x_Lambda(lam)
            return '(%s = %s, %s)' % (rname, code, got), gt
        return self.x_Lambda(ir.Lambda(params=params, body=body, type=e.type))

    @staticmethod
    def ref_args(names, ptypes):
        out = []
        for n, t in zip(names, ptypes):
            out.append(ir.Cast(x=n, type=t) if t is not None and t.kind not in ('typevar', 'wildcard') and n.type != t else n)
        return out


def program(units, main=None):
    return Program(units, main).write()


def build(units, out):
    """Write the program as C and compile it with clang (else the host's gcc) against the runtime.
    -> (exe or None, why)"""
    from codebreaker.translate.backends.cpp03 import Decline as _D
    try:
        text = program(units)
    except _D as e:
        return None, 'Decline: %s' % e
    src = os.path.join(out, 'program.c')
    with open(src, 'w', encoding='utf-8') as f:
        f.write(text)
    cc = CLANG if os.path.isfile(CLANG) else 'gcc'
    exe = os.path.join(out, 'program.exe')
    r = subprocess.run([cc] + list(FLAGS) + ['-I', RUNTIME, src, os.path.join(RUNTIME, 'jrt.c'), '-o', exe],
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=600)
    if r.returncode != 0:
        err = r.stdout.decode('utf-8', 'replace')
        first = next((l for l in err.splitlines() if 'error' in l), err[:300])
        return None, 'does not compile: ' + first[:400]
    return exe, ''
