"""java.py -- the translator's Java back end: a C or C++ program (L2 from the clang front end) as Java 17.

docs/TRANSLATOR_DESIGN.md, phase 5. The output comes in the design's two tiers, chosen per program by a
pointer analysis (`analyse`):

  * **idiomatic** -- when every pointer is disciplined (it only ever names a whole struct, or an array
    it indexes; no pointer arithmetic, no casts between pointer types, no unions, no address of a
    scalar): structs become classes, struct pointers references, arrays Java arrays, malloc `new`, free
    nothing. C++ classes are this tier's: methods, constructors, single inheritance, virtual calls.
  * **faithful** -- otherwise: C's memory is emulated, as Emscripten and CheerpJ do. Everything whose
    bytes the program can reach (arrays, structs, any variable whose address is taken, the heap) lives
    in one linear memory (`crt.Mem`, a little-endian byte array); a pointer is an int address into it;
    a struct is its address, its fields at offsets named in a small class per struct (`Pair.b`), laid
    out as the source's compiler lays them out (ClassDecl.layout), so unions, punning, pointer
    arithmetic and sizeof all mean what they meant. Variables whose address is never taken stay
    ordinary Java locals and fields.

Both tiers share C's arithmetic on Java's types: a char is a byte, unsigned short a char, unsigned int
an int and unsigned long long a long, with the unsigned operations (division, comparison, widening,
`>>`) written out (`Integer.divideUnsigned`, `x & 0xFF`, `>>>`). C's library is `crt.LibC` (printf with
C's own formatting, the string and memory functions, malloc/free on the emulated heap, MSVC's rand),
statically imported, so calls read as they did in C.

What the back end does not write it refuses, with the reason (`Refused`): a goto, a varargs function
definition, setjmp, a library function the runtime does not have. Nothing is dropped silently.

    prog = java.program([unit, ...], 'Hello')      # -> Program: .source (Java text), .tier, .notes
    java.RUNTIME                                     # the folder holding crt/*.java
"""
import os
import re

from codebreaker.translate import ir

RUNTIME = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'java_runtime')

JPRIM = {'bool': 'boolean', 'i8': 'byte', 'u8': 'byte', 'char': 'byte', 'i16': 'short', 'u16': 'char',
         'i32': 'int', 'u32': 'int', 'i64': 'long', 'u64': 'long', 'f32': 'float', 'f64': 'double',
         'f80': 'double'}
_BITS = {'bool': 1, 'i8': 8, 'u8': 8, 'char': 8, 'i16': 16, 'u16': 16, 'i32': 32, 'u32': 32, 'i64': 64, 'u64': 64}
_JBITS = {'byte': 8, 'short': 16, 'char': 16, 'int': 32, 'long': 64}
_ACCESS = {'byte': 'Byte', 'short': 'Short', 'char': 'Char', 'int': 'Int', 'long': 'Long', 'float': 'Float',
           'double': 'Double', 'boolean': 'Bool'}
_KEYWORDS = frozenset('''abstract assert boolean break byte case catch char class const continue default do double
else enum extends final finally float for goto if implements import instanceof int interface long native new package
private protected public return short static strictfp super switch synchronized this throw throws transient try void
volatile while true false null var record yield sealed permits Mem LibC Fn String Object Math System Integer Long
Double Float Character Byte Short Boolean frame'''.split())

# Java operator precedence (higher binds tighter)
P_ATOM, P_POST, P_UNARY, P_MUL, P_ADD, P_SHIFT, P_REL, P_EQ, P_BAND, P_XOR, P_BOR, P_AND, P_OR, P_COND, P_ASSIGN = \
    17, 16, 15, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2
_BINPREC = {'*': P_MUL, '/': P_MUL, '%': P_MUL, '+': P_ADD, '-': P_ADD, '<<': P_SHIFT, '>>': P_SHIFT, '>>>': P_SHIFT,
            '<': P_REL, '<=': P_REL, '>': P_REL, '>=': P_REL, '==': P_EQ, '!=': P_EQ, '&': P_BAND, '^': P_XOR,
            '|': P_BOR, '&&': P_AND, '||': P_OR}


class Refused(Exception):
    """The back end does not write this program (the reason says why)."""


class J(object):
    """A Java expression: its text, its precedence and its Java type ('boolean' for a C truth value)."""
    __slots__ = ('code', 'prec', 'jt', 'fresh')

    def __init__(self, code, prec, jt, fresh=False):
        self.code, self.prec, self.jt, self.fresh = code, prec, jt, fresh

    def at(self, prec):
        return self.code if self.prec >= prec else '(%s)' % self.code

    def __repr__(self):
        return 'J(%r, %s, %s)' % (self.code, self.prec, self.jt)


def jname(name):
    name = re.sub(r'\W', '_', name or 'x')
    if name in _KEYWORDS or name[:1].isdigit():
        return name + '_'
    return name


def _kind(t):
    """A type's kind with plain char resolved by its sign."""
    if t is None:
        return 'void'
    if t.kind == 'char':
        return 'i8' if t.info.get('signed', True) else 'u8'
    return t.kind


def is_int(t):
    return _kind(t) in _BITS and _kind(t) != 'bool'


def is_unsigned(t):
    return _kind(t) in ('u8', 'u16', 'u32', 'u64')


def is_float(t):
    return _kind(t) in ('f32', 'f64', 'f80')


def is_ptr(t):
    return t is not None and t.kind in ('ptr', 'fnptr', 'null')


def is_record(t):
    return t is not None and t.kind in ('class', 'record')


def spell(t):
    """A type's canonical spelling -- the same the clang front end names records and template instances by
    (ClassDecl.qname): 'Box<f64>', 'geo.Vec', 'char*'."""
    if t is None:
        return '?'
    k = t.kind
    if k == 'class':
        return t.name + ('<%s>' % ', '.join(spell(a) for a in t.args) if t.args else '')
    if k == 'ptr':
        return spell(t.elem) + ('&' if t.info.get('ref') else '*')
    if k == 'array':
        return '%s[%s]' % (spell(t.elem), t.info.get('count', ''))
    if k == 'fnptr':
        return spell(t.info['fn']) + '*'
    if k == 'fn':
        return '%s(%s)' % (spell(t.info['result'].get('type')), ','.join(spell(p['type']) for p in t.info['params']))
    return k


def strip(e):
    while isinstance(e, ir.Paren):
        e = e.x
    return e


def java_string(s):
    """A Java string literal for a C string held one character per byte."""
    out = []
    for ch in s:
        o = ord(ch)
        if ch == '"':
            out.append('\\"')
        elif ch == '\\':
            out.append('\\\\')
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\t':
            out.append('\\t')
        elif ch == '\r':
            out.append('\\r')
        elif 32 <= o < 127:
            out.append(ch)
        else:
            out.append('\\%03o' % (o & 0xFF))
    return '"%s"' % ''.join(out)


def java_wstring(s):
    """A Java string literal for a wide (UTF-16) C string."""
    out = []
    for ch in s:
        o = ord(ch)
        if ch in '"\\':
            out.append('\\' + ch)
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\r':
            out.append('\\r')
        elif ch == '\t':
            out.append('\\t')
        elif 32 <= o < 127:
            out.append(ch)
        elif o < 256:
            out.append('\\%03o' % o)
        else:
            out.append('\\u%04x' % o)
    return '"%s"' % ''.join(out)


def string_lit(lit):
    """A C string literal (char or wide) as a Java string literal."""
    t = lit.type
    if t is not None and t.kind == 'array' and t.elem is not None and _kind(t.elem) in ('u16', 'i16'):
        return java_wstring(lit.value)
    return java_string(lit.value)


def is_wide(lit):
    t = lit.type
    return t is not None and t.kind == 'array' and t.elem is not None and _kind(t.elem) in ('u16', 'i16')


def int_lit(v, jt):
    """An integer constant as a Java literal of type jt (the value reduced to jt's bits)."""
    bits = _JBITS.get(jt, 32)
    v &= (1 << bits) - 1
    if jt == 'char':
        return J('(char) %d' % v if v > 127 or v < 32 else repr(chr(v)) if chr(v) not in "'\\" else '(char) %d' % v,
                 P_UNARY if v > 127 or v < 32 or chr(v) in "'\\" else P_ATOM, 'char')
    if v >= 1 << (bits - 1):
        v -= 1 << bits
    if jt == 'long':
        if v < -(1 << 40) or v > (1 << 40):
            return J('0x%XL' % (v & 0xFFFFFFFFFFFFFFFF), P_ATOM, 'long')
        return J('%dL' % v, P_ATOM if v >= 0 else P_UNARY, 'long')
    if jt in ('byte', 'short'):
        return J('(%s) %d' % (jt, v), P_UNARY, jt) if v != 0 else J('0', P_ATOM, 'int')
    if v < -(1 << 24) and v != -(1 << 31):
        return J('0x%X' % (v & 0xFFFFFFFF), P_ATOM, 'int')
    return J(str(v), P_ATOM if v >= 0 else P_UNARY, 'int')


def float_lit(v, jt):
    if v != v:
        return J('Double.NaN' if jt == 'double' else 'Float.NaN', P_ATOM, jt)
    if v in (float('inf'), float('-inf')):
        w = 'Double' if jt == 'double' else 'Float'
        return J('%s.%s_INFINITY' % (w, 'POSITIVE' if v > 0 else 'NEGATIVE'), P_ATOM, jt)
    text = repr(float(v))
    if 'e' in text and '.' not in text.split('e')[0]:
        text = text.replace('e', '.0e')
    return J(text + ('f' if jt == 'float' else ''), P_ATOM if v >= 0 and not text.startswith('-') else P_UNARY, jt)


# ============================================================================================ analysis
class Analysis(object):
    """The pointer analysis that picks the tier, and the facts both tiers need."""

    def __init__(self, units):
        self.units = units
        self.why = []                      # what keeps the program from the idiomatic tier
        self.addressed = set()             # sym keys whose address is taken
        self.cpp_classes = False

    RAW = frozenset(('memset', 'memcpy', 'memmove', 'memcmp', 'memchr', 'qsort', 'bsearch', 'realloc',
                     'vprintf', 'vfprintf', 'vsprintf', 'vsnprintf', 'strchr', 'strrchr', 'strstr', 'strpbrk',
                     'strtol', 'strtoul', 'strtoll', 'strtoull', 'strtod', 'modf', 'frexp', 'time', 'strdup',
                     '_strdup', 'fgets'))

    def run(self):
        for u in self.units:
            for t in u.types or []:
                if isinstance(t, ir.ClassDecl):
                    if t.kind == 'union':
                        self.why.append('union %s' % t.name)
                    if t.kind == 'class' or any(isinstance(m, ir.MethodDecl) for m in t.members or []) or t.extends:
                        self.cpp_classes = True
                    if t.kind in ('struct', 'union') and any('bitfield' in (m.mods or []) for m in t.members or []):
                        self.why.append('bit-fields')
                if isinstance(t, ir.MethodDecl) and t.sym is not None and t.sym.varargs and t.body is not None:
                    self.why.append('a variadic function (%s)' % t.name)
                self.exempt = set()
                for n in ir.walk(t):
                    # free's argument and C's NULL ((void *) 0) are pointers to void that name nothing
                    if isinstance(n, ir.Call) and n.target is None and n.sym is not None and n.sym.name == 'free':
                        for a in n.args or []:
                            self.exempt.add(id(strip(a)))
                    if isinstance(n, ir.Cast) and _null(n.x):
                        self.exempt.add(id(n))
                        self.exempt.add(id(strip(n.x)))
                    if id(n) in self.exempt and isinstance(n, ir.Cast):
                        self.exempt.add(id(strip(n.x)))
                        continue
                    self.visit(n)
        return self

    def visit(self, n):
        if isinstance(n, ir.Unary) and n.op == '&':
            x = strip(n.x)
            if isinstance(x, ir.Name) and not is_record(x.type) and (x.type is None or x.type.kind != 'array'):
                self.addressed.add(x.sym.name)      # (a scalar whose address is taken: boxed in the idiomatic tier)
            elif isinstance(x, (ir.Index,)) or (isinstance(x, ir.Select) and not is_record(x.type)):
                self.why.append('address of an element or field')
        elif isinstance(n, ir.Call) and n.target is None and n.sym is not None and n.sym.name in self.RAW:
            self.why.append('raw memory (%s)' % n.sym.name)
        elif isinstance(n, ir.Call) and n.target is not None and n.target.type is not None and \
                n.target.type.kind in ('fnptr', 'fn') and len((n.target.type.info.get('fn') or n.target.type).info.get(
                    'params', [])) != len(n.args or []):
            self.why.append('a K&R function pointer (no prototype)')
        elif isinstance(n, ir.Binary) and n.op in ('<', '<=', '>', '>=') and is_ptr(n.left.type):
            self.why.append('pointer comparison')
        elif isinstance(n, ir.Index) and n.arr is not None and n.arr.type is not None and n.arr.type.kind == 'ptr' \
                and n.arr.type.elem is not None and n.arr.type.elem.kind == 'void':
            self.why.append('a void pointer')
        elif isinstance(n, ir.Binary) and n.op in ('+', '-') and (is_ptr(n.left.type) or is_ptr(n.right.type)):
            self.why.append('pointer arithmetic')
        elif isinstance(n, ir.Unary) and n.op in ('++', '--') and is_ptr(n.type):
            self.why.append('pointer arithmetic')
        elif isinstance(n, ir.Assign) and n.op in ('+', '-') and is_ptr(n.target.type):
            self.why.append('pointer arithmetic')
        elif isinstance(n, ir.Cast) and is_ptr(n.type) and n.x.type is not None and is_ptr(n.x.type):
            a, b = n.type, n.x.type
            if a.kind == 'ptr' and b.kind == 'ptr' and a.elem is not None and b.elem is not None and \
                    _kind(a.elem) != _kind(b.elem) and not (a.elem.kind == 'void' or b.elem.kind == 'void') and \
                    not n.implicit:
                self.why.append('a cast between pointer types')
            elif a.kind == 'ptr' and b.kind == 'ptr' and (a.elem.kind == 'void') != (b.elem.kind == 'void'):
                if not _from_alloc(n.x):
                    self.why.append('a void pointer')
        elif isinstance(n, ir.Cast) and is_ptr(n.type) and n.x.type is not None and is_int(n.x.type):
            self.why.append('an integer used as a pointer')
        elif isinstance(n, ir.Cast) and is_int(n.type) and is_ptr(n.x.type):
            self.why.append('a pointer used as an integer')


def _null(e):
    """C's null pointer constant: 0, NULL ((void *) 0), nullptr."""
    e = strip(e)
    while isinstance(e, ir.Cast):
        e = strip(e.x)
    return isinstance(e, ir.Lit) and (e.value is None or (e.value == 0 and not isinstance(e.value, float)))


def _from_alloc(e):
    e = strip(e)
    return isinstance(e, ir.Call) and e.target is None and e.sym is not None and \
        e.sym.name in ('malloc', 'calloc', 'realloc')


class Program(object):
    def __init__(self, name, source, tier, notes, refused=None):
        self.name, self.source, self.tier, self.notes, self.refused = name, source, tier, notes, refused


def program(units, name, tier='auto'):
    """L2 units of one C/C++ program -> Program (Java source of class `name`). Raises Refused.
    tier: 'auto' (idiomatic where the analysis allows it, else faithful), 'idiomatic' or 'faithful'."""
    a = Analysis(units).run()
    why = sorted(set(a.why))
    if tier == 'faithful' and a.cpp_classes:
        raise Refused('C++ classes have no faithful (emulated memory) form yet')
    declined = None
    if tier == 'idiomatic' or (tier == 'auto' and (not why or a.cpp_classes)):
        if why:
            raise Refused('C++ code the object model cannot carry: ' + ', '.join(why))
        from codebreaker.translate.backends import java_objects
        try:
            w = java_objects.Objects(units, name, a)
            src = w.write()
            if not w.gaps or a.cpp_classes or tier == 'idiomatic':
                return Program(name, src, 'idiomatic', w.notes)
            declined = 'gaps in the idiomatic tier: ' + '; '.join(w.gaps)
        except Refused as e:
            if a.cpp_classes or tier == 'idiomatic':
                raise
            declined = str(e)
    a = Analysis(units).run()                    # (afresh: the idiomatic writer may have changed its facts)
    w = Faithful(units, name, a)
    src = w.write()
    return Program(name, src, 'faithful', w.notes + ['faithful tier: ' + (
        'the idiomatic tier declined (%s)' % declined if declined else ', '.join(sorted(set(a.why)) or ['chosen']))])


# ============================================================================================ faithful
class Var(object):
    """A C variable as the Java side keeps it: `java` its Java name; `mem` True when it lives in the
    emulated memory (then `java` names an int holding its address)."""
    __slots__ = ('java', 'mem', 'type', 'kind')

    def __init__(self, java, mem, type, kind):
        self.java, self.mem, self.type, self.kind = java, mem, type, kind


class Faithful(object):
    """The faithful tier: C memory emulated in crt.Mem."""

    def __init__(self, units, name, analysis):
        self.units = units
        self.name = name
        self.a = analysis
        self.notes = []
        tgt = (units[0].target or {}) if units else {}
        self.ptr_size = tgt.get('pointer', 8)
        self.records = {}          # qname -> ClassDecl (with layout)
        self.layout_names = {}     # qname -> Java class name of its offsets
        self.functions = {}        # name -> MethodDecl
        self.globals = {}          # name -> Var
        self.global_decls = []     # FieldDecl in order
        self.fn_ifaces = {}        # signature key -> interface name
        self.fn_consts = {}        # function name -> constant naming its address
        self.used_names = set()
        self.statics = []          # static locals: (Var, Local)
        self.lib_used = set()
        self.gaps = []             # functions written as an explicit gap: 'name (reason)'
        self.overloads = {}        # name -> [MethodDecl] where C++ overloads one name
        for u in units:
            for t in u.types or []:
                if isinstance(t, ir.ClassDecl):
                    self.records[t.qname] = t
                elif isinstance(t, ir.MethodDecl):
                    if t.mods and 'unsupported' in t.mods:
                        raise Refused('a declaration the front end did not lower: %s' % _first_unsupported(t))
                    if t.body is not None and t.sym is not None:
                        if t.name in self.functions:
                            if (u.target or {}).get('language') != 'c++':
                                raise Refused('two definitions of %s' % t.name)
                            # C++ overloads (a function template's instances among them): Java overloads
                            self.overloads.setdefault(t.name, [self.functions[t.name]]).append(t)
                            continue
                        self.functions[t.name] = t
                elif isinstance(t, ir.FieldDecl):
                    self.global_decls.append(t)
        self._check_records()
        self._names()
        self._addressed()

    def _check_records(self):
        for t in list(self.records.values()):
            if t.kind not in ('struct', 'union'):
                raise Refused('%s %s' % (t.kind, t.name))
            if t.layout is None:
                raise Refused('struct %s has no layout (bit-fields?)' % t.name)

    # ------------------------------------------------------------------------------------- names
    def _names(self):
        taken = set(jname(n) for n in self.functions) | set(jname(f.sym.name) for f in self.global_decls)
        taken.add(self.name)
        for qn, c in self.records.items():
            base = jname(c.name.replace('__unnamed_', 'Anon'))
            base = base[:1].upper() + base[1:]
            n = base
            while n in taken:
                n += '_t'
            taken.add(n)
            self.layout_names[qn] = n
        self.used_names = taken

    def _addressed(self):
        """Every variable whose address is taken (by & or by decay of an array): it lives in memory."""
        self.addr_taken = set()
        for u in self.units:
            for n in ir.walk(u):
                if isinstance(n, ir.Unary) and n.op == '&':
                    x = strip(n.x)
                    if isinstance(x, ir.Name):
                        self.addr_taken.add(self.key(x.sym))

    def all_functions(self):
        out = []
        for name, f in self.functions.items():
            out += self.overloads.get(name, [f])
        return out

    def function_for(self, s):
        """The definition a call's symbol names (by its parameter types, where C++ overloads the name)."""
        if s.name not in self.functions:
            return None
        cands = self.overloads.get(s.name)
        if not cands:
            return self.functions[s.name]
        want = [spell(p) for p in s.params]
        for f in cands:
            if [spell(p.type) for p in f.params] == want:
                return f
        for f in cands:
            if len(f.params) == len(want):
                return f
        return cands[0]

    def key(self, s):
        if s.kind in ('global', 'function'):
            return ('g', s.name)
        return ('l', s.owner, s.name)

    # ------------------------------------------------------------------------------------- types
    def jt(self, t):
        k = _kind(t)
        if k in JPRIM:
            return JPRIM[k]
        if k == 'void':
            return 'void'
        if k in ('ptr', 'fnptr', 'array', 'class', 'record', 'null'):
            return 'int'
        raise Refused('type %r' % (t,))

    def layout(self, t):
        c = self.records.get(self.rec_name(t))
        if c is None:
            raise Refused('struct %s: a library type the C runtime for Java does not have' % spell(t))
        return c.layout

    def rec_name(self, t):
        if t.kind == 'class':
            return spell(t)
        return t.name

    def size(self, t):
        k = _kind(t)
        if k in _BITS:
            return 1 if k == 'bool' else _BITS[k] // 8
        if k in ('f32',):
            return 4
        if k in ('f64', 'f80'):
            return 8
        if k in ('ptr', 'fnptr', 'null'):
            return self.ptr_size
        if k == 'array':
            if 'count' not in t.info:
                raise Refused('an array of unknown size')
            return self.size(t.elem) * t.info['count']
        if k in ('class', 'record'):
            return self.layout(t).info['size']
        if k == 'void':
            return 1
        raise Refused('size of %r' % (t,))

    def align(self, t):
        k = _kind(t)
        if k == 'array':
            return self.align(t.elem)
        if k in ('class', 'record'):
            lay = self.layout(t)
            return max([self.align(f['type']) for f in lay.info['fields']] or [1])
        return min(self.size(t), 8)

    def access(self, t):
        """The Mem accessor suffix for a scalar type: Int, Byte, Ptr, ..."""
        k = _kind(t)
        if k in ('ptr', 'fnptr', 'null'):
            return 'Ptr'
        return _ACCESS[JPRIM[k]]

    def size_code(self, t):
        if is_record(t):
            return '%s.SIZE' % self.layout_names[self.rec_name(t)]
        return str(self.size(t))

    def field_off(self, rec_t, fname):
        lay = self.layout(rec_t)
        for f in lay.info['fields']:
            if f['name'] == fname:
                return f
        raise Refused('no field %s in %r' % (fname, rec_t))

    # ------------------------------------------------------------------------------------- the file
    def write(self):
        body = []
        self.static_init = []
        self.fields_out = []
        for g in self.global_decls:
            self.declare_global(g)
        funcs = []
        for f in self.all_functions():
            funcs.append(self.function_or_gap(f, lambda: FnWriter(self, f).write()))
        for f in self.global_decls:
            self.init_global(f)
        head = ['// %s.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).' % self.name,
                'import crt.*;', '', 'import static crt.LibC.*;', '',
                'public final class %s {' % self.name]
        out = list(head)
        for qn in sorted(self.records, key=lambda q: (self.records[q].line, q)):
            out += self.layout_class(self.records[qn])
        for fn in self.fn_consts:                       # (each registered function's interface, before they are written)
            self.iface(self.fn_sig(self.functions[fn].sym))
        for sig, name in self.fn_ifaces.items():
            params, ret = sig
            ps = ', '.join('%s a%d' % (p, i) for i, p in enumerate(params))
            out += ['', '    interface %s { %s call(%s); }' % (name, ret, ps)]
        if self.fields_out or self.static_init:
            out.append('')
        out += ['    ' + f for f in self.fields_out]
        for fn, const in self.fn_consts.items():
            sig = self.fn_sig(self.functions[fn].sym)
            out.append('    static final int %s = Fn.register((%s) %s::%s);' % (const, self.iface(sig), self.name, jname(fn)))
        if self.static_init:
            out += ['', '    static {'] + ['        ' + s for s in self.static_init] + ['    }']
        for f in funcs:
            out += [''] + f
        out += self.entry()
        out.append('}')
        self.notes += ['gap: ' + g for g in self.gaps]
        return '\n'.join(out) + '\n'

    def function_or_gap(self, f, write, indent='    ', owner=None):
        """A function written, or -- where the back end declines it -- the explicit gap the design asks for:
        its signature, and a body that throws with the reason (the Java lane's J2X-GAP), listed in notes."""
        try:
            return write()
        except Refused as e:
            reason = str(e)
            if f.name == 'main' and owner is None:
                raise                                          # (a program whose main is a gap is refused)
            sig = self.gap_signature(f, owner)
            if sig is None:
                raise
            what = '%s%s' % (owner + '::' if owner else '', f.name)
            self.gaps.append('%s (%s)' % (what, reason))
            msg = java_string('CodeBreaker gap: %s: %s' % (what, reason))
            return [indent + sig + ' {', indent + '    // ' + reason.replace('\n', ' '),
                    indent + '    throw new UnsupportedOperationException(%s);' % msg, indent + '}']

    def gap_signature(self, f, owner):
        try:
            params, ret = self.fn_sig(f.sym)
        except Refused:
            return None
        names = ['result'] if len(params) > len(f.params) else []
        names += [jname(p.sym.name) for p in f.params]
        ps = ', '.join('%s %s' % (t, n) for t, n in zip(params, names))
        if f.sym.varargs:
            ps += (', ' if ps else '') + 'Object... va'
        return 'static %s %s(%s)' % (ret, jname(f.name), ps)

    def layout_class(self, c):
        lay = c.layout
        n = self.layout_names[c.qname]
        what = 'union' if c.kind == 'union' else 'struct'
        parts = ['SIZE = %d' % lay.info['size']]
        for f in lay.info['fields']:
            fname = jname(f['name']) if f['name'] != 'SIZE' else 'SIZE_'
            parts.append('%s = %d' % (fname, f['offset']))
        out = ['', '    /** %s %s: its size and its fields\' offsets in memory. */' % (what, c.name.replace('__unnamed_', '(anonymous) ')),
               '    static final class %s {' % n]
        line = '        static final int '
        out.append(line + ', '.join(parts) + ';')
        out.append('    }')
        return out

    def entry(self):
        m = self.functions.get('main')
        if m is None:
            raise Refused('no main function')
        out = ['', '    public static void main(String[] args) {']
        call = 'main()'
        if len(m.params) >= 2:
            call = 'main(args.length + 1, Mem.argv(System.getProperty("c.argv0", "%s"), args))' % self.name
        if m.ret is None or m.ret.kind == 'void':
            out.append('        %s;' % call)
            out.append('        exit(0);')
        else:
            out.append('        exit(%s);' % call)
        out.append('    }')
        return out

    # ------------------------------------------------------------------------------------- globals
    def declare_global(self, g):
        s = g.sym
        name = jname(s.name)
        if 'const' in (g.mods or []) and any(m.startswith('enum:') for m in g.mods or []):
            v = Var(name, False, ir.I32, 'enum')
            self.globals[s.name] = v
            self.fields_out.append('static final int %s = %s;' % (name, g.init.value))
            return
        t = g.type
        mem = t.kind in ('array', 'class', 'record') or ('g', s.name) in self.addr_taken
        v = Var(name, mem, t, 'global')
        if s.name in self.globals and self.globals[s.name].kind != 'enum':
            return                                              # (a tentative definition again)
        self.globals[s.name] = v
        if mem:
            self.fields_out.append('static final int %s = Mem.global(%s, %d);  // %s' % (
                name, self.size_code(t), self.align(t), _ctype_text(t, s.name)))
        else:
            jt = self.jt(t)
            init = g.init
            const = self.const_value(init, t) if init is not None else None
            if init is None:
                self.fields_out.append('static %s %s;' % (jt, name))
            elif const is not None:
                self.fields_out.append('static %s %s = %s;' % (jt, name, const))
            else:
                self.fields_out.append('static %s %s;' % (jt, name))

    def const_value(self, e, t):
        e = strip(e)
        if isinstance(e, ir.Cast) and e.implicit:
            inner = strip(e.x)
            if isinstance(inner, ir.Lit) and isinstance(inner.value, (int, float)) and not isinstance(inner.value, bool):
                e = ir.Lit(value=inner.value, type=t)
        if isinstance(e, ir.Lit) and isinstance(e.value, bool):
            return 'true' if e.value else 'false'
        if isinstance(e, ir.Lit) and isinstance(e.value, int) and (is_int(t) or is_float(t)):
            if is_float(t):
                return float_lit(float(e.value), self.jt(t)).code
            return int_lit(e.value, self.jt(t)).code
        if isinstance(e, ir.Lit) and isinstance(e.value, float) and is_float(t):
            return float_lit(e.value, self.jt(t)).code
        if isinstance(e, ir.Lit) and e.value is None and is_ptr(t):
            return '0'
        return None

    def init_global(self, g):
        v = self.globals.get(g.sym.name)
        if v is None or v.kind == 'enum' or g.init is None:
            return
        if not v.mem and self.const_value(g.init, g.type) is not None:
            return
        w = FnWriter(self, None)
        if v.mem:
            self.static_init.extend(w.init_place(v.java, g.type, g.init))
        else:
            self.static_init.append('%s = %s;' % (v.java, w.conv(w.value(g.init), g.init.type, g.type).code))

    # ------------------------------------------------------------------------------------- functions
    def fn_sig(self, s):
        """(java param types, java return type) of a function symbol (a struct result: an int address)."""
        params = tuple(self.jt(p) for p in s.params)
        ret = self.jt(s.ret) if s.ret is not None else 'void'
        if s.ret is not None and is_record(s.ret):
            params = ('int',) + params
        return params, ret

    def fn_sig_of_type(self, ft):
        f = ft.info['fn'] if ft.kind == 'fnptr' else ft
        params = tuple(self.jt(p['type']) for p in f.info['params'])
        rt = f.info['result'].get('type')
        ret = self.jt(rt) if rt is not None else 'void'
        if rt is not None and is_record(rt):
            params = ('int',) + params
        return params, ret

    def iface(self, sig):
        if sig not in self.fn_ifaces:
            params, ret = sig
            n = ''.join(p[:1].upper() + p[1:] for p in params) + 'To' + ret[:1].upper() + ret[1:] if params else \
                'To' + ret[:1].upper() + ret[1:]
            self.fn_ifaces[sig] = n
        return self.fn_ifaces[sig]

    def fn_const(self, name):
        if name not in self.functions:
            raise Refused('the address of library function %s' % name)
        if name not in self.fn_consts:
            self.fn_consts[name] = 'FN_' + jname(name)
        return self.fn_consts[name]


def _first_unsupported(n):
    u = ir.unsupported(n)
    return '%s (line %s)' % (u[0].kind, u[0].line) if u else '?'


def _ctype_text(t, name):
    k = t.kind
    if k == 'array':
        return _ctype_text(t.elem, '%s[%s]' % (name, t.info.get('count', '')))
    if k == 'ptr':
        return _ctype_text(t.elem, '*' + name)
    if k == 'class':
        return 'struct %s %s' % (t.name, name)
    if k == 'char':
        return 'char ' + name
    return '%s %s' % ({'i8': 'signed char', 'u8': 'unsigned char', 'i16': 'short', 'u16': 'unsigned short',
                       'i32': 'int', 'u32': 'unsigned', 'i64': 'long long', 'u64': 'unsigned long long',
                       'f32': 'float', 'f64': 'double', 'bool': '_Bool', 'fnptr': 'function pointer'}.get(k, k), name)


# ============================================================================================ functions
class FnWriter(object):
    """One function's body in the faithful tier."""

    def __init__(self, prog, fn):
        self.p = prog
        self.fn = fn
        self.vars = {}             # local name -> Var
        self.frame = 0             # frame bytes so far
        self.has_frame = False
        self.lines = []
        self.tmp = 0
        self.declared = set()
        self.goto_map = {}         # id(Goto) -> 'break L' / 'continue L'
        self.labels_done = set()   # id(Labeled) placed by structure()
        self.ret_struct =fn is not None and fn.ret is not None and is_record(fn.ret)

    # ------------------------------------------------------------------------------------- frame
    def alloc(self, t, size=None, align=None):
        size = self.p.size(t) if size is None else size
        align = self.p.align(t) if align is None else align
        self.frame = (self.frame + align - 1) // align * align
        off = self.frame
        self.frame += size
        self.has_frame = True
        return 'frame' if off == 0 else 'frame + %d' % off

    def temp(self, t):
        return self.alloc(t)

    def fresh(self, base):
        self.tmp += 1
        n = '%s%d' % (base, self.tmp)
        while n in self.declared or n in self.p.used_names:
            self.tmp += 1
            n = '%s%d' % (base, self.tmp)
        self.declared.add(n)
        return n

    # ------------------------------------------------------------------------------------- the function
    def write(self):
        f = self.fn
        s = f.sym
        sig_params, ret = self.p.fn_sig(s)
        params = []
        pre = []
        if self.ret_struct:
            params.append('int result')
            self.declared.add('result')
        for prm in f.params:
            name = jname(prm.sym.name)
            self.declared.add(name)
            t = prm.type
            key = ('l', prm.sym.owner, prm.sym.name)
            if is_record(t):
                # a struct passed by value arrives as the address of the caller's copy
                self.vars[prm.sym.name] = Var(name, True, t, 'param')
                params.append('int %s' % name)
            elif key in self.p.addr_taken:
                arg = name + 'Arg'
                self.declared.add(arg)
                params.append('%s %s' % (self.p.jt(t), arg))
                self.vars[prm.sym.name] = Var(name, True, t, 'param')
                pre.append(('int %s = %s;' % (name, self.alloc(t)), 'Mem.set%s(%s, %s);' % (self.p.access(t), name, arg)))
            else:
                self.vars[prm.sym.name] = Var(name, False, t, 'param')
                params.append('%s %s' % (self.p.jt(t), name))
        body = []
        if s.varargs:
            # the arguments past the last named one: 8-byte slots in memory, as MSVC's va_list reads them
            params.append('Object... va')
            body.append('int vaList = Mem.varargs(va);')
            self.declared.update(('va', 'vaList'))
            self.has_frame = True
        for decl, store in pre:
            body.append(decl)
            body.append(store)
        self.block_stmts(f.body.stmts, body, 1)
        if ret != 'void' and self.can_complete(f.body):
            body.append('return 0;' if f.name == 'main' else 'return %s;  // (C: the function ends without a value)' % ('0' if ret not in ('boolean',) else 'false'))
        mods = 'static'
        head = '    %s %s %s(%s) {' % (mods, ret, jname(f.name), ', '.join(params))
        out = []
        if self.has_frame:
            size = (self.frame + 15) // 16 * 16
            inner = ['    ' + b for b in body]
            out = [head, '        int frame = Mem.enter(%d);' % size, '        try {'] + \
                  ['            ' + x if x else x for x in body] + \
                  ['        } finally {', '            Mem.leave(frame);', '        }', '    }']
        else:
            out = [head] + ['        ' + x if x else x for x in body] + ['    }']
        return out

    # ------------------------------------------------------------------------------------- statements
    def structure(self, stmts):
        """C's goto, where it nests, as Java's labeled break and continue: the statements from the first
        goto to a later label become `L: { ... }` (goto L is break L); the statements from a label to the
        last goto back to it become `L: while (true) { ...; break; }` (goto L is continue L)."""
        stmts = list(stmts)
        if not any(isinstance(s, ir.Labeled) for s in stmts):
            return stmts
        while True:
            labs = [i for i, s in enumerate(stmts) if isinstance(s, ir.Labeled) and id(s) not in self.labels_done]
            if not labs:
                return stmts
            j = labs[-1]
            lab = stmts[j]
            self.labels_done.add(id(lab))
            name = lab.label
            where = [i for i, s in enumerate(stmts) if any(isinstance(n, ir.Goto) and n.label == name and
                                                            id(n) not in self.goto_map for n in ir.walk(s))]
            fwd = [i for i in where if i < j]
            back = [i for i in where if i >= j]
            jl = jname(name)
            if fwd and back:
                raise Refused('goto %s both forward and backward in %s' % (name, self.fn.name))
            if fwd:
                a = min(fwd)
                for i in fwd:
                    for n in ir.walk(stmts[i]):
                        if isinstance(n, ir.Goto) and n.label == name:
                            self.goto_map[id(n)] = 'break ' + jl
                block = ir.Labeled(label=name, body=ir.Block(stmts=stmts[a:j]))
                self.labels_done.add(id(block))
                stmts = stmts[:a] + [block, lab.body] + stmts[j + 1:]
            elif back:
                b = max(back)
                region = [lab.body] + stmts[j + 1:b + 1]
                if any(self.loose_jump(s) for s in region):
                    raise Refused('a break or continue inside the loop goto %s makes in %s' % (name, self.fn.name))
                for i in back:
                    for n in ir.walk(stmts[i]):
                        if isinstance(n, ir.Goto) and n.label == name:
                            self.goto_map[id(n)] = 'continue ' + jl
                loop = ir.Labeled(label=name, body=ir.While(cond=ir.Lit(value=1, type=ir.I32),
                                                             body=ir.Block(stmts=region + [ir.Break(label=None)])))
                self.labels_done.add(id(loop))
                stmts = stmts[:j] + [loop] + stmts[b + 1:]
            else:
                stmts = stmts[:j] + [lab.body] + stmts[j + 1:]

    def loose_jump(self, s):
        """An unlabeled break/continue in s that leaves s (not inside a loop or switch of s)."""
        def walk(n):
            if isinstance(n, (ir.Break, ir.Continue)) and not n.label:
                return True
            if isinstance(n, (ir.While, ir.For, ir.DoWhile, ir.Switch)):
                return False
            return any(walk(c) for c in n.children() if isinstance(c, ir.Stmt))
        return walk(s)

    def block_stmts(self, stmts, out, ind):
        stmts = self.structure(stmts)
        for s in stmts:
            self.stmt(s, out)
            if self.abrupt(s):
                break                                    # (Java rejects what follows a jump in a block)

    def abrupt(self, s):
        if isinstance(s, (ir.Return, ir.Break, ir.Continue, ir.Throw, ir.Goto)):
            return True
        if isinstance(s, ir.Block):
            return any(self.abrupt(x) for x in s.stmts)
        return not self.can_complete(s) if isinstance(s, (ir.While, ir.For, ir.DoWhile)) else False

    def can_complete(self, s):
        """Java's 'can complete normally', for the statements C programs write."""
        if s is None:
            return True
        if isinstance(s, (ir.Return, ir.Break, ir.Continue, ir.Throw, ir.Goto)):
            return False
        if isinstance(s, ir.Block):
            for x in s.stmts:
                if not self.can_complete(x):
                    return False
            return True
        if isinstance(s, ir.If):
            if s.orelse is None:
                return True
            return self.can_complete(s.then) or self.can_complete(s.orelse)
        if isinstance(s, (ir.While, ir.For)):
            c = s.cond
            if c is None or self.const_true(c):
                return self.has_break(s.body)
            return True
        if isinstance(s, ir.DoWhile):
            if self.const_true(s.cond):
                return self.has_break(s.body)
            return True
        if isinstance(s, ir.Switch):
            # Java: a switch completes unless it has a default, no break leaves it, and its last group cannot
            if not any(c.default for c in s.cases) or not s.cases:
                return True
            if any(self.has_break(ir.Block(stmts=c.body)) for c in s.cases):
                return True
            return self.can_complete(ir.Block(stmts=s.cases[-1].body))
        if isinstance(s, ir.Labeled):
            brk = 'break ' + jname(s.label)
            if any(isinstance(n, ir.Goto) and self.goto_map.get(id(n)) == brk for n in ir.walk(s.body)):
                return True
            return self.can_complete(s.body)
        return True

    def const_true(self, c):
        c = strip(c)
        if isinstance(c, ir.Cast):
            c = strip(c.x)
        return isinstance(c, ir.Lit) and isinstance(c.value, (int, bool)) and c.value not in (0, False) and c.value is not None

    def has_break(self, body):
        """A break that leaves this loop (not one inside a nested loop or switch)."""
        def walk(n):
            if isinstance(n, ir.Break):
                return True
            if isinstance(n, (ir.While, ir.For, ir.DoWhile, ir.Switch)):
                return False
            return any(walk(c) for c in n.children() if isinstance(c, ir.Stmt))
        return walk(body)

    def sub(self, s):
        """A statement as the body of if/while/for: its lines, indented one more."""
        out = []
        if isinstance(s, ir.Block):
            self.block_stmts(s.stmts, out, 0)
        else:
            self.stmt(s, out)
        return ['    ' + x for x in out]

    def stmt(self, s, out):
        if isinstance(s, ir.Block):
            inner = []
            self.block_stmts(s.stmts, inner, 0)
            out.append('{')
            out += ['    ' + x for x in inner]
            out.append('}')
        elif isinstance(s, ir.Local):
            self.local(s, out)
        elif isinstance(s, ir.ExprStmt):
            self.expr_stmt(s.expr, out)
        elif isinstance(s, ir.Return):
            self.ret(s, out)
        elif isinstance(s, ir.If):
            out.append('if (%s) {' % self.cond(s.cond).code)
            out += self.sub(s.then)
            e = s.orelse
            while isinstance(e, ir.If):
                out.append('} else if (%s) {' % self.cond(e.cond).code)
                out += self.sub(e.then)
                e = e.orelse
            if e is not None:
                out.append('} else {')
                out += self.sub(e)
            out.append('}')
        elif isinstance(s, ir.While):
            out.append('while (%s) {' % self.loop_cond(s.cond))
            out += self.sub(s.body)
            out.append('}')
        elif isinstance(s, ir.DoWhile):
            out.append('do {')
            out += self.sub(s.body)
            out.append('} while (%s);' % self.loop_cond(s.cond))
        elif isinstance(s, ir.For):
            self.for_(s, out)
        elif isinstance(s, ir.Switch):
            self.switch(s, out)
        elif isinstance(s, ir.Break):
            out.append('break %s;' % jname(s.label) if s.label else 'break;')
        elif isinstance(s, ir.Continue):
            out.append('continue %s;' % jname(s.label) if s.label else 'continue;')
        elif isinstance(s, ir.Empty):
            pass
        elif isinstance(s, ir.Goto):
            if id(s) in self.goto_map:
                out.append(self.goto_map[id(s)] + ';')
            else:
                raise Refused('goto %s in %s: not a jump that nests' % (s.label, self.fn.name if self.fn else 'an initialiser'))
        elif isinstance(s, ir.Labeled):
            if id(s) not in self.labels_done:
                raise Refused('a label (%s) for goto in %s: not one that nests' % (s.label, self.fn.name))
            inner = []
            self.stmt(s.body, inner)
            if inner:
                inner[0] = '%s: %s' % (jname(s.label), inner[0])
            out += inner
        else:
            raise Refused('statement %s' % type(s).__name__)

    def loop_cond(self, c):
        if c is None or self.const_true(c):
            return 'true'
        return self.cond(c).code

    def for_(self, s, out):
        inits = []
        simple = True
        for i in s.init or []:
            if isinstance(i, ir.Local):
                v_lines = []
                self.local(i, v_lines)
                if len(v_lines) != 1 or not v_lines[0].endswith(';'):
                    simple = False
                inits.append(v_lines)
            elif isinstance(i, ir.ExprStmt):
                inits.append([self.expr_text(i.expr) + ';'])
            else:
                simple = False
        decls = [l[0] for l in inits if l and l[0].split(' ')[0] in ('int', 'long', 'byte', 'short', 'char', 'float', 'double', 'boolean')]
        updates = [self.expr_text(u) for u in s.update or []]
        if simple and all(len(l) == 1 for l in inits) and (len(decls) == len(inits) <= 1 or not decls):
            init_text = ', '.join(l[0][:-1] for l in inits) if not decls else inits[0][0][:-1]
            if len(inits) > 1 and not decls:
                init_text = ', '.join(l[0][:-1] for l in inits)
            out.append('for (%s; %s; %s) {' % (init_text, '' if s.cond is None else self.loop_cond(s.cond) if not self.const_true(s.cond) else '',
                                               ', '.join(updates)))
            out += self.sub(s.body)
            out.append('}')
            return
        # an initialiser Java's for cannot hold: before the loop, in a block of its own
        block = []
        for l in inits:
            block += l
        block.append('for (; %s; %s) {' % ('' if s.cond is None else self.loop_cond(s.cond), ', '.join(updates)))
        block += self.sub(s.body)
        block.append('}')
        out.append('{')
        out += ['    ' + x for x in block]
        out.append('}')

    def switch(self, s, out):
        subj = self.value(s.expr)
        st = s.expr.type
        if _kind(st) in ('i64', 'u64'):
            raise Refused('a switch on a 64-bit value')
        out.append('switch (%s) {' % subj.code)
        jt = subj.jt if subj.jt in ('int', 'char', 'byte', 'short') else 'int'
        for c in s.cases:
            heads = []
            for lab in c.labels:
                lab = strip(lab)
                if not isinstance(lab, ir.Lit):
                    raise Refused('a case label that is not a constant')
                heads.append('case %s:' % int_lit(int(lab.value), jt).code)
            if c.default:
                heads.append('default:')
            body = []
            self.block_stmts(c.body, body, 0)
            out.append('    ' + ' '.join(heads))
            out += ['        ' + x for x in body]
        out.append('}')

    def local(self, s, out):
        name = jname(s.sym.name)
        if name in self.declared:
            name = self.fresh(name + '_')
        self.declared.add(name)
        t = s.type
        mods = s.mods or []
        if 'extern' in mods:
            return
        if 'static' in mods:
            self.static_local(s, name)
            return
        key = ('l', s.sym.owner, s.sym.name)
        mem = t.kind in ('array', 'class', 'record') or key in self.p.addr_taken
        v = Var(name, mem, t, 'local')
        if mem:
            addr = self.alloc(t)
            out.append('int %s = %s;  // %s' % (name, addr, _ctype_text(t, s.sym.name)))
            self.vars[s.sym.name] = v
            if s.init is not None:
                out += self.init_place(name, t, s.init, fresh=True)
            return
        self.vars[s.sym.name] = v
        jt = self.p.jt(t)
        if s.init is None:
            out.append('%s %s = %s;' % (jt, name, self.zero(jt)))
        else:
            val = self.conv(self.value(s.init), s.init.type, t)
            out.append('%s %s = %s;' % (jt, name, val.code))

    def static_local(self, s, name):
        g = ir.FieldDecl(sym=ir.Sym('global', '%s_%s' % (self.fn.name, s.sym.name), None, True, s.type), type=s.type,
                         init=s.init, mods=['static'])
        if ('l', s.sym.owner, s.sym.name) in self.p.addr_taken:
            self.p.addr_taken.add(('g', g.sym.name))
        self.p.declare_global(g)
        self.p.init_global(g)
        self.vars[s.sym.name] = self.p.globals[g.sym.name]

    def zero(self, jt):
        return {'boolean': 'false', 'float': '0.0f', 'double': '0.0', 'long': '0L'}.get(jt, '0')

    def init_place(self, addr, t, init, fresh=False):
        """Statements storing initialiser `init` into the memory of type t at `addr`."""
        init = strip(init)
        out = []
        if isinstance(init, ir.Lit) and isinstance(init.value, str) and t.kind == 'array':
            n = t.info.get('count', len(init.value) + 1)
            out.append('Mem.setString(%s, %s, %d);' % (addr, java_string(init.value), n))
            return out
        if isinstance(init, ir.NewArray) and t.kind == 'array':
            items = init.init or []
            n = t.info.get('count', len(items))
            leaves = self.flat_leaves(items, t.elem)
            if leaves is not None:
                et = leaves[1]
                total = self.p.size(t) // max(self.p.size(et), 1)
                if len(leaves[0]) < total:
                    out.append('Mem.fill(%s, 0, %d);' % (addr, self.p.size(t)))
                if leaves[0]:
                    vals = [self.conv(self.value(x), x.type, et).code if not self.small_lit(x, et) else self.small_lit(x, et)
                            for x in leaves[0]]
                    acc = self.p.access(et)
                    out += self.wrap_call('Mem.set%ss(%s, ' % (acc, addr), vals, ');')
                return out
            esz = self.p.size(t.elem)
            if len(items) < n:
                out.append('Mem.fill(%s, 0, %d);' % (addr, self.p.size(t)))
            for i, x in enumerate(items):
                out += self.init_place(self.offset(addr, i * esz), t.elem, x)
            return out
        if isinstance(init, ir.New) and is_record(t) and init.sym is not None and init.sym.name.startswith('<aggregate'):
            lay = self.p.layout(t)
            fields = lay.info['fields']
            ln = self.p.layout_names[self.p.rec_name(t)]
            if init.sym.name.startswith('<aggregate:'):
                fname = init.sym.name[len('<aggregate:'):-1]
                fields = [f for f in fields if f['name'] == fname]
            if len(init.args or []) < len(fields) or lay.info['role'] == 'union':
                out.append('Mem.fill(%s, 0, %s.SIZE);' % (addr, ln))
            for f, x in zip(fields, init.args or []):
                out += self.init_place(self.field_addr(addr, ln, f), f['type'], x)
            return out
        if is_record(t):
            out.append('Mem.copy(%s, %s, %s);' % (addr, self.value(init).code, self.p.size_code(t)))
            return out
        val = self.conv(self.value(init), init.type, t)
        out.append('Mem.set%s(%s, %s);' % (self.p.access(t), addr, val.code))
        return out

    def small_lit(self, x, et):
        """An integer literal element of an array initialiser, written as an int for Mem.setBytes & co."""
        x = strip(x)
        if isinstance(x, ir.Cast) and x.implicit:
            x = strip(x.x)
        if isinstance(x, ir.Lit) and isinstance(x.value, int) and not isinstance(x.value, bool) and is_int(et):
            v = x.value
            jt = self.p.jt(et)
            if jt == 'long':
                return int_lit(v, 'long').code
            if x.text and x.text.startswith("'") and 0 <= v < 128:
                return x.text if x.text != "'\\0'" else '0'
            if is_unsigned(et) or (jt in ('byte', 'short') and v >= 0):
                b = _JBITS[jt] if jt != 'char' else 16
                v &= (1 << b) - 1
                return ('0x%02x' % v) if (jt == 'byte' and v > 9) else str(v)
            return int_lit(v, 'int').code
        return None

    def flat_leaves(self, items, et):
        """The scalar leaves of a (nested) array initialiser when it covers whole rows -> ([exprs], scalar type)."""
        if et.kind == 'array':
            rows = []
            n = et.info.get('count')
            for x in items:
                x = strip(x)
                if not isinstance(x, ir.NewArray):
                    return None
                got = self.flat_leaves(x.init or [], et.elem)
                if got is None or n is None:
                    return None
                leaves, base = got
                per = self.p.size(et) // self.p.size(base)
                if len(leaves) != per and x is not items[-1]:
                    return None
                rows += leaves
            return rows, self._scalar(et)
        if is_record(et):
            return None
        return list(items), et

    def _scalar(self, t):
        while t.kind == 'array':
            t = t.elem
        return t

    def wrap_call(self, head, vals, tail, width=100):
        line = head + ', '.join(vals) + tail
        if len(line) <= width:
            return [line]
        out = [head.rstrip()]
        cur = '        '
        for i, v in enumerate(vals):
            piece = v + (', ' if i < len(vals) - 1 else '')
            if len(cur) + len(piece) > width:
                out.append(cur.rstrip())
                cur = '        '
            cur += piece
        out.append(cur.rstrip() + tail)
        return out

    def offset(self, addr, off):
        if off == 0:
            return addr
        m = re.match(r'^(.*) \+ (\d+)$', addr)
        if m and '(' not in m.group(1) and '?' not in m.group(1):
            return '%s + %d' % (m.group(1), int(m.group(2)) + off)
        if addr == 'frame':
            return 'frame + %d' % off
        return '%s + %d' % (addr, off)

    def field_addr(self, addr, ln, f):
        if f['offset'] == 0 and False:
            return addr
        fname = jname(f['name']) if f['name'] != 'SIZE' else 'SIZE_'
        return '%s + %s.%s' % (addr, ln, fname)

    def ret(self, s, out):
        if s.expr is None:
            if self.fn is not None and self.fn.ret is not None and self.fn.ret.kind != 'void':
                out.append('return %s;  // (C: a return without a value)' % self.zero(self.p.jt(self.fn.ret)))
            else:
                out.append('return;')
            return
        if self.ret_struct:
            out.append('return Mem.copy(result, %s, %s);' % (self.value(s.expr).code, self.p.size_code(self.fn.ret)))
            return
        if self.fn is not None and self.fn.ret is not None and self.fn.ret.kind == 'void':
            self.expr_stmt(s.expr, out)
            out.append('return;')
            return
        v = self.conv(self.value(s.expr), s.expr.type, self.fn.ret)
        out.append('return %s;' % v.code)

    def expr_stmt(self, e, out):
        e = strip(e)
        if isinstance(e, ir.Cast) and e.type is not None and e.type.kind == 'void':
            e = strip(e.x)
            if isinstance(e, (ir.Name, ir.Lit)):
                return
        if isinstance(e, ir.Binary) and e.op == ',':
            self.expr_stmt(e.left, out)
            self.expr_stmt(e.right, out)
            return
        if isinstance(e, (ir.Lit, ir.Name)):
            return
        if isinstance(e, ir.Binary) and e.op in ('&&', '||') and not self.pure(e.right):
            # `a || f();` as a statement (assert's expansion): if (!a) f();
            c = self.cond(e.left)
            if e.op == '||':
                c = J('!' + c.at(P_UNARY), P_UNARY, 'boolean') if not c.code.endswith(' != 0') else \
                    J(c.code[:-5] + ' == 0', P_EQ, 'boolean')
            inner = []
            self.expr_stmt(e.right, inner)
            out.append('if (%s) {' % c.code)
            out += ['    ' + x for x in inner]
            out.append('}')
            return
        if isinstance(e, ir.Cond):
            if self.statement_expr(e.then) and self.statement_expr(e.orelse):
                out.append('if (%s) {' % self.cond(e.cond).code)
                self.expr_stmt(e.then, [x for x in []])
                a, b = [], []
                self.expr_stmt(e.then, a)
                self.expr_stmt(e.orelse, b)
                out += ['    ' + x for x in a]
                out.append('} else {')
                out += ['    ' + x for x in b]
                out.append('}')
                return
        if isinstance(e, ir.Assign) and e.op is not None and not self.pure_place(e.target):
            place = self.place(e.target)
            if place.mem:
                t = self.fresh('at')
                out.append('{')
                out.append('    int %s = %s;' % (t, place.java))
                out.append('    %s;' % self.compound(Place(t, True, place.type), e).code)
                out.append('}')
                return
        text = self.expr_text(e)
        if not self.statement_expr(e) and not text.startswith(('Mem.', 'printf')):
            if self.java_statement(text):
                out.append(text + ';')
            else:
                out.append('discard(%s);' % text)
            return
        out.append(text + ';')

    def java_statement(self, text):
        """Is this Java text a statement expression (an assignment, ++/--, a call, new)?"""
        if re.match(r'^[\w.]+\(.*\)$', text, re.S) and _balanced_call(text):
            return True
        if re.match(r'^(\+\+|--)[\w.\[\]]+$', text) or re.match(r'^[\w.\[\]]+(\+\+|--)$', text):
            return True
        if re.match(r'^[\w.\[\]]+ (=|\+=|-=|\*=|/=|%=|&=|\|=|\^=|<<=|>>=|>>>=) ', text):
            return True
        return False

    def statement_expr(self, e):
        e = strip(e)
        return isinstance(e, (ir.Assign, ir.Call)) or (isinstance(e, ir.Unary) and e.op in ('++', '--'))

    def expr_text(self, e):
        e = strip(e)
        if isinstance(e, ir.Unary) and e.op in ('++', '--'):
            return self.incdec(e, as_statement=True).code
        return self.value(e).code

    # ------------------------------------------------------------------------------------- places
    def var(self, s):
        if s.name in self.vars and s.kind in ('local', 'param'):
            return self.vars[s.name]
        if s.kind in ('local', 'param') and s.name in self.vars:
            return self.vars[s.name]
        g = self.p.globals.get(s.name)
        if g is not None:
            return g
        raise Refused('unknown variable %s (%s, from %s)' % (s.name, s.kind, s.owner))

    def place(self, e):
        """An lvalue -> Place (a Java variable, or memory at an address)."""
        e = strip(e)
        if isinstance(e, ir.Name):
            v = self.var(e.sym)
            return Place(v.java, v.mem, v.type)
        if isinstance(e, ir.Unary) and e.op == '*':
            return Place(self.value(e.x).code, True, e.type, prec=self.value(e.x).prec)
        if isinstance(e, ir.Index):
            base = self.value(e.arr)                       # (an array decays to its address)
            et = e.type
            idx = self.conv(self.value(e.index), e.index.type, ir.I32)
            size = self.p.size(et) if not is_record(et) else None
            scale = self.p.size_code(et)
            if isinstance(strip(e.index), ir.Lit) or _is_int_text(idx.code):
                n = int(idx.code.strip('()')) if _is_int_text(idx.code) else None
                if n is not None and size is not None:
                    return Place(self.offset(base.at(P_ADD), n * size), True, et, prec=P_ADD)
            term = idx.at(P_MUL) if scale == '1' else '%s * %s' % (idx.at(P_MUL), scale)
            return Place('%s + %s' % (base.at(P_ADD), term), True, et, prec=P_ADD)
        if isinstance(e, ir.Select):
            obj = e.obj
            ot = obj.type
            if not is_record(ot):
                raise Refused('a field of a non-struct')
            base = self.addr(obj)
            f = self.p.field_off(ot, e.sym.name)
            ln = self.p.layout_names[self.p.rec_name(ot)]
            return Place(self.field_addr(base.at(P_ADD), ln, f), True, f['type'] if e.type is None else e.type, prec=P_ADD)
        if isinstance(e, ir.Cast):
            return self.place(e.x)
        raise Refused('an lvalue %s' % type(e).__name__)

    def pure_place(self, e):
        return self.pure(e)

    def pure(self, e):
        for n in ir.walk(e):
            if isinstance(n, (ir.Assign, ir.Call)) or (isinstance(n, ir.Unary) and n.op in ('++', '--')):
                return False
        return True

    def addr(self, e):
        """The address of an lvalue in memory (an array or struct: the value itself)."""
        e = strip(e)
        if isinstance(e, ir.Unary) and e.op == '*':
            return self.value(e.x)
        if isinstance(e, (ir.Call, ir.New, ir.Assign, ir.Cond)) and is_record(e.type):
            return self.value(e)
        pl = self.place(e)
        if not pl.mem:
            raise Refused('the address of %s, which is not in memory' % pl.java)
        return J(pl.java, pl.prec, 'int')

    def load(self, pl):
        if not pl.mem:
            return J(pl.java, P_ATOM, self.p.jt(pl.type))
        t = pl.type
        if t.kind in ('array', 'class', 'record'):
            return J(pl.java, pl.prec, 'int')
        return J('Mem.get%s(%s)' % (self.p.access(t), pl.java), P_POST, self.p.jt(t))

    def store(self, pl, val):
        """Assign val (a J already of the place's type) -> J of the assignment."""
        if not pl.mem:
            return J('%s = %s' % (pl.java, val.at(P_ASSIGN)), P_ASSIGN, self.p.jt(pl.type))
        t = pl.type
        if is_record(t):
            return J('Mem.copy(%s, %s, %s)' % (pl.java, val.code, self.p.size_code(t)), P_POST, 'int')
        return J('Mem.set%s(%s, %s)' % (self.p.access(t), pl.java, val.code), P_POST, self.p.jt(t))

    # ------------------------------------------------------------------------------------- values
    def cond(self, e):
        """A C condition as a Java boolean."""
        e = strip(e)
        j = self.value(e)
        return self.truth(j, e.type)

    def truth(self, j, t):
        if j.jt == 'boolean':
            return j
        if is_float(t):
            return J('%s != 0' % j.at(P_EQ + 1), P_EQ, 'boolean')
        return J('%s != 0' % j.at(P_EQ + 1), P_EQ, 'boolean')

    def as_value(self, j, t):
        """A J that may be a Java boolean as the C value of type t (1/0)."""
        if j.jt == 'boolean' and _kind(t) != 'bool':
            jt = self.p.jt(t)
            one, zero = ('1L', '0L') if jt == 'long' else ('1', '0')
            return J('%s ? %s : %s' % (j.at(P_COND + 1), one, zero), P_COND, jt)
        return j

    def value(self, e):
        m = getattr(self, 'x_' + type(e).__name__, None)
        if m is None:
            raise Refused('expression %s' % type(e).__name__ + (' (%s)' % e.kind if isinstance(e, ir.Unsupported) else ''))
        return m(e)

    def x_Paren(self, e):
        # the grouping the source chose is kept where it groups an operator (readability, not precedence)
        j = self.value(e.x)
        if j.prec < P_UNARY and j.jt != 'void':
            return J('(%s)' % j.code, P_ATOM, j.jt, j.fresh)
        return j

    def x_Unsupported(self, e):
        raise Refused('%s (line %s): the front end did not lower it' % (e.kind, e.line))

    def x_Lit(self, e):
        v = e.value
        t = e.type
        if isinstance(v, str) and is_float(t):
            v = float(v)                                 # (L2 keeps inf and nan as text)
        if isinstance(v, bool):
            if _kind(t) == 'bool':
                return J('true' if v else 'false', P_ATOM, 'boolean')
            return J('1' if v else '0', P_ATOM, 'int')
        if v is None:
            return J('0', P_ATOM, 'int')
        if isinstance(v, str):
            return J('Mem.%s(%s)' % ('wstr' if is_wide(e) else 'str', string_lit(e)), P_POST, 'int')
        jt = self.p.jt(t) if t is not None and t.kind not in ('any',) else 'int'
        if isinstance(v, float) or is_float(t):
            return float_lit(float(v), jt if jt in ('float', 'double') else 'double')
        if e.text and e.text.startswith('sizeof(') and is_int(t):
            pass
        if e.text and e.text.startswith("'") and jt == 'int':
            return J(e.text, P_ATOM, 'char')
        return int_lit(int(v), jt)

    def x_Name(self, e):
        v = self.var(e.sym)
        if v.mem and (v.type.kind in ('array', 'class', 'record')):
            return J(v.java, P_ATOM, 'int')
        return self.load(Place(v.java, v.mem, v.type))

    def x_Select(self, e):
        return self.load(self.place(e))

    def x_Index(self, e):
        return self.load(self.place(e))

    def x_TypeName(self, e):
        raise Refused('a type used as a value')

    def x_AddressOf(self, e):
        return J(self.p.fn_const(e.sym.name), P_ATOM, 'int')

    def x_Unary(self, e):
        op = e.op
        if op == '*':
            if e.type is not None and e.type.kind in ('fn',):
                return self.value(e.x)
            return self.load(self.place(e))
        if op == '&':
            x = strip(e.x)
            if isinstance(x, ir.AddressOf):
                return self.value(x)
            return self.addr(x)
        if op in ('++', '--'):
            return self.incdec(e)
        if op == '!':
            c = self.cond(e.x)
            if c.code.endswith(' != 0') and c.prec == P_EQ:
                return J(c.code[:-5] + ' == 0', P_EQ, 'boolean')
            return J('!' + c.at(P_UNARY), P_UNARY, 'boolean')
        x = self.as_value(self.value(e.x), e.x.type)
        jt = self.p.jt(e.type)
        if op == '-':
            code = '-' + x.at(P_UNARY)
            if code.startswith('--'):
                code = '-(%s)' % x.code
            return self.narrow(J(code, P_UNARY, _promoted(x.jt)), jt)
        if op == '+':
            return x
        if op == '~':
            return self.narrow(J('~' + x.at(P_UNARY), P_UNARY, _promoted(x.jt)), jt)
        raise Refused('unary %s' % op)

    def narrow(self, j, jt):
        """Java widened a byte/short/char operand to int: back to the C type's Java type."""
        if j.jt == jt or jt in ('int', 'long', 'float', 'double', 'boolean') and j.jt == jt:
            return j
        if jt in ('byte', 'short', 'char') and j.jt in ('int', 'long'):
            return J('(%s) %s' % (jt, j.at(P_UNARY)), P_UNARY, jt)
        if jt == 'int' and j.jt == 'long':
            return J('(int) %s' % j.at(P_UNARY), P_UNARY, 'int')
        return J(j.code, j.prec, jt) if j.jt in ('int',) and jt == 'int' else j

    def incdec(self, e, as_statement=False):
        t = e.type
        pl = self.place(e.x)
        delta = 1
        if is_ptr(t):
            delta = self.p.size(t.elem) if t.kind == 'ptr' else 1
        sign = '+' if e.op == '++' else '-'
        if not pl.mem:
            if delta == 1 or not is_ptr(t):
                if _kind(t) == 'bool':
                    raise Refused('++ on a bool')
                code = (pl.java + e.op) if e.postfix else (e.op + pl.java)
                return J(code, P_POST if e.postfix else P_UNARY, self.p.jt(t))
            if as_statement or not e.postfix:
                return J('%s %s= %d' % (pl.java, sign, delta), P_ASSIGN, 'int')
            return J('(%s %s= %d) %s %d' % (pl.java, sign, delta, '-' if sign == '+' else '+', delta), P_ADD, 'int')
        acc = self.p.access(t)
        d = delta if sign == '+' else -delta
        jt = self.p.jt(t)
        dl = int_lit(d, 'long').code if jt == 'long' else (float_lit(float(d), jt).code if jt in ('float', 'double') else str(d))
        fn = 'Mem.postAdd%s' if e.postfix and not as_statement else 'Mem.add%s'
        return J('%s(%s, %s)' % (fn % acc, pl.java, dl), P_POST, jt)

    def x_Binary(self, e):
        op = e.op
        if op == ',':
            l = self.value(e.left)
            r = self.value(e.right)
            return J('seq(%s, %s)' % (l.code, r.code), P_POST, r.jt)
        if op in ('&&', '||'):
            l, r = self.cond(e.left), self.cond(e.right)
            p = _BINPREC[op]
            return J('%s %s %s' % (l.at(p), op, r.at(p + 1)), p, 'boolean')
        lt, rt = e.left.type, e.right.type
        if op in ('<', '<=', '>', '>=', '==', '!='):
            return self.compare(e, op)
        # pointer arithmetic
        if op in ('+', '-') and is_ptr(lt) and is_int(rt):
            return self.ptr_add(self.value(e.left), self.value(e.right), rt, lt, op)
        if op == '+' and is_int(lt) and is_ptr(rt):
            return self.ptr_add(self.value(e.right), self.value(e.left), lt, rt, '+')
        if op == '-' and is_ptr(lt) and is_ptr(rt):
            size = self.p.size(lt.elem) if lt.kind == 'ptr' else 1
            diff = '%s - %s' % (self.value(e.left).at(P_ADD), self.value(e.right).at(P_ADD + 1))
            code = diff if size == 1 else '(%s) / %d' % (diff, size)
            j = J(code, P_ADD if size == 1 else P_MUL, 'int')
            return self.conv(j, ir.I32, e.type)
        t = e.type
        l = self.as_value(self.value(e.left), lt)
        r = self.as_value(self.value(e.right), rt)
        jt = self.p.jt(t)
        if op in ('<<', '>>', '>>>'):
            jop = '>>>' if op == '>>' and is_unsigned(t) else op
            p = P_SHIFT
            rr = r if r.jt in ('int', 'long', 'char', 'short', 'byte') else r
            if r.jt == 'long':
                rr = J('(int) %s' % r.at(P_UNARY), P_UNARY, 'int')
            return self.narrow(J('%s %s %s' % (l.at(p), jop, rr.at(p + 1)), p, _promoted(l.jt)), jt)
        if is_unsigned(t) and op in ('/', '%') and _kind(t) in ('u32', 'u64'):
            w = 'Integer' if _kind(t) == 'u32' else 'Long'
            fn = 'divideUnsigned' if op == '/' else 'remainderUnsigned'
            return J('%s.%s(%s, %s)' % (w, fn, l.code, r.code), P_POST, jt)
        p = _BINPREC[op]
        res = J('%s %s %s' % (l.at(p), op, r.at(p + 1)), p, _promoted2(l.jt, r.jt))
        return self.narrow(res, jt) if jt in ('byte', 'short', 'char') else res

    def ptr_add(self, base, idx, it, pt, op):
        size = self.p.size(pt.elem) if pt.kind == 'ptr' else 1
        i = self.conv(idx, it, ir.I32)
        if _is_int_text(i.code):
            n = int(i.code.strip('()')) * size
            if op == '-':
                n = -n
            if n == 0:
                return base
            return J('%s %s %d' % (base.at(P_ADD), '+' if n > 0 else '-', abs(n)), P_ADD, 'int')
        term = i.at(P_MUL + (op == '-')) if size == 1 else '%s * %d' % (i.at(P_MUL), size)
        if size != 1 and op == '-':
            term = '(%s)' % term
        return J('%s %s %s' % (base.at(P_ADD), op, term), P_ADD, 'int')

    def compare(self, e, op):
        lt, rt = e.left.type, e.right.type
        l = self.as_value(self.value(e.left), lt)
        r = self.as_value(self.value(e.right), rt)
        p = _BINPREC[op]
        if op in ('<', '<=', '>', '>=') and (is_unsigned(lt) and _kind(lt) in ('u32', 'u64')):
            w = 'Integer' if _kind(lt) == 'u32' else 'Long'
            return J('%s.compareUnsigned(%s, %s) %s 0' % (w, l.code, r.code, op), P_REL, 'boolean')
        if l.jt == 'boolean' and r.jt == 'boolean':
            return J('%s %s %s' % (l.at(p), op, r.at(p + 1)), p, 'boolean')
        if l.jt == 'boolean':
            l = self.as_value(l, ir.I32)
        if r.jt == 'boolean':
            r = self.as_value(r, ir.I32)
        return J('%s %s %s' % (l.at(p), op, r.at(p + 1)), p, 'boolean')

    def x_Assign(self, e):
        if e.op is None:
            pl = self.place(e.target)
            t = e.target.type
            if is_record(t):
                return J('Mem.copy(%s, %s, %s)' % (self.addr(e.target).code, self.value(e.value).code,
                                                     self.p.size_code(t)), P_POST, 'int')
            val = self.conv(self.value(e.value), e.value.type, t)
            return self.store(pl, val)
        pl = self.place(e.target)
        if pl.mem and not self.pure(e.target):
            raise Refused('a compound assignment through an address with side effects, inside an expression')
        return self.compound(pl, e)

    def compound(self, pl, e):
        t = e.target.type
        op = e.op
        vt = e.value.type
        if is_ptr(t):
            size = self.p.size(t.elem) if t.kind == 'ptr' else 1
            i = self.conv(self.value(e.value), vt, ir.I32)
            term = i.code if size == 1 else '%s * %d' % (i.at(P_MUL), size)
            if not pl.mem:
                return J('%s %s= %s' % (pl.java, op, term), P_ASSIGN, 'int')
            return self.store(pl, J('%s %s %s' % (self.load(pl).at(P_ADD), op, i.at(P_MUL) if size == 1 else '(%s)' % term
                                               if op == '-' else term), P_ADD, 'int'))
        jt = self.p.jt(t)
        val = self.as_value(self.value(e.value), vt)
        simple_ops = ('+', '-', '*', '&', '|', '^', '<<')
        if not pl.mem and is_int(t) and op in simple_ops and is_int(vt) and \
                (_BITS[_kind(vt)] <= 32 or jt == 'long'):
            v = val
            if op == '<<' and v.jt == 'long':
                v = J('(int) %s' % v.at(P_UNARY), P_UNARY, 'int')
            return J('%s %s= %s' % (pl.java, op, v.at(P_ASSIGN)), P_ASSIGN, jt)
        if not pl.mem and _kind(t) in ('i32', 'i64', 'f64', 'f32') and _kind(vt) == _kind(t) and op in ('/', '%', '>>'):
            return J('%s %s= %s' % (pl.java, op, val.at(P_ASSIGN)), P_ASSIGN, jt)
        if not pl.mem and _kind(t) in ('u32', 'u64') and _kind(vt) == _kind(t) and op == '>>':
            return J('%s >>>= %s' % (pl.java, val.at(P_ASSIGN)), P_ASSIGN, jt)
        if not pl.mem and is_float(t) and is_float(vt) and _kind(t) == _kind(vt):
            return J('%s %s= %s' % (pl.java, op, val.at(P_ASSIGN)), P_ASSIGN, jt)
        # the general form: target = (T) (target op value), in the computation type
        ct = vt if op not in ('<<', '>>') else _promote_type(t)
        if is_float(t) and not is_float(vt):
            ct = t
        cur = self.conv(self.load(pl), t, ct)
        fake = ir.Binary(op=op, left=_Code(cur, ct), right=_Code(val, vt), type=ct)
        res = self.value(fake)
        return self.store(pl, self.conv(res, ct, t))

    def x__Code(self, e):
        return e.j

    def x_Cond(self, e):
        c = self.cond(e.cond)
        t = e.type
        if is_record(t):
            a, b = self.value(e.then), self.value(e.orelse)
            return J('%s ? %s : %s' % (c.at(P_COND + 1), a.at(P_COND + 1), b.at(P_COND)), P_COND, 'int')
        a = self.conv(self.value(e.then), e.then.type, t)
        b = self.conv(self.value(e.orelse), e.orelse.type, t)
        jt = self.p.jt(t) if t.kind != 'void' else a.jt
        if t.kind == 'void':
            raise Refused('a conditional expression of type void used as a value')
        av, bv = a, b
        if jt in ('byte', 'short', 'char'):
            res = J('%s ? %s : %s' % (c.at(P_COND + 1), av.at(P_COND + 1), bv.at(P_COND)), P_COND, 'int')
            return J('(%s) (%s)' % (jt, res.code), P_UNARY, jt)
        return J('%s ? %s : %s' % (c.at(P_COND + 1), av.at(P_COND + 1), bv.at(P_COND)), P_COND, jt)

    def x_Cast(self, e):
        x = strip(e.x)
        t = e.type
        st = x.type
        if t.kind == 'void':
            return self.value(x)
        if st is not None and st.kind == 'array' and is_ptr(t):
            if isinstance(x, ir.Lit) and isinstance(x.value, str):
                return J('Mem.%s(%s)' % ('wstr' if is_wide(x) else 'str', string_lit(x)), P_POST, 'int')
            return self.addr(x)
        if isinstance(x, ir.Lit) and x.value is None:
            return J('0', P_ATOM, 'int')
        if isinstance(x, ir.AddressOf):
            return self.value(x)
        j = self.value(x)
        return self.conv(j, st, t)

    def conv(self, j, s, t):
        """Convert a Java value j of C type s to C type t."""
        if t is None or s is None:
            return j
        sk, tk = _kind(s), _kind(t)
        if tk == 'void':
            return j
        if tk == 'bool':
            if j.jt == 'boolean':
                return j
            return J('%s != 0' % j.at(P_EQ + 1), P_EQ, 'boolean')
        if j.jt == 'boolean':
            j = J('%s ? 1 : 0' % j.at(P_COND + 1), P_COND, 'int')
            sk = 'i32'
        tj = self.p.jt(t)
        if sk in ('ptr', 'fnptr', 'array', 'null', 'class', 'record'):
            sk = 'i32'                                      # an address is an int
        if tk in ('ptr', 'fnptr', 'array', 'class', 'record'):
            tk = 'i32'
        if sk == tk:
            return j if j.jt == tj or tj == 'int' and j.jt == 'char' and False else self.retype(j, tj)
        if tk in ('f32', 'f64', 'f80'):
            fj = 'float' if tk == 'f32' else 'double'
            if sk in ('f32', 'f64', 'f80'):
                if _kind(s) in ('f64', 'f80') and fj == 'double' or (sk == tk):
                    return j
                return J('(%s) %s' % (fj, j.at(P_UNARY)), P_UNARY, fj)
            if sk == 'u32':
                return J('(%s) Integer.toUnsignedLong(%s)' % (fj, j.code), P_UNARY, fj)
            if sk == 'u64':
                return J('%sunsignedToDouble(%s)' % ('(float) ' if fj == 'float' else '', j.code),
                         P_UNARY if fj == 'float' else P_POST, fj)
            if sk == 'u8':
                return J('(%s) (%s & 0xFF)' % (fj, j.code), P_UNARY, fj)
            return J('(%s) %s' % (fj, j.at(P_UNARY)), P_UNARY, fj)
        # to an integer
        if sk in ('f32', 'f64', 'f80'):
            if tk == 'u32':
                return J('(int) (long) %s' % j.at(P_UNARY), P_UNARY, 'int')
            if tk == 'u64':
                return J('doubleToUnsigned(%s)' % j.code, P_POST, 'long')
            if tk in ('i64',):
                return J('(long) %s' % j.at(P_UNARY), P_UNARY, 'long')
            if tk in ('i32',):
                return J('(int) %s' % j.at(P_UNARY), P_UNARY, 'int')
            return J('(%s) (int) %s' % (tj, j.at(P_UNARY)), P_UNARY, tj)
        sb, tb = _BITS[sk], _BITS[tk]
        if tb <= sb:                                       # narrowing or the same width: the low bits
            if tj == j.jt or (tj == 'int' and j.jt in ('char',) and False):
                return J(j.code, j.prec, tj)
            if j.jt in ('byte', 'short', 'char') and tj == 'int' and tb == sb:
                return self.widen(j, sk, tj)
            return J('(%s) %s' % (tj, j.at(P_UNARY)), P_UNARY, tj)
        # widening: zero-extended when the source is unsigned
        w = self.widen(j, sk, 'long' if tb == 64 else 'int')
        if tj in ('byte', 'short', 'char') and w.jt != tj:
            return J('(%s) %s' % (tj, w.at(P_UNARY)), P_UNARY, tj)
        return w

    def widen(self, j, sk, to):
        """A value of C kind sk as a Java int or long of the same value."""
        if sk == 'u8':
            if _is_int_text(j.code):
                return J(str(int(j.code.strip('()').split(')')[-1]) & 0xFF), P_ATOM, to)
            jj = J('%s & 0xFF' % j.at(P_BAND), P_BAND, 'int')
            return jj if to == 'int' else J('(long) (%s)' % jj.code, P_UNARY, 'long')
        if sk == 'u32' and to == 'long':
            return J('Integer.toUnsignedLong(%s)' % j.code, P_POST, 'long')
        if sk == 'u16':
            return J(j.code, j.prec, to) if to == 'int' else J('(long) %s' % j.at(P_UNARY), P_UNARY, 'long')
        if to == 'long' and j.jt != 'long':
            if _is_int_text(j.code):
                return int_lit(int(j.code.strip('()')), 'long')
            return J('(long) %s' % j.at(P_UNARY), P_UNARY, 'long')
        return J(j.code, j.prec, to if j.jt in ('byte', 'short', 'int', 'char') else j.jt)

    def retype(self, j, tj):
        if j.jt == tj:
            return j
        if j.jt in ('int', 'long') and tj in ('byte', 'short', 'char'):
            return J('(%s) %s' % (tj, j.at(P_UNARY)), P_UNARY, tj)
        if j.jt == 'char' and tj == 'int':
            return J(j.code, j.prec, 'int')
        return J(j.code, j.prec, tj)

    def x_Call(self, e):
        args = e.args or []
        if e.sym is not None and e.target is None:
            name = e.sym.name
            if name in self.p.functions:
                f = self.p.function_for(e.sym)
                return self.own_call(f, jname(name), args, e)
            return self.lib_call(e, name, args)
        target = strip(e.target) if e.target is not None else None
        while isinstance(target, ir.Unary) and target.op == '*' and target.x.type is not None and \
                target.x.type.kind == 'fnptr':
            target = strip(target.x)                       # (*fp)(...) calls fp
        if target is not None and target.type is not None and target.type.kind == 'fn':
            target = _Code(self.value(target), ir.Type('fnptr', space='flat', fn=target.type))
        if target is not None and target.type is not None and target.type.kind == 'fnptr':
            e = ir.Call(target=target, sym=None, args=args, typeargs=[], type=e.type)
            ft = target.type
            params = [p['type'] for p in ft.info['fn'].info['params']]
            if not params and args:                    # a K&R pointer `int (*f)()`: typed by the call
                params = [a.type for a in args]
                ft = ir.Type('fnptr', space='flat', fn=ir.Type('fn', conv='cdecl', params=[{'type': t} for t in params],
                                                                result=ft.info['fn'].info['result']))
            sig = self.p.fn_sig_of_type(ft)
            iface = self.p.iface(sig)
            fn = self.value(e.target)
            ac = self.args(args, params, ft.info['fn'].info['result'].get('type'))
            return J('((%s) Fn.at(%s)).call(%s)' % (iface, fn.code, ', '.join(ac)), P_POST, sig[1])
        raise Refused('a call this back end cannot place')

    def args(self, args, params, ret):
        out = []
        if ret is not None and is_record(ret):
            out.append(self.temp(ret))
        for i, a in enumerate(args):
            pt = params[i] if i < len(params) else a.type
            if is_record(pt):
                v = self.value(a)
                if v.fresh:
                    out.append(v.code)
                else:
                    out.append('Mem.copy(%s, %s, %s)' % (self.temp(pt), v.code, self.p.size_code(pt)))
            else:
                out.append(self.conv(self.value(a), a.type, pt).code)
        return out

    def own_call(self, f, jn, args, e):
        s = f.sym
        # the definition's own parameter types (a K&R definition's symbol has no prototype)
        ac = self.args(args, [p.type for p in f.params], s.ret)
        jt = self.p.jt(s.ret) if s.ret is not None else 'void'
        return J('%s(%s)' % (jn, ', '.join(ac)), P_POST, jt, fresh=s.ret is not None and is_record(s.ret))

    def lib_call(self, e, name, args):
        if name.startswith('__builtin_va_') and args:
            a0 = strip(args[0])                      # (the builtins take their va_list by reference)
            if isinstance(a0, ir.Unary) and a0.op == '&' and a0.type is not None and a0.type.info.get('ref'):
                args = [a0.x] + list(args[1:])
        if name == '__builtin_va_start':
            return self.store(self.place(args[0]), J('vaList', P_ATOM, 'int'))
        if name == '__builtin_va_end':
            return J('discard(0)', P_POST, 'void')
        if name == '__builtin_va_copy':
            return self.store(self.place(args[0]), self.value(args[1]))
        if name == '__builtin_va_arg':
            t = e.type
            if is_record(t) or _kind(t) in ('f32',):
                raise Refused('va_arg of %r' % (t,))
            pl = self.place(args[0])
            slot = '(%s += 8) - 8' % pl.java if not pl.mem else 'Mem.postAddPtr(%s, 8)' % pl.java
            return J('Mem.get%s(%s)' % (self.p.access(t), slot), P_POST, self.p.jt(t))
        from codebreaker.translate.backends import java_libc
        return java_libc.call(self, e, name, args)

    def x_New(self, e):
        if e.memory == 'stack' and is_record(e.type):
            tmp = self.temp(e.type)
            lines = self.init_place(tmp, e.type, e)
            # an aggregate value in an expression: written into a temporary in the frame first
            return J(self.seq_lines(lines, tmp), P_POST, 'int', fresh=True)
        raise Refused('new (%s)' % (e.memory,))

    def seq_lines(self, lines, result):
        calls = [l[:-1] for l in lines]
        if not calls:
            return result
        return 'seq(%s, %s)' % (', '.join(calls), result) if len(calls) == 1 else \
            'seq(%s)' % ', '.join(calls + [result])

    def x_NewArray(self, e):
        raise Refused('an array value outside an initialiser')


class Place(object):
    __slots__ = ('java', 'mem', 'type', 'prec')

    def __init__(self, java, mem, type, prec=P_ATOM):
        self.java, self.mem, self.type, self.prec = java, mem, type, prec


class _Code(ir.Expr):
    """A Java expression already written, standing in the tree for a compound assignment's operand."""
    __slots__ = ('j',)
    fields = ()

    def __init__(self, j, t):
        self.j = j
        self.type = t
        self.line = self.col = 0
        self.origin = None


def _is_int_text(code):
    return re.fullmatch(r'\(?-?\d+\)?', code) is not None


def _balanced_call(text):
    depth = 0
    for i, ch in enumerate(text):
        depth += ch == '('
        depth -= ch == ')'
        if depth == 0 and ch == ')' and i != len(text) - 1:
            return False
    return True


def _promoted(jt):
    return 'int' if jt in ('byte', 'short', 'char') else jt


def _promoted2(a, b):
    order = ['int', 'long', 'float', 'double']
    a, b = _promoted(a), _promoted(b)
    if a in order and b in order:
        return order[max(order.index(a), order.index(b))]
    return a


def _promote_type(t):
    k = _kind(t)
    if k in ('i8', 'u8', 'i16', 'u16', 'bool'):
        return ir.I32
    return t
